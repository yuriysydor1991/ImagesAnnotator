/**
 * The "Simplified BSD License"
 *
 * Copyright (c) 2025, 2026, Yurii Sydor (yuriysydor1991@gmail.com)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   - Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "src/gui/gtkmm3/main-window/custom-widgets/CentralWorkingCanvas.h"

#include <algorithm>
#include <cassert>
#include <memory>
#include <vector>

#include "src/annotator-events/events/ImageRecord.h"
#include "src/gui/gtkmm3/MainWindowContext.h"
#include "src/gui/gtkmm3/gtkmm_includes.h"
#include "src/gui/gtkmm3/main-window/WindowDataContext.h"
#include "src/helpers/SegmentPathHelper.h"
#include "src/log/log.h"

namespace imagesAnnotatorGtkmm3::window::custom_widgets
{

CentralWorkingCanvas::CentralWorkingCanvas(std::shared_ptr<ImagePathLabel>& nci)
    : current_image{nci}, pointMouseOver{helpers::SegmentPathHelper::npos}
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

  const int tx = toI(toD(rptr->x) * is);
  const int ty = toI(toD(rptr->y) * is);
  const int tw = toI(toD(rptr->width) * is);
  const int th = toI(toD(rptr->height) * is);

  const auto segment = helpers::SegmentPathHelper::as_segment(rptr);

  if (segment != nullptr && segment == drawnSegment) {
    draw_segment_path(cr, segment, ir);
    return;
  }

  for (const auto& [px, py] : rptr->outline()) {
    cr->line_to(px * is, py * is);
  }

  cr->close_path();
  cr->stroke();

  if (segment != nullptr &&
      (ir->current_rect == rptr || is_inside_or_near_by(x, y, rptr, ir))) {
    draw_segment_points(cr, segment, ir);
  }

  static const int minSize = cornersPixelSize * 2 + 2;

  if (tw < minSize) {
    LOGD("The rectangle has no width to draw resize corners");
    return;
  }

  if (th < minSize) {
    LOGD("The rectangle has no height to draw resize corners");
    return;
  }

  if (!is_inside_or_near_by(x, y, rptr, ir)) {
    LOGT("Mouse in not inside nor near by the given rectangle");
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

void CentralWorkingCanvas::draw_segment_path(
    const Cairo::RefPtr<Cairo::Context>& cr,
    const ImageRecordSegmentPtr& segment, ImageRecordPtr ir)
{
  assert(cr);
  assert(segment != nullptr);
  assert(ir != nullptr);

  const double& is = ir->imageScale;

  const auto image = helpers::SegmentPathHelper::image_points(*segment);

  if (image.empty()) {
    LOGT("The drawn segment has no points yet");
    return;
  }

  for (const auto& [px, py] : image) {
    cr->line_to(px * is, py * is);
  }

  cr->stroke();

  if (helpers::SegmentPathHelper::is_closable(*segment)) {
    // The edge the Shift key release loops the path with
    const auto& [fx, fy] = image.front();
    const auto& [lx, ly] = image.back();

    cr->save();
    cr->set_dash(std::vector<double>{4.0, 4.0}, 0.0);
    cr->move_to(lx * is, ly * is);
    cr->line_to(fx * is, fy * is);
    cr->stroke();
    cr->restore();
  }

  draw_segment_points(cr, segment, ir);
}

void CentralWorkingCanvas::draw_segment_points(
    const Cairo::RefPtr<Cairo::Context>& cr,
    const ImageRecordSegmentPtr& segment, ImageRecordPtr ir)
{
  assert(cr);
  assert(segment != nullptr);
  assert(ir != nullptr);

  const double& is = ir->imageScale;
  const double half = pointPixelSize / 2.0;

  const auto image = helpers::SegmentPathHelper::image_points(*segment);

  cr->set_line_width(1.0);

  for (std::size_t index = 0U; index < image.size(); ++index) {
    const auto& [px, py] = image[index];

    cr->rectangle(px * is - half, py * is - half, pointPixelSize,
                  pointPixelSize);

    if (segment == segmentMouseOver && index == pointMouseOver) {
      cr->fill();
    } else {
      cr->stroke();
    }
  }
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
  x = dx;
  y = dy;

  if (current_image == nullptr) {
    LOGT("No image provided");
    return false;
  }

  auto ir = current_image->get_image_rec();

  if (ir == nullptr) {
    LOGT("Current widget contains no image record");
    return false;
  }

  const bool nResizersDraw =
      is_inside_or_near_by_any(dx, dy, current_image->get_image_rec());

  if (nResizersDraw || (resizersDrawed != nResizersDraw)) {
    resizersDrawed = nResizersDraw;
    LOGT("Need to redraw since cursor near by some rect");
    queue_draw();
  }

  for (auto& rptr : ir->rects) {
    // The segment being drawn has no resize corners drawn
    if (rptr == drawnSegment) {
      continue;
    }

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

bool CentralWorkingCanvas::is_inside_or_near_by_any(const int& dx,
                                                    const int& dy,
                                                    ImageRecordPtr ir)
{
  if (ir == nullptr) {
    LOGT("Current widget contains no image record");
    return false;
  }

  for (auto& rptr : ir->rects) {
    if (!is_inside_or_near_by(dx, dy, rptr, ir)) {
      continue;
    }

    LOGT("Found inside one of the rects");
    return true;
  }

  return false;
}

bool CentralWorkingCanvas::is_inside_or_near_by(const int& dx, const int& dy,
                                                const ImageRecordRectPtr& rptr,
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

  const int tx = toI(toD(rptr->x) * is) - nearByDistPixelSize;
  const int ty = toI(toD(rptr->y) * is) - nearByDistPixelSize;
  const int tw = toI(toD(rptr->width) * is) + nearByDistPixelSize;
  const int th = toI(toD(rptr->height) * is) + nearByDistPixelSize;

  return is_in_rect(dx, dy, tx, ty, tw, th);
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

  const int tx = toI(toD(rptr->x) * is);
  const int ty = toI(toD(rptr->y) * is);
  const int tw = toI(toD(rptr->width) * is);
  const int th = toI(toD(rptr->height) * is);

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

bool CentralWorkingCanvas::mouse_is_over_point(const int& dx, const int& dy)
{
  using helpers::SegmentPathHelper;

  const auto lastSegment = segmentMouseOver;
  const auto lastPoint = pointMouseOver;

  segmentMouseOver.reset();
  pointMouseOver = SegmentPathHelper::npos;

  auto ir = current_image != nullptr ? current_image->get_image_rec() : nullptr;

  bool found = false;

  if (ir != nullptr) {
    const auto is_over = [&](const ImageRecordRectPtr& rptr) -> bool {
      const auto segment = SegmentPathHelper::as_segment(rptr);

      if (segment == nullptr || segment == drawnSegment) {
        return false;
      }

      const auto index = SegmentPathHelper::find_point(*segment, toD(dx),
                                                       toD(dy), ir->imageScale);

      if (index == SegmentPathHelper::npos) {
        return false;
      }

      segmentMouseOver = segment;
      pointMouseOver = index;

      return true;
    };

    found = is_over(ir->current_rect) ||
            std::any_of(ir->rects.rbegin(), ir->rects.rend(), is_over);
  }

  if (lastSegment != segmentMouseOver || lastPoint != pointMouseOver) {
    LOGT("Need to redraw since the selected segment point changed");
    queue_draw();
  }

  return found;
}

CentralWorkingCanvas::ImageRecordSegmentPtr
CentralWorkingCanvas::get_mouse_over_segment()
{
  return segmentMouseOver;
}

std::size_t CentralWorkingCanvas::get_mouse_over_point()
{
  return pointMouseOver;
}

void CentralWorkingCanvas::set_drawn_segment(
    const ImageRecordSegmentPtr& nsegment)
{
  drawnSegment = nsegment;

  queue_draw();
}

CentralWorkingCanvas::ImageRecordSegmentPtr
CentralWorkingCanvas::get_drawn_segment()
{
  return drawnSegment;
}

}  // namespace imagesAnnotatorGtkmm3::window::custom_widgets
