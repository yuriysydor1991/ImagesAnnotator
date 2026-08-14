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

#include "src/gui/qt6/main-window/custom-widgets/CentralWorkingCanvas.h"

#include <cassert>

#include "src/log/log.h"

namespace imagesAnnotatorQt6::window::custom_widgets
{

namespace
{
// The annotation rectangle colours, matching the GTKmm cairo source_rgb values
// (0.9,0,0) selected / (0,0.2,0.8) otherwise scaled to 0-255.
const QColor selectedColour(230, 0, 0);
const QColor normalColour(0, 51, 204);

/// @brief One mouse wheel detent in QWheelEvent::angleDelta() units.
constexpr int wheelStepDegrees = 120;

// QPen takes its width as a qreal; the selected rectangle is drawn with the
// thicker one, matching the GTKmm cairo line widths.
constexpr qreal selectedWidth = 2.0;
constexpr qreal normalWidth = 1.0;
}  // namespace

CentralWorkingCanvas::CentralWorkingCanvas(QWidget* parent,
                                           std::shared_ptr<ImagePathLabel>& nci)
    : QWidget{parent}, current_image{nci}
{
  // Without mouse tracking Qt only delivers move events while a button is held,
  // and the resize-handle cursor feedback (which the GTKmm/wx canvases give on
  // a plain hover) would never fire.
  setMouseTracking(true);
}

void CentralWorkingCanvas::paintEvent(QPaintEvent* /*event*/)
{
  QPainter painter{this};

  if (pixmap.isNull()) {
    return;
  }

  painter.drawPixmap(0, 0, pixmap);

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
    draw_rectangle(painter, rptr, ir);
  }
}

void CentralWorkingCanvas::draw_rectangle(QPainter& painter,
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
  const QColor& col = selected ? selectedColour : normalColour;

  painter.setBrush(Qt::NoBrush);
  painter.setPen(QPen{col, selected ? selectedWidth : normalWidth});

  const double& is = ir->imageScale;

  const int tx = toI(toD(rptr->x) * is);
  const int ty = toI(toD(rptr->y) * is);
  const int tw = toI(toD(rptr->width) * is);
  const int th = toI(toD(rptr->height) * is);

  painter.drawRect(tx, ty, tw, th);

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

  painter.setPen(QPen{col, normalWidth});

  // up left corner
  painter.drawRect(tx, ty, cornersPixelSize, cornersPixelSize);
  // up right corner
  painter.drawRect(tx + tw - cornersPixelSize, ty, cornersPixelSize,
                   cornersPixelSize);
  // down left corner
  painter.drawRect(tx, ty + th - cornersPixelSize, cornersPixelSize,
                   cornersPixelSize);
  // down right corner
  painter.drawRect(tx + tw - cornersPixelSize, ty + th - cornersPixelSize,
                   cornersPixelSize, cornersPixelSize);
}

void CentralWorkingCanvas::mousePressEvent(QMouseEvent* event)
{
  assert(event != nullptr);

  if (event->button() != Qt::LeftButton) {
    QWidget::mousePressEvent(event);
    return;
  }

  const QPoint p = event->position().toPoint();

  emit mousePressedAt(p.x(), p.y());
}

void CentralWorkingCanvas::mouseMoveEvent(QMouseEvent* event)
{
  assert(event != nullptr);

  const QPoint p = event->position().toPoint();

  emit mouseMovedAt(p.x(), p.y());
}

void CentralWorkingCanvas::mouseReleaseEvent(QMouseEvent* event)
{
  assert(event != nullptr);

  if (event->button() != Qt::LeftButton) {
    QWidget::mouseReleaseEvent(event);
    return;
  }

  emit mouseReleased();
}

void CentralWorkingCanvas::wheelEvent(QWheelEvent* event)
{
  assert(event != nullptr);

  if (!event->modifiers().testFlag(Qt::ShiftModifier)) {
    // Not a zoom gesture - hand it back so the enclosing QScrollArea scrolls,
    // exactly as the GTKmm and wx canvases do.
    event->ignore();
    return;
  }

  const int degrees = event->angleDelta().y();

  if (degrees == 0) {
    event->ignore();
    return;
  }

  event->accept();

  emit zoomStepRequested(degrees / wheelStepDegrees);
}

bool CentralWorkingCanvas::set_pixmap(const QPixmap& npb)
{
  assert(!npb.isNull());

  if (npb.isNull()) {
    LOGE("No valid pixmap provided");
    return false;
  }

  pixmap = npb;

  // The widget is its own scrolled content: sizing it to the image is what
  // makes the enclosing QScrollArea show scroll bars and keeps the widget
  // coordinates equal to the image ones.
  setFixedSize(pixmap.size());

  update();

  return true;
}

void CentralWorkingCanvas::clear()
{
  pixmap = QPixmap{};

  // Drop the fixed size the last image imposed, so an empty canvas stops
  // reserving the scroll area the previous image needed.
  setMinimumSize(0, 0);
  setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
  resize(0, 0);

  update();
}

QPixmap CentralWorkingCanvas::get_pixmap() { return pixmap; }

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
    update();
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

}  // namespace imagesAnnotatorQt6::window::custom_widgets
