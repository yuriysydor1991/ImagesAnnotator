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

#ifndef IMAGES_ANNOTATOR_PROJECT_GTKMM4_EVENTHANDLERS_MOUSEDRAWEVENTSHANDLER_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_GTKMM4_EVENTHANDLERS_MOUSEDRAWEVENTSHANDLER_CLASS_H

#include <functional>
#include <memory>

#include "src/gui/gtkmm4/gtkmm_includes.h"
#include "src/gui/gtkmm4/main-window/event-handlers/AbstractWindowEventHandler.h"
#include "src/helpers/TypeHelper.h"

namespace templateGtkmm4::window::event_handlers
{

/**
 * @brief Handles the central canvas mouse events: rectangle drawing, resizing
 * and the resize cursor feedback.
 *
 * GTK4 delivers pointer input through event controllers instead of the removed
 * signal_button_press_event()/signal_motion_notify_event() signals, so the
 * handler attaches a Gtk::GestureClick (left button only) and a
 * Gtk::EventControllerMotion to the canvas. The former GdkEvent* arguments
 * become plain widget-relative double coordinates.
 */
class MouseDrawEventsHandler : public AbstractWindowEventHandler,
                               virtual public ::helpers::TypeHelper
{
 public:
  explicit MouseDrawEventsHandler(std::shared_ptr<WindowOps> nops);

  bool subscribe() override;

 protected:
  void handle_motion_start(int n_press, double x, double y);
  void handle_motion_end(int n_press, double x, double y);
  void handle_motion_event(double x, double y);

  bool handle_rectangle_draw_start(double x, double y);
  bool handle_rectangle_draw_end(double x, double y);
  bool handle_rectangle_size_change(double x, double y);

  bool handle_resize_motion_start(double x, double y);
  bool handle_resize_motion_event(double x, double y);
  bool update_current_resize(double x, double y);

  void normalize_current_rect();

  void set_resize_cursor();
  void reset_cursor();

  std::function<bool(int x, int y, bool keyboard_tooltip,
                     const Glib::RefPtr<Gtk::Tooltip>& tooltip)>
  get_tooltip_lambda();
};

}  // namespace templateGtkmm4::window::event_handlers

#endif  // IMAGES_ANNOTATOR_PROJECT_GTKMM4_EVENTHANDLERS_MOUSEDRAWEVENTSHANDLER_CLASS_H
