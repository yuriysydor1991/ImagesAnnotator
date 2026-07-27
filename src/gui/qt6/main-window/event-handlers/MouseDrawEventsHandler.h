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

#ifndef IMAGES_ANNOTATOR_PROJECT_QT6_EVENTHANDLERS_MOUSEDRAWEVENTSHANDLER_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_QT6_EVENTHANDLERS_MOUSEDRAWEVENTSHANDLER_CLASS_H

#include <memory>
#include <string>

#include "src/gui/qt6/main-window/event-handlers/AbstractWindowEventHandler.h"
#include "src/gui/qt6/qt_includes.h"
#include "src/helpers/TypeHelper.h"

namespace templateQt6::window::event_handlers
{

/**
 * @brief Handles the canvas mouse events: rectangle drawing, resizing and the
 * resize cursor feedback - the Qt analogue of GTKmm's MouseDrawEventsHandler.
 *
 * The coordinates arrive already in image space: the canvas widget is sized to
 * the scaled image inside the window's QScrollArea, so its widget coordinates
 * are the drawing-area coordinates the GTKmm original works in and no viewport
 * translation is needed. Qt also grabs the mouse implicitly for the widget a
 * button was pressed on, so - unlike the wxWidgets port - there is no explicit
 * capture to take or release.
 */
class MouseDrawEventsHandler : public AbstractWindowEventHandler,
                               virtual public ::helpers::TypeHelper
{
 public:
  explicit MouseDrawEventsHandler(std::shared_ptr<WindowOps> nops);

  bool subscribe() override;

 protected:
  void handle_motion_start(int x, int y);
  void handle_motion_end();
  void handle_motion_event(int x, int y);

  bool handle_rectangle_draw_start(int x, int y);
  void handle_rectangle_draw_end();
  void handle_rectangle_size_change(int x, int y);

  void handle_resize_motion_start();
  void handle_resize_motion_event(int x, int y);
  bool update_current_resize(int x, int y);

  void normalize_current_rect();

  void set_resize_cursor();
  void reset_cursor();

  void update_tooltip();

 private:
  std::string lastTooltip;
};

}  // namespace templateQt6::window::event_handlers

#endif  // IMAGES_ANNOTATOR_PROJECT_QT6_EVENTHANDLERS_MOUSEDRAWEVENTSHANDLER_CLASS_H
