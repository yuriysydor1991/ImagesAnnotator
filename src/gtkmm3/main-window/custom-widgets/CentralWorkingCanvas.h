/**
 * The "Simplified BSD License"
 *
 * Copyright (c) 2025, Yurii Sydor (yuriysydor1991@gmail.com)
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

#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM3_CUSTOM_WIDGETS_CENTRALWORKINGCANVAS_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM3_CUSTOM_WIDGETS_CENTRALWORKINGCANVAS_CLASS_H

#include <memory>

#include "src/annotator-events/events/ImageRecord.h"
#include "src/gtkmm3/gtkmm_includes.h"
#include "src/gtkmm3/main-window/custom-widgets/ImagePathLabel.h"
#include "src/helpers/TypeHelper.h"

namespace templateGtkmm3
{
class MainWindowContext;
}

namespace templateGtkmm3::window::custom_widgets
{

/**
 * @brief Subclass of the DrawingArea to allow draw annotations over an image.
 */
class CentralWorkingCanvas
    : public Gtk::DrawingArea,
      public std::enable_shared_from_this<CentralWorkingCanvas>,
      virtual public helpers::TypeHelper
{
 public:
  using ImageRecord = events::events::ImageRecord;
  using ImageRecordRectPtr = events::events::ImageRecordRectPtr;
  using ImageRecordPtr = events::events::ImageRecordPtr;

  virtual ~CentralWorkingCanvas() = default;
  explicit CentralWorkingCanvas(std::shared_ptr<ImagePathLabel>& nci);

  virtual bool set_pixbuf(Glib::RefPtr<Gdk::Pixbuf>& npb);

  virtual void clear();

  virtual Glib::RefPtr<Gdk::Pixbuf> get_pixbuf();

  virtual bool mouse_is_over_resize(const int& dx, const int& dy);

  virtual bool is_over_up_left();
  virtual bool is_over_up_right();
  virtual bool is_over_down_left();
  virtual bool is_over_down_right();

  virtual ImageRecordRectPtr get_mouse_over_rect();

 private:
  // Called when the widget needs to redraw
  virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

  virtual void draw_rectangle(const Cairo::RefPtr<Cairo::Context>& cr,
                              const ImageRecordRectPtr& rptr,
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
  Glib::RefPtr<Gdk::Pixbuf> pixbuf;
  ImageRecordRectPtr rectMouseOver;

  // up left corner
  bool isUL{false};
  // up right corner
  bool isUR{false};
  // dow left corner
  bool isDL{false};
  // up right corner
  bool isDR{false};

  /// @brief latest recorded mouse X and Y coordinates
  int x{0};
  int y{0};

  bool resizersDrawed{false};
};

}  // namespace templateGtkmm3::window::custom_widgets

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM3_CUSTOM_WIDGETS_CENTRALWORKINGCANVAS_CLASS_H
