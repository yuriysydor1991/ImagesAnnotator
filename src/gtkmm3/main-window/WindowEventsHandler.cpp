#include "src/gtkmm3/main-window/WindowEventsHandler.h"

#include <cassert>
#include <cmath>
#include <memory>

#include "src/gtkmm3/MainWindowContext.h"
#include "src/gtkmm3/gtkmm_includes.h"
#include "src/gtkmm3/main-window/WindowDataContext.h"
#include "src/gtkmm3/main-window/custom-widgets/ImagePathLabel.h"
#include "src/log/log.h"

namespace templateGtkmm3::window
{

bool WindowEventsHandler::init(std::shared_ptr<MainWindowContext> nmwctx)
{
  assert(MainWindowContext::validate_context(nmwctx));

  if (!MainWindowContext::validate_context(nmwctx)) {
    LOGE("Invalid context pointer provided");
    return false;
  }

  mwctx = nmwctx;

  auto* imagesListBox = mwctx->wloader->get_images_list();
  auto* zoomInB = mwctx->wloader->get_current_image_zoom_in();
  auto* zoomOutB = mwctx->wloader->get_current_image_zoom_out();

  assert(imagesListBox != nullptr);
  assert(zoomInB != nullptr);
  assert(zoomOutB != nullptr);

  imagesListBox->signal_row_selected().connect(
      sigc::mem_fun(*this, &WindowEventsHandler::on_images_row_selected));
  zoomInB->signal_clicked().connect(
      sigc::mem_fun(*this, &WindowEventsHandler::on_zoom_in_clicked));
  zoomOutB->signal_clicked().connect(
      sigc::mem_fun(*this, &WindowEventsHandler::on_zoom_out_clicked));

  return true;
}

void WindowEventsHandler::on_zoom_in_clicked()
{
  assert(MainWindowContext::validate_context(mwctx));

  if (!MainWindowContext::validate_context(mwctx)) {
    LOGE("Invalid context pointer provided");
    return;
  }

  mwctx->imageScale += mwctx->defaultScaleStep;

  update_image_zoom();
}

void WindowEventsHandler::on_zoom_out_clicked()
{
  assert(MainWindowContext::validate_context(mwctx));

  if (!MainWindowContext::validate_context(mwctx)) {
    LOGE("Invalid context pointer provided");
    return;
  }

  mwctx->imageScale -= mwctx->defaultScaleStep;

  update_image_zoom();
}

void WindowEventsHandler::update_image_zoom()
{
  assert(MainWindowContext::validate_context(mwctx));

  if (!MainWindowContext::validate_context(mwctx)) {
    LOGE("Invalid context pointer provided");
    return;
  }

  LOGD("Current image zoom factor: " << mwctx->imageScale);

  auto pb = mwctx->current_image_original_pixbuf;

  const int width = ceilInt(toD(pb->get_width()) * mwctx->imageScale);
  const int height = ceilInt(toD(pb->get_height()) * mwctx->imageScale);

  auto scaled = pb->scale_simple(width, height, Gdk::INTERP_BILINEAR);

  assert(mwctx->centralCanvas != nullptr);

  if (mwctx->centralCanvas == nullptr) {
    LOGE("No central canvas available");
    return;
  }

  mwctx->centralCanvas->set_pixbuf(scaled);
}

template <class Ntype>
inline double WindowEventsHandler::toD(const Ntype& val)
{
  return static_cast<double>(val);
}

template <class Ntype>
inline int WindowEventsHandler::ceilInt(const Ntype& val)
{
  const auto ceiledV = std::ceil(val);

  return static_cast<int>(ceiledV);
}

bool WindowEventsHandler::on_image_scroll(GdkEventScroll* scroll_event)
{
  assert(scroll_event != nullptr);

  if (scroll_event == nullptr) {
    LOGE("No valid event pointer provided");
    return false;
  }

  if (!(scroll_event->state & GDK_SHIFT_MASK)) {
    return false;
  }

  if (scroll_event->direction == GDK_SCROLL_UP) {
    on_zoom_in_clicked();
  } else if (scroll_event->direction == GDK_SCROLL_DOWN) {
    on_zoom_out_clicked();
  } else {
    LOGE("Unknown scroll event type");
    return false;
  }

  return true;
}

void WindowEventsHandler::on_images_row_selected(Gtk::ListBoxRow* row)
{
  assert(MainWindowContext::validate_context(mwctx));

  if (!MainWindowContext::validate_context(mwctx)) {
    LOGE("Invalid context pointer provided");
    return;
  }

  assert(row != nullptr);

  if (row == nullptr) {
    LOGE("No row pointer provided");
    return;
  }

  ImagePathLabel* label = dynamic_cast<ImagePathLabel*>(row->get_child());

  assert(label != nullptr);

  if (label == nullptr) {
    LOGE("Invalid label type for the obtained row widget");
    return;
  }

  mwctx->imageScale = mwctx->scaleInitValue;
  mwctx->current_image = label->shared_from_this();

  auto ir = label->get_image_rec();

  assert(ir != nullptr);

  if (ir == nullptr) {
    LOGE("Invalid image record pointer retrieved");
    return;
  }

  auto eventsFactory = mwctx->actx->eventer->get_events_factory();

  auto event = eventsFactory->create_current_image_changed(ir);

  mwctx->actx->eventer->submit(event);

  mwctx->current_image_original_pixbuf =
      Gdk::Pixbuf::create_from_file(ir->path);

  mwctx->centralCanvas;

  assert(mwctx->centralCanvas != nullptr);

  if (mwctx->centralCanvas == nullptr) {
    LOGE("No central working canvas available");
    return;
  }

  mwctx->centralCanvas->set_pixbuf(mwctx->current_image_original_pixbuf);

  LOGT("New image selected: " << ir->path);
}

}  // namespace templateGtkmm3::window
