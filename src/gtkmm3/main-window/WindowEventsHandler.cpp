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
  auto& drawArea = mwctx->centralCanvas;

  assert(imagesListBox != nullptr);
  assert(zoomInB != nullptr);
  assert(zoomOutB != nullptr);
  assert(drawArea != nullptr);

  imagesListBox->signal_row_selected().connect(
      sigc::mem_fun(*this, &WindowEventsHandler::on_images_row_selected));
  zoomInB->signal_clicked().connect(
      sigc::mem_fun(*this, &WindowEventsHandler::on_zoom_in_clicked));
  zoomOutB->signal_clicked().connect(
      sigc::mem_fun(*this, &WindowEventsHandler::on_zoom_out_clicked));

  drawArea->signal_button_press_event().connect(
      sigc::mem_fun(*this, &WindowEventsHandler::on_rectangle_draw_start));
  drawArea->signal_button_release_event().connect(
      sigc::mem_fun(*this, &WindowEventsHandler::on_rectangle_draw_end));
  drawArea->signal_motion_notify_event().connect(
      sigc::mem_fun(*this, &WindowEventsHandler::on_rectangle_size_change));

  return true;
}

bool WindowEventsHandler::on_rectangle_draw_start(GdkEventButton* event)
{
  assert(mwctx != nullptr);
  assert(event != nullptr);

  if (event->button != 1) {
    return true;
  }

  if (mwctx->current_image == nullptr) {
    LOGT("No current image available");
    return true;
  }

  mwctx->current_image->mark_as_has_records();

  auto ir = mwctx->current_image->get_image_rec();

  assert(ir != nullptr);

  auto efactory = mwctx->actx->eventer->get_events_factory();

  assert(efactory != nullptr);

  if (efactory == nullptr) {
    LOGE("No factory available");
    return true;
  }

  ir->current_rect = efactory->create_image_rect_record();
  ir->rects.insert(ir->current_rect);

  assert(ir->current_rect != nullptr);

  const auto& imageScale = ir->imageScale;

  ir->current_rect->x = event->x / imageScale;
  ir->current_rect->y = event->y / imageScale;

  return true;
}

bool WindowEventsHandler::on_rectangle_draw_end(GdkEventButton* event)
{
  assert(mwctx != nullptr);
  assert(mwctx->centralCanvas != nullptr);
  assert(event != nullptr);

  if (mwctx->current_image == nullptr) {
    LOGT("No current image available");
    return true;
  }

  auto ir = mwctx->current_image->get_image_rec();

  assert(ir != nullptr);

  if (ir == nullptr) {
    LOGT("No current image available");
    return true;
  }

  if (event->button != 1 || ir->current_rect == nullptr) {
    return true;
  }

  ir->current_rect.reset();

  assert(mwctx->centralCanvas != nullptr);

  mwctx->centralCanvas->queue_draw();

  return true;
}

bool WindowEventsHandler::on_rectangle_size_change(GdkEventMotion* event)
{
  assert(mwctx != nullptr);
  assert(event != nullptr);

  if (mwctx == nullptr) {
    LOGT("No context available");
    return true;
  }

  if (mwctx->current_image == nullptr) {
    LOGT("No current image available");
    return true;
  }

  auto ir = mwctx->current_image->get_image_rec();

  assert(ir != nullptr);

  if (ir == nullptr) {
    LOGT("No current image available");
    return true;
  }

  if (ir->current_rect == nullptr) {
    LOGT("No current image current rectangle available");
    return true;
  }

  const auto& imageScale = ir->imageScale;

  const double start_x = toD(ir->current_rect->x);
  const double start_y = toD(ir->current_rect->y);
  const double end_x = event->x / imageScale;
  const double end_y = event->y / imageScale;

  ir->current_rect->x = toI(std::min(start_x, end_x));
  ir->current_rect->y = toI(std::min(start_y, end_y));
  ir->current_rect->width = toI(std::abs(end_x - start_x));
  ir->current_rect->height = toI(std::abs(end_y - start_y));

  assert(mwctx->centralCanvas != nullptr);

  mwctx->centralCanvas->queue_draw();

  return true;
}

void WindowEventsHandler::on_zoom_in_clicked()
{
  assert(MainWindowContext::validate_context(mwctx));

  if (!MainWindowContext::validate_context(mwctx)) {
    LOGE("Invalid context pointer provided");
    return;
  }

  if (mwctx->current_image == nullptr) {
    LOGT("No image available");
    return;
  }

  assert(mwctx->current_image->get_image_rec() != nullptr);

  mwctx->current_image->get_image_rec()->imageScale +=
      events::events::ImageRecord::defaultScaleStep;

  update_image_zoom();
}

void WindowEventsHandler::on_zoom_out_clicked()
{
  assert(MainWindowContext::validate_context(mwctx));

  if (!MainWindowContext::validate_context(mwctx)) {
    LOGE("Invalid context pointer provided");
    return;
  }

  if (mwctx->current_image == nullptr) {
    LOGT("No image available");
    return;
  }

  assert(mwctx->current_image->get_image_rec() != nullptr);

  mwctx->current_image->get_image_rec()->imageScale -=
      events::events::ImageRecord::defaultScaleStep;

  update_image_zoom();
}

void WindowEventsHandler::update_image_zoom()
{
  assert(MainWindowContext::validate_context(mwctx));

  if (!MainWindowContext::validate_context(mwctx)) {
    LOGE("Invalid context pointer provided");
    return;
  }

  if (mwctx->current_image == nullptr ||
      mwctx->current_image->get_image_rec() == nullptr) {
    LOGT("No image available");
    return;
  }

  assert(mwctx->current_image->get_image_rec() != nullptr);

  const auto currentScale = mwctx->current_image->get_image_rec()->imageScale;

  LOGD("Current image zoom factor: " << currentScale);

  auto pb = mwctx->current_image_original_pixbuf;

  const int width = ceilInt(toD(pb->get_width()) * currentScale);
  const int height = ceilInt(toD(pb->get_height()) * currentScale);

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

template <class Ntype>
int WindowEventsHandler::toI(const Ntype& val)
{
  return static_cast<int>(val);
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

  update_image_zoom();

  LOGT("New image selected: " << ir->path);
}

}  // namespace templateGtkmm3::window
