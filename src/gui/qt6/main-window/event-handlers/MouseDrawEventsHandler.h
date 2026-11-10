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

namespace imagesAnnotatorQt6::window::event_handlers
{

/**
 * @brief Handles the canvas mouse events: the drawing of a rectangle, resizing
 * and the resize cursor feedback, the drawing of a segment polygon and the
 * dragging of its points - the Qt analogue of GTKmm's MouseDrawEventsHandler.
 *
 * Every left mouse button click made while the Shift key is held appends a
 * point to the path of the segment being drawn, and the release of the Shift
 * key loops the path into the polygon (a path of less than three points is
 * dropped). A press over a point of a segment path selects the point and the
 * drag moves it. Qt delivers a key to the focused widget only, so the Shift
 * key release is caught by an event filter of the whole application; the
 * application turning inactive, a click without the Shift key or a pointer
 * motion without it finish the segment too, as the release may happen out of
 * the application.
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
  void handle_motion_start(int x, int y, bool shift);
  void handle_motion_end();
  void handle_motion_event(int x, int y, bool shift);

  bool handle_rectangle_draw_start(int x, int y);
  void handle_rectangle_draw_end();
  void handle_rectangle_size_change(int x, int y);

  void handle_resize_motion_start();
  void handle_resize_motion_event(int x, int y);
  bool update_current_resize(int x, int y);

  void normalize_current_rect();

  /// @brief Appends the canvas position to the path of the drawn segment, which
  /// starts first unless the current image has one being drawn
  bool handle_segment_point_add(int x, int y);
  /// @brief Starts a new segment of the current image
  bool start_segment();
  /// @brief Loops the path of the drawn segment into the polygon, or drops the
  /// path too short to loop
  void finish_segment();

  void handle_point_drag_start();
  void handle_point_drag(int x, int y);
  void handle_point_drag_end();

  void set_resize_cursor();
  void set_point_cursor();
  void reset_cursor();

  void update_tooltip();

 private:
  std::string lastTooltip;
};

}  // namespace imagesAnnotatorQt6::window::event_handlers

#endif  // IMAGES_ANNOTATOR_PROJECT_QT6_EVENTHANDLERS_MOUSEDRAWEVENTSHANDLER_CLASS_H
