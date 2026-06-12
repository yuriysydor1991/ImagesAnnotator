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

#include "src/gtkmm3/gtkmm_includes.h"
#include "src/gtkmm3/main-window/event-handlers/AbstractWindowEventHandler.h"
#include "src/helpers/TypeHelper.h"

namespace templateGtkmm3::window::event_handlers
{

/**
 * @brief Handles the central canvas mouse events: rectangle drawing, resizing
 * and the resize cursor feedback.
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

  bool handle_rectangle_draw_start(GdkEventButton* event);
  bool handle_rectangle_draw_end(GdkEventButton* event);
  bool handle_rectangle_size_change(GdkEventMotion* event);

  bool handle_resize_motion_start(GdkEventButton* event);
  bool handle_resize_motion_event(GdkEventMotion* event);
  bool update_current_resize(GdkEventMotion* event);

  void normalize_current_rect();

  void set_resize_cursor();
  void reset_cursor();

  std::function<bool(const int& x, const int& y, bool keyboard_tooltip,
                     const Glib::RefPtr<Gtk::Tooltip>& tooltip)>
  get_tooltip_lambda();
};

}  // namespace templateGtkmm3::window::event_handlers

#endif  // IMAGES_ANNOTATOR_PROJECT_GTKMM_EVENTHANDLERS_MOUSEDRAWEVENTSHANDLER_CLASS_H
