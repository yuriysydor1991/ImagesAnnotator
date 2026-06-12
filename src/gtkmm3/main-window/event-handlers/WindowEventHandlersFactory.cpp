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

#include "src/gtkmm3/main-window/event-handlers/WindowEventHandlersFactory.h"

#include <memory>

#include "src/gtkmm3/main-window/event-handlers/AboutEventsHandler.h"
#include "src/gtkmm3/main-window/event-handlers/AnnotationsEventsHandler.h"
#include "src/gtkmm3/main-window/event-handlers/ExportEventsHandler.h"
#include "src/gtkmm3/main-window/event-handlers/ImageListEventsHandler.h"
#include "src/gtkmm3/main-window/event-handlers/ImageNavigationEventsHandler.h"
#include "src/gtkmm3/main-window/event-handlers/KeyboardEventsHandler.h"
#include "src/gtkmm3/main-window/event-handlers/MouseDrawEventsHandler.h"
#include "src/gtkmm3/main-window/event-handlers/ProjectFileEventsHandler.h"
#include "src/gtkmm3/main-window/event-handlers/WebPageEventsHandler.h"
#include "src/gtkmm3/main-window/event-handlers/WindowSystemEventsHandler.h"
#include "src/gtkmm3/main-window/event-handlers/ZoomEventsHandler.h"

namespace templateGtkmm3::window::event_handlers
{

std::shared_ptr<MouseDrawEventsHandler>
WindowEventHandlersFactory::create_mouse_events_handler(WindowOpsPtr ops)
{
  return std::make_shared<MouseDrawEventsHandler>(ops);
}

std::shared_ptr<KeyboardEventsHandler>
WindowEventHandlersFactory::create_keyboard_events_handler(WindowOpsPtr ops)
{
  return std::make_shared<KeyboardEventsHandler>(ops);
}

std::shared_ptr<ZoomEventsHandler>
WindowEventHandlersFactory::create_zoom_events_handler(WindowOpsPtr ops)
{
  return std::make_shared<ZoomEventsHandler>(ops);
}

std::shared_ptr<ImageNavigationEventsHandler>
WindowEventHandlersFactory::create_image_navigation_events_handler(
    WindowOpsPtr ops)
{
  return std::make_shared<ImageNavigationEventsHandler>(ops);
}

std::shared_ptr<ImageListEventsHandler>
WindowEventHandlersFactory::create_image_list_events_handler(WindowOpsPtr ops)
{
  return std::make_shared<ImageListEventsHandler>(ops);
}

std::shared_ptr<AnnotationsEventsHandler>
WindowEventHandlersFactory::create_annotations_events_handler(WindowOpsPtr ops)
{
  return std::make_shared<AnnotationsEventsHandler>(ops);
}

std::shared_ptr<ProjectFileEventsHandler>
WindowEventHandlersFactory::create_project_file_events_handler(WindowOpsPtr ops)
{
  return std::make_shared<ProjectFileEventsHandler>(ops);
}

std::shared_ptr<ExportEventsHandler>
WindowEventHandlersFactory::create_export_events_handler(WindowOpsPtr ops)
{
  return std::make_shared<ExportEventsHandler>(ops);
}

std::shared_ptr<WebPageEventsHandler>
WindowEventHandlersFactory::create_web_page_events_handler(WindowOpsPtr ops)
{
  return std::make_shared<WebPageEventsHandler>(ops);
}

std::shared_ptr<AboutEventsHandler>
WindowEventHandlersFactory::create_about_events_handler(WindowOpsPtr ops)
{
  return std::make_shared<AboutEventsHandler>(ops);
}

std::shared_ptr<WindowSystemEventsHandler>
WindowEventHandlersFactory::create_window_system_events_handler(WindowOpsPtr ops)
{
  return std::make_shared<WindowSystemEventsHandler>(ops);
}

}  // namespace templateGtkmm3::window::event_handlers
