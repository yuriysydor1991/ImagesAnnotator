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

#ifndef IMAGES_ANNOTATOR_PROJECT_GTKMM_EVENTHANDLERS_MOUSEDRAWEVENTSHANDLER_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_GTKMM_EVENTHANDLERS_MOUSEDRAWEVENTSHANDLER_CLASS_H

#include <functional>
#include <memory>

#include "src/gui/gtkmm3/gtkmm_includes.h"
#include "src/gui/gtkmm3/main-window/event-handlers/AbstractWindowEventHandler.h"
#include "src/helpers/TypeHelper.h"

namespace imagesAnnotatorGtkmm3::window::event_handlers
{

/**
 * @brief Handles the central canvas mouse events: the drawing of a rectangle,
 * resizing and the resize cursor feedback, the drawing of a segment polygon and
 * the dragging of its points.
 *
 * Every left mouse button click made while the Shift key is held appends a
 * point to the path of the segment being drawn, and the release of the Shift
 * key loops the path into the polygon (a path of less than three points is
 * dropped). A press over a point of a segment path selects the point and the
 * drag moves it. The Shift key release is caught by the main window, which
 * sees every key before its focused widget; the window losing the focus, a
 * click without the Shift key or a pointer motion without it finish the segment
 * too, as the release may happen out of the window.
 */
class MouseDrawEventsHandler : public AbstractWindowEventHandler,
                               virtual public ::helpers::TypeHelper
{
 public:
  explicit MouseDrawEventsHandler(std::shared_ptr<WindowOps> nops);

  bool subscribe() override;

 protected:
  bool handle_motion_start(GdkEventButton* event);
  bool handle_motion_end(GdkEventButton* event);
  bool handle_motion_event(GdkEventMotion* event);
  bool handle_key_release(GdkEventKey* event);
  bool handle_focus_out(GdkEventFocus* event);

  bool handle_rectangle_draw_start(GdkEventButton* event);
  bool handle_rectangle_draw_end(GdkEventButton* event);
  bool handle_rectangle_size_change(GdkEventMotion* event);

  bool handle_resize_motion_start(GdkEventButton* event);
  bool handle_resize_motion_event(GdkEventMotion* event);
  bool update_current_resize(GdkEventMotion* event);

  void normalize_current_rect();

  /// @brief Appends the canvas position to the path of the drawn segment, which
  /// starts first unless the current image has one being drawn
  bool handle_segment_point_add(const double& x, const double& y);
  /// @brief Starts a new segment of the current image
  bool start_segment();
  /// @brief Loops the path of the drawn segment into the polygon, or drops the
  /// path too short to loop
  void finish_segment();

  bool handle_point_drag_start();
  bool handle_point_drag(const double& x, const double& y);
  bool handle_point_drag_end();

  void set_resize_cursor();
  void set_point_cursor();
  void reset_cursor();

  std::function<bool(const int& x, const int& y, bool keyboard_tooltip,
                     const Glib::RefPtr<Gtk::Tooltip>& tooltip)>
  get_tooltip_lambda();
};

}  // namespace imagesAnnotatorGtkmm3::window::event_handlers

#endif  // IMAGES_ANNOTATOR_PROJECT_GTKMM_EVENTHANDLERS_MOUSEDRAWEVENTSHANDLER_CLASS_H
