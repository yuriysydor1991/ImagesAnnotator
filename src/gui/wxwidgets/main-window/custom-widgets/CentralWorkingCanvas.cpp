/**
 * The "Simplified BSD License"
 *
 * Copyright (c) 2026, Yurii Sydor (yuriysydor1991@gmail.com)
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

#include "src/gui/wxwidgets/main-window/custom-widgets/CentralWorkingCanvas.h"

#include <cassert>

#include "src/log/log.h"

namespace imagesAnnotatorWx::window::custom_widgets
{

namespace
{
// The annotation rectangle colours, matching the GTKmm cairo source_rgb values
// (0.9,0,0) selected / (0,0.2,0.8) otherwise scaled to 0-255.
const wxColour selectedColour(230, 0, 0);
const wxColour normalColour(0, 51, 204);
}  // namespace

CentralWorkingCanvas::CentralWorkingCanvas(wxWindow* parent,
                                           std::shared_ptr<ImagePathLabel>& nci)
    : wxScrolledWindow(parent, wxID_ANY), current_image{nci}
{
  SetScrollRate(10, 10);
  SetBackgroundStyle(wxBG_STYLE_PAINT);

  Bind(wxEVT_PAINT, &CentralWorkingCanvas::on_paint, this);
}

void CentralWorkingCanvas::on_paint(wxPaintEvent& /*event*/)
{
  wxAutoBufferedPaintDC dc(this);
  DoPrepareDC(dc);

  dc.SetBackground(wxBrush(GetBackgroundColour()));
  dc.Clear();

  if (!bitmap.IsOk()) {
    return;
  }

  dc.DrawBitmap(bitmap, 0, 0, false);

  if (current_image == nullptr) {
    LOGT("No image provided");
    return;
  }

  auto ir = current_image->get_image_rec();

  if (ir == nullptr) {
    LOGT("Current widget contains no image record");
    return;
  }

  for (auto& rptr : ir->rects) {
    draw_rectangle(dc, rptr, ir);
  }
}

void CentralWorkingCanvas::draw_rectangle(wxDC& dc,
                                          const ImageRecordRectPtr& rptr,
                                          ImageRecordPtr ir)
{
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

  const bool selected = ir->current_rect == rptr;
  const wxColour& col = selected ? selectedColour : normalColour;

  dc.SetBrush(*wxTRANSPARENT_BRUSH);
  dc.SetPen(wxPen(col, selected ? 2 : 1));

  const double& is = ir->imageScale;

  const int tx = toI(toD(rptr->x) * is);
  const int ty = toI(toD(rptr->y) * is);
  const int tw = toI(toD(rptr->width) * is);
  const int th = toI(toD(rptr->height) * is);

  dc.DrawRectangle(tx, ty, tw, th);

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

  dc.SetPen(wxPen(col, 1));

  // up left corner
  dc.DrawRectangle(tx, ty, cornersPixelSize, cornersPixelSize);
  // up right corner
  dc.DrawRectangle(tx + tw - cornersPixelSize, ty, cornersPixelSize,
                   cornersPixelSize);
  // down left corner
  dc.DrawRectangle(tx, ty + th - cornersPixelSize, cornersPixelSize,
                   cornersPixelSize);
  // down right corner
  dc.DrawRectangle(tx + tw - cornersPixelSize, ty + th - cornersPixelSize,
                   cornersPixelSize, cornersPixelSize);
}

bool CentralWorkingCanvas::set_bitmap(const wxBitmap& npb)
{
  assert(npb.IsOk());

  if (!npb.IsOk()) {
    LOGE("No valid bitmap provided");
    return false;
  }

  bitmap = npb;

  SetVirtualSize(bitmap.GetWidth(), bitmap.GetHeight());

  Refresh();

  return true;
}

void CentralWorkingCanvas::clear()
{
  bitmap = wxNullBitmap;

  Refresh();
}

wxBitmap CentralWorkingCanvas::get_bitmap() { return bitmap; }

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

  const bool nResizersDraw = is_inside_or_near_by_any(dx, dy, ir);

  if (nResizersDraw || (resizersDrawed != nResizersDraw)) {
    resizersDrawed = nResizersDraw;
    LOGT("Need to redraw since cursor near by some rect");
    Refresh();
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
  // down left corner
  isDL = is_in_rect(dx, dy, tx, ty + th - cornersPixelSize, cornersPixelSize,
                    cornersPixelSize);
  // down right corner
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

}  // namespace imagesAnnotatorWx::window::custom_widgets
