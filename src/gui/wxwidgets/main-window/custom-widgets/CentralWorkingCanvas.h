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

#ifndef IMAGES_ANNOTATOR_PROJECT_WX_CUSTOM_WIDGETS_CENTRALWORKINGCANVAS_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_WX_CUSTOM_WIDGETS_CENTRALWORKINGCANVAS_CLASS_H

#include <memory>

#include "src/annotator-events/events/ImageRecord.h"
#include "src/gui/wxwidgets/main-window/custom-widgets/ImagePathLabel.h"
#include "src/gui/wxwidgets/wx_includes.h"
#include "src/helpers/TypeHelper.h"

namespace templateWx::window::custom_widgets
{

/**
 * @brief Scrolled canvas that draws the current image plus its annotation
 * rectangles - the wxWidgets analogue of templateGtkmm3's CentralWorkingCanvas
 * (a Gtk::DrawingArea). The rendering pipeline, coordinate scaling by
 * ImageRecord::imageScale and the resize-handle hit-testing are ported one to
 * one; only the drawing calls change from cairo to wxDC.
 *
 * The widget is owned by its parent wxWindow (created with new, destroyed by
 * wxWidgets), so it is referenced through a raw pointer rather than a
 * shared_ptr.
 */
class CentralWorkingCanvas : public wxScrolledWindow,
                             virtual public helpers::TypeHelper
{
 public:
  using ImageRecord = events::events::ImageRecord;
  using ImageRecordRectPtr = events::events::ImageRecordRectPtr;
  using ImageRecordPtr = events::events::ImageRecordPtr;

  ~CentralWorkingCanvas() override = default;
  CentralWorkingCanvas(wxWindow* parent, std::shared_ptr<ImagePathLabel>& nci);

  /// @brief Install the already-scaled image to display and resize the scrolled
  /// virtual area to it (the wx analogue of set_pixbuf()).
  virtual bool set_bitmap(const wxBitmap& npb);

  virtual void clear();

  virtual wxBitmap get_bitmap();

  virtual bool mouse_is_over_resize(const int& dx, const int& dy);

  virtual bool is_over_up_left();
  virtual bool is_over_up_right();
  virtual bool is_over_down_left();
  virtual bool is_over_down_right();

  virtual ImageRecordRectPtr get_mouse_over_rect();

 private:
  void on_paint(wxPaintEvent& event);

  virtual void draw_rectangle(wxDC& dc, const ImageRecordRectPtr& rptr,
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

  std::shared_ptr<ImagePathLabel>& current_image;
  wxBitmap bitmap;
  ImageRecordRectPtr rectMouseOver;

  // up left / up right / down left / down right corner under the cursor
  bool isUL{false};
  bool isUR{false};
  bool isDL{false};
  bool isDR{false};

  /// @brief latest recorded mouse X and Y coordinates (image/virtual space)
  int x{0};
  int y{0};

  bool resizersDrawed{false};
};

}  // namespace templateWx::window::custom_widgets

#endif  // IMAGES_ANNOTATOR_PROJECT_WX_CUSTOM_WIDGETS_CENTRALWORKINGCANVAS_CLASS_H
