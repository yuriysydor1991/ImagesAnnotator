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

#include "src/gtkmm3/main-window/WindowEventsHandler.h"

#include <cassert>
#include <memory>

#include "src/gtkmm3/MainWindowContext.h"
#include "src/gtkmm3/main-window/event-handlers/AbstractWindowEventHandler.h"
#include "src/gtkmm3/main-window/event-handlers/AboutEventsHandler.h"
#include "src/gtkmm3/main-window/event-handlers/AnnotationsEventsHandler.h"
#include "src/gtkmm3/main-window/event-handlers/ExportEventsHandler.h"
#include "src/gtkmm3/main-window/event-handlers/ImageListEventsHandler.h"
#include "src/gtkmm3/main-window/event-handlers/ImageNavigationEventsHandler.h"
#include "src/gtkmm3/main-window/event-handlers/KeyboardEventsHandler.h"
#include "src/gtkmm3/main-window/event-handlers/MouseDrawEventsHandler.h"
#include "src/gtkmm3/main-window/event-handlers/ProjectFileEventsHandler.h"
#include "src/gtkmm3/main-window/event-handlers/WebPageEventsHandler.h"
#include "src/gtkmm3/main-window/event-handlers/WindowEventHandlersFactory.h"
#include "src/gtkmm3/main-window/event-handlers/WindowOps.h"
#include "src/gtkmm3/main-window/event-handlers/WindowSystemEventsHandler.h"
#include "src/gtkmm3/main-window/event-handlers/ZoomEventsHandler.h"
#include "src/log/log.h"

namespace templateGtkmm3::window
{

bool WindowEventsHandler::init(std::shared_ptr<MainWindowContext> nmwctx)
{
  assert(MainWindowContext::validate_context(nmwctx));

  if (!MainWindowContext::validate_context(nmwctx)) {
    LOGE("Invalid context pointer provided");
    return false;
  }

  mwctx = nmwctx;

  build_event_handlers();

  subscribe_4_visual_events();
  subscribe_4_system_events();

  return true;
}

void WindowEventsHandler::build_event_handlers()
{
  assert(mwctx != nullptr);

  ops = std::make_shared<event_handlers::WindowOps>(mwctx);
  ehFactory = std::make_shared<event_handlers::WindowEventHandlersFactory>();

  systemHandler = ehFactory->create_window_system_events_handler(ops);

  handlers.clear();
  handlers.emplace_back(ehFactory->create_mouse_events_handler(ops));
  handlers.emplace_back(ehFactory->create_keyboard_events_handler(ops));
  handlers.emplace_back(ehFactory->create_zoom_events_handler(ops));
  handlers.emplace_back(ehFactory->create_image_navigation_events_handler(ops));
  handlers.emplace_back(ehFactory->create_image_list_events_handler(ops));
  handlers.emplace_back(ehFactory->create_annotations_events_handler(ops));
  handlers.emplace_back(ehFactory->create_project_file_events_handler(ops));
  handlers.emplace_back(ehFactory->create_export_events_handler(ops));
  handlers.emplace_back(ehFactory->create_web_page_events_handler(ops));
  handlers.emplace_back(ehFactory->create_about_events_handler(ops));
  handlers.emplace_back(systemHandler);
}

void WindowEventsHandler::subscribe_4_system_events()
{
  auto myshared = shared_from_this();
  std::shared_ptr<ImagesDirProviderChangedHandler> myptr = myshared;

  mwctx->actx->eventer->subscribe(myptr);
  mwctx->actx->eventer->subscribe(DisplayErrorEventHandlerPtr{myshared});

  /**
   * Sending requests for the initial objects.
   */
  auto efactory = mwctx->actx->eventer->get_events_factory();

  auto idbRequest = efactory->create_image_dir_object_request();

  mwctx->actx->eventer->submit(idbRequest);
}

void WindowEventsHandler::subscribe_4_visual_events()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);
  assert(mwctx->wloader->all_widget_are_valid());

  for (auto& handler : handlers) {
    assert(handler != nullptr);
    handler->subscribe();
  }
}

void WindowEventsHandler::handle(
    std::shared_ptr<ImagesDirProviderChanged> event)
{
  assert(systemHandler != nullptr);

  systemHandler->handle_images_dir_provider_changed(event);
}

void WindowEventsHandler::handle(DisplayErrorEventPtr event)
{
  assert(systemHandler != nullptr);

  systemHandler->handle_display_error(event);
}

}  // namespace templateGtkmm3::window
