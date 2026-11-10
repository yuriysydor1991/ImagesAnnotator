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

#ifndef IMAGES_ANNOTATOR_PROJECT_QT6_CUSTOM_WIDGETS_CENTRALWORKINGCANVAS_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_QT6_CUSTOM_WIDGETS_CENTRALWORKINGCANVAS_CLASS_H

#include <cstddef>
#include <memory>

#include "src/annotator-events/events/ImageRecord.h"
#include "src/annotator-events/events/ImageRecordSegment.h"
#include "src/gui/qt6/main-window/custom-widgets/ImagePathLabel.h"
#include "src/gui/qt6/qt_includes.h"
#include "src/helpers/TypeHelper.h"

namespace imagesAnnotatorQt6::window::custom_widgets
{

/**
 * @brief Widget that draws the current image plus its annotation rectangles -
 * the Qt analogue of imagesAnnotatorGtkmm3's CentralWorkingCanvas (a
 * Gtk::DrawingArea). The rendering pipeline, the coordinate scaling by
 * ImageRecord::imageScale and the resize-handle hit-testing are ported one to
 * one; only the drawing calls change from cairo to QPainter.
 *
 * The widget is resized to exactly the scaled pixmap and placed inside the
 * window's QScrollArea, so its own coordinate space is the image space the
 * GTKmm drawing area used - no viewport translation is needed anywhere.
 *
 * It is owned by its parent widget (Qt's parent-child ownership), so it is
 * referenced through a raw pointer rather than a shared_ptr. The mouse and
 * wheel events it receives are re-emitted as Qt signals, which is what lets the
 * decomposed event handlers connect to them without subclassing the widget.
 */
class CentralWorkingCanvas : public QWidget, virtual public helpers::TypeHelper
{
  Q_OBJECT

 public:
  using ImageRecord = events::events::ImageRecord;
  using ImageRecordRectPtr = events::events::ImageRecordRectPtr;
  using ImageRecordPtr = events::events::ImageRecordPtr;
  using ImageRecordSegmentPtr = events::events::ImageRecordSegmentPtr;

  ~CentralWorkingCanvas() override = default;
  CentralWorkingCanvas(QWidget* parent, std::shared_ptr<ImagePathLabel>& nci);

  /// @brief Install the already-scaled image to display and resize the widget
  /// to it (the Qt analogue of set_pixbuf()).
  virtual bool set_pixmap(const QPixmap& npb);

  virtual void clear();

  virtual QPixmap get_pixmap();

  virtual bool mouse_is_over_resize(const int& dx, const int& dy);

  virtual bool is_over_up_left();
  virtual bool is_over_up_right();
  virtual bool is_over_down_left();
  virtual bool is_over_down_right();

  virtual ImageRecordRectPtr get_mouse_over_rect();

  /**
   * @brief Tells whether the mouse is over a point of a segment path, looked
   * for in the current annotation first and in the others from the topmost
   * drawn one down, and remembers the point for the get_mouse_over_segment()
   * and the get_mouse_over_point(). The point is drawn filled as the
   * selected one.
   */
  virtual bool mouse_is_over_point(const int& dx, const int& dy);

  /// @brief The segment of the point the mouse was over last, or nullptr
  virtual ImageRecordSegmentPtr get_mouse_over_segment();
  /// @brief The index of the point the mouse was over last, or
  /// helpers::SegmentPathHelper::npos
  virtual std::size_t get_mouse_over_point();

  /// @brief Sets the segment whose path the Shift-held clicks are drawing,
  /// which is drawn open with the closing edge dashed; nullptr for none.
  virtual void set_drawn_segment(const ImageRecordSegmentPtr& nsegment);
  virtual ImageRecordSegmentPtr get_drawn_segment();

 signals:
  /// @brief Left mouse button pressed at the given image-space coordinates;
  /// @c shift tells whether the Shift key was held.
  void mousePressedAt(int x, int y, bool shift);

  /// @brief Mouse moved to the given image-space coordinates (mouse tracking is
  /// on, so this fires with no button held as well); @c shift tells whether
  /// the Shift key was held.
  void mouseMovedAt(int x, int y, bool shift);

  /// @brief Left mouse button released.
  void mouseReleased();

  /// @brief Shift + mouse wheel over the image. @c steps is positive when the
  /// wheel was rotated towards the user's screen (zoom in) and negative
  /// otherwise. A wheel event without Shift is left to the enclosing
  /// QScrollArea instead of being reported here.
  void zoomStepRequested(int steps);

 protected:
  void paintEvent(QPaintEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void wheelEvent(QWheelEvent* event) override;

 private:
  virtual void draw_rectangle(QPainter& painter, const ImageRecordRectPtr& rptr,
                              ImageRecordPtr ir);

  /// @brief Draws the open path of the segment being drawn
  virtual void draw_segment_path(QPainter& painter,
                                 const ImageRecordSegmentPtr& segment,
                                 ImageRecordPtr ir);

  /// @brief Draws the handles of the segment path points
  virtual void draw_segment_points(QPainter& painter,
                                   const ImageRecordSegmentPtr& segment,
                                   ImageRecordPtr ir);

  virtual bool is_in_one_of_resize_rects(const int& dx, const int& dy,
                                         const ImageRecordRectPtr& rptr,
                                         ImageRecordPtr ir);

  virtual bool is_inside_or_near_by(const int& dx, const int& dy,
                                    const ImageRecordRectPtr& rptr,
                                    ImageRecordPtr ir);

  virtual bool is_inside_or_near_by_any(const int& dx, const int& dy,
                                        ImageRecordPtr ir);

  virtual bool is_in_rect(const int& dx, const int& dy, const int& rx,
                          const int& ry, const int& rw, const int& rh);

  inline static constexpr const int cornersPixelSize = 10;
  inline static constexpr const int nearByDistPixelSize = cornersPixelSize;
  /// @brief The side of the square handle of a segment path point
  inline static constexpr const int pointPixelSize = 6;

  std::shared_ptr<ImagePathLabel>& current_image;
  QPixmap pixmap;
  ImageRecordRectPtr rectMouseOver;
  ImageRecordSegmentPtr drawnSegment;
  ImageRecordSegmentPtr segmentMouseOver;
  std::size_t pointMouseOver;

  // up left / up right / down left / down right corner under the cursor
  bool isUL{false};
  bool isUR{false};
  bool isDL{false};
  bool isDR{false};

  /// @brief latest recorded mouse X and Y coordinates (image space)
  int x{0};
  int y{0};

  bool resizersDrawed{false};
};

}  // namespace imagesAnnotatorQt6::window::custom_widgets

#endif  // IMAGES_ANNOTATOR_PROJECT_QT6_CUSTOM_WIDGETS_CENTRALWORKINGCANVAS_CLASS_H
