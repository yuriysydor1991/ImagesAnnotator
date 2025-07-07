#include "src/gtkmm3/main-window/custom-widgets/CentralWorkingCanvas.h"

#include <cassert>
#include <memory>

#include "src/annotator-events/events/ImageRecord.h"
#include "src/gtkmm3/MainWindowContext.h"
#include "src/gtkmm3/gtkmm_includes.h"
#include "src/gtkmm3/main-window/WindowDataContext.h"
#include "src/log/log.h"

namespace templateGtkmm3::window::custom_widgets
{

CentralWorkingCanvas::CentralWorkingCanvas(std::shared_ptr<ImagePathLabel>& nci)
    : current_image{nci}
{
}

bool CentralWorkingCanvas::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
  if (!pixbuf) {
    return true;
  }

  Gdk::Cairo::set_source_pixbuf(cr, pixbuf, 0, 0);

  cr->paint();

  if (current_image == nullptr) {
    LOGT("No image provided");
    return true;
  }

  auto ir = current_image->get_image_rec();

  if (ir == nullptr) {
    LOGT("Current widget contains no image record");
    return true;
  }

  for (auto& rptr : ir->rects) {
    draw_rectangle(cr, rptr, ir);
  }

  return true;
}

void CentralWorkingCanvas::draw_rectangle(
    const Cairo::RefPtr<Cairo::Context>& cr, const ImageRecordRectPtr& rptr,
    ImageRecordPtr ir)
{
  assert(cr);
  assert(rptr != nullptr);
  assert(ir != nullptr);

  if (rptr == nullptr) {
    LOGE("Invalid rectangle pointer");
    return;
  }

  if (ir == nullptr) {
    LOGE("No image record pointer provided");
    return;
  }

  cr->set_line_width(2.0);

  if (ir->current_rect == rptr) {
    cr->set_line_width(2.0);
    cr->set_source_rgb(0.9, 0.0, 0.0);
  } else {
    cr->set_line_width(1.0);
    cr->set_source_rgb(0.0, 0.2, 0.8);
  }

  const double& is = ir->imageScale;

  const int tx = rptr->x * is;
  const int ty = rptr->y * is;
  const int tw = rptr->width * is;
  const int th = rptr->height * is;

  cr->rectangle(tx, ty, tw, th);

  cr->stroke();

  static const int minSize = cornersPixelSize * 2 + 2;

  if (tw < minSize) {
    LOGD("The rectangle has no width to draw resize corners");
    return;
  }

  if (th < minSize) {
    LOGD("The rectangle has no height to draw resize corners");
    return;
  }

  cr->set_line_width(1.0);

  // up left corner
  cr->rectangle(tx, ty, cornersPixelSize, cornersPixelSize);
  // up right corner
  cr->rectangle(tx + tw - cornersPixelSize, ty, cornersPixelSize,
                cornersPixelSize);
  // dow left corner
  cr->rectangle(tx, ty + th - cornersPixelSize, cornersPixelSize,
                cornersPixelSize);
  // up right corner
  cr->rectangle(tx + tw - cornersPixelSize, ty + th - cornersPixelSize,
                cornersPixelSize, cornersPixelSize);

  cr->stroke();
}

bool CentralWorkingCanvas::set_pixbuf(Glib::RefPtr<Gdk::Pixbuf>& npb)
{
  assert(npb);

  if (!npb) {
    LOGE("No valid pixbuf pointer provided");
    return false;
  }

  pixbuf = npb;

  set_size_request(pixbuf->get_width(), pixbuf->get_height());

  queue_draw();

  return true;
}

void CentralWorkingCanvas::clear()
{
  pixbuf.reset();

  queue_draw();
}

Glib::RefPtr<Gdk::Pixbuf> CentralWorkingCanvas::get_pixbuf() { return pixbuf; }

bool CentralWorkingCanvas::mouse_is_over_resize(const int& dx, const int& dy)
{
  if (current_image == nullptr) {
    LOGT("No image provided");
    return false;
  }

  auto ir = current_image->get_image_rec();

  if (ir == nullptr) {
    LOGT("Current widget contains no image record");
    return false;
  }

  for (auto& rptr : ir->rects) {
    if (!is_in_one_of_resize_rects(dx, dy, rptr, ir)) {
      continue;
    }

    LOGT("Found inside one of the rects");
    return true;
  }

  return false;
}

bool CentralWorkingCanvas::is_in_rect(const int& dx, const int& dy,
                                      const int& rx, const int& ry,
                                      const int& rw, const int& rh)
{
  LOGT("Checking the [" << dx << "," << dy << "] to [" << rx << "," << ry << ","
                        << (rx + rw) << "," << (ry + rh) << "]");

  const bool isInX = dx >= rx && dx <= (rx + rw);
  const bool isInY = dy >= ry && dy <= (ry + rh);

  return isInX && isInY;
}

bool CentralWorkingCanvas::is_in_one_of_resize_rects(
    const int& dx, const int& dy, const ImageRecordRectPtr& rptr,
    ImageRecordPtr ir)
{
  assert(rptr != nullptr);
  assert(ir != nullptr);

  if (rptr == nullptr) {
    LOGE("Invalid rectangle pointer");
    return false;
  }

  if (ir == nullptr) {
    LOGE("No image record pointer provided");
    return false;
  }

  const double& is = ir->imageScale;

  const int tx = rptr->x * is;
  const int ty = rptr->y * is;
  const int tw = rptr->width * is;
  const int th = rptr->height * is;

  if (!is_in_rect(dx, dy, tx, ty, tw, th)) {
    LOGT("Given point is out of the main rectangle");
    return false;
  }

  rectMouseOver = rptr;

  // up left corner
  isUL = is_in_rect(dx, dy, tx, ty, cornersPixelSize, cornersPixelSize);
  // up right corner
  isUR = is_in_rect(dx, dy, tx + tw - cornersPixelSize, ty, cornersPixelSize,
                    cornersPixelSize);
  // dow left corner
  isDL = is_in_rect(dx, dy, tx, ty + th - cornersPixelSize, cornersPixelSize,
                    cornersPixelSize);
  // up right corner
  isDR =
      is_in_rect(dx, dy, tx + tw - cornersPixelSize, ty + th - cornersPixelSize,
                 cornersPixelSize, cornersPixelSize);

  return isUL || isUR || isDL || isDR;
}

bool CentralWorkingCanvas::is_over_up_left() { return isUL; }

bool CentralWorkingCanvas::is_over_up_right() { return isUR; }

bool CentralWorkingCanvas::is_over_down_left() { return isDL; }

bool CentralWorkingCanvas::is_over_down_right() { return isDR; }

CentralWorkingCanvas::ImageRecordRectPtr
CentralWorkingCanvas::get_mouse_over_rect()
{
  return rectMouseOver;
}

}  // namespace templateGtkmm3::window::custom_widgets
