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

#ifndef IMAGES_ANNOTATOR_PROJECT_GTKMM_EVENTHANDLERS_WINDOWEVENTHANDLERSFACTORY_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_GTKMM_EVENTHANDLERS_WINDOWEVENTHANDLERSFACTORY_CLASS_H

#include <memory>

namespace templateGtkmm3::window::event_handlers
{

class WindowOps;
class MouseDrawEventsHandler;
class KeyboardEventsHandler;
class ZoomEventsHandler;
class ImageNavigationEventsHandler;
class ImageListEventsHandler;
class AnnotationsEventsHandler;
class ProjectFileEventsHandler;
class ExportEventsHandler;
class WebPageEventsHandler;
class AboutEventsHandler;
class WindowSystemEventsHandler;

/**
 * @brief Factory creating each decomposed window event handler.
 *
 * Every produced handler receives the shared infrastructure object (WindowOps)
 * through its constructor.
 */
class WindowEventHandlersFactory
{
 public:
  using WindowOpsPtr = std::shared_ptr<WindowOps>;

  virtual ~WindowEventHandlersFactory() = default;
  WindowEventHandlersFactory() = default;

  virtual std::shared_ptr<MouseDrawEventsHandler> create_mouse_events_handler(
      WindowOpsPtr ops);
  virtual std::shared_ptr<KeyboardEventsHandler> create_keyboard_events_handler(
      WindowOpsPtr ops);
  virtual std::shared_ptr<ZoomEventsHandler> create_zoom_events_handler(
      WindowOpsPtr ops);
  virtual std::shared_ptr<ImageNavigationEventsHandler>
  create_image_navigation_events_handler(WindowOpsPtr ops);
  virtual std::shared_ptr<ImageListEventsHandler>
  create_image_list_events_handler(WindowOpsPtr ops);
  virtual std::shared_ptr<AnnotationsEventsHandler>
  create_annotations_events_handler(WindowOpsPtr ops);
  virtual std::shared_ptr<ProjectFileEventsHandler>
  create_project_file_events_handler(WindowOpsPtr ops);
  virtual std::shared_ptr<ExportEventsHandler> create_export_events_handler(
      WindowOpsPtr ops);
  virtual std::shared_ptr<WebPageEventsHandler> create_web_page_events_handler(
      WindowOpsPtr ops);
  virtual std::shared_ptr<AboutEventsHandler> create_about_events_handler(
      WindowOpsPtr ops);
  virtual std::shared_ptr<WindowSystemEventsHandler>
  create_window_system_events_handler(WindowOpsPtr ops);
};

}  // namespace templateGtkmm3::window::event_handlers

#endif  // IMAGES_ANNOTATOR_PROJECT_GTKMM_EVENTHANDLERS_WINDOWEVENTHANDLERSFACTORY_CLASS_H
