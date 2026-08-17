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

#include "src/gui/wxwidgets/main-window/WindowEventsHandler.h"

#include <cassert>

#include "src/gui/wxwidgets/MainWindowContext.h"
#include "src/gui/wxwidgets/main-window/event-handlers/AboutEventsHandler.h"
#include "src/gui/wxwidgets/main-window/event-handlers/AbstractWindowEventHandler.h"
#include "src/gui/wxwidgets/main-window/event-handlers/AnnotationsEventsHandler.h"
#include "src/gui/wxwidgets/main-window/event-handlers/ExportEventsHandler.h"
#include "src/gui/wxwidgets/main-window/event-handlers/ImageListEventsHandler.h"
#include "src/gui/wxwidgets/main-window/event-handlers/ImageNavigationEventsHandler.h"
#include "src/gui/wxwidgets/main-window/event-handlers/ImportEventsHandler.h"
#include "src/gui/wxwidgets/main-window/event-handlers/KeyboardEventsHandler.h"
#include "src/gui/wxwidgets/main-window/event-handlers/MouseDrawEventsHandler.h"
#include "src/gui/wxwidgets/main-window/event-handlers/ProjectFileEventsHandler.h"
#include "src/gui/wxwidgets/main-window/event-handlers/WebPageEventsHandler.h"
#include "src/gui/wxwidgets/main-window/event-handlers/WindowEventHandlersFactory.h"
#include "src/gui/wxwidgets/main-window/event-handlers/WindowOps.h"
#include "src/gui/wxwidgets/main-window/event-handlers/WindowSystemEventsHandler.h"
#include "src/gui/wxwidgets/main-window/event-handlers/ZoomEventsHandler.h"
#include "src/log/log.h"

namespace imagesAnnotatorWx::window
{

bool WindowEventsHandler::init(std::shared_ptr<MainWindowContext> nmwctx)
{
  assert(nmwctx != nullptr);

  mwctx = nmwctx;

  if (!MainWindowContext::validate_context(mwctx)) {
    LOGE("Invalid main window context provided to the event handler");
    return false;
  }

  build_event_handlers();

  subscribe_4_system_events();
  subscribe_4_visual_events();

  // Ask the business layer for the initially loaded images/annotations provider
  // so the freshly built window shows whatever was passed on the command line.
  auto efactory = mwctx->actx->eventer->get_events_factory();
  auto idbRequest = efactory->create_image_dir_object_request();
  mwctx->actx->eventer->submit(idbRequest);

  return true;
}

void WindowEventsHandler::build_event_handlers()
{
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
  handlers.emplace_back(ehFactory->create_import_events_handler(ops));
  handlers.emplace_back(ehFactory->create_web_page_events_handler(ops));
  handlers.emplace_back(ehFactory->create_about_events_handler(ops));
  handlers.emplace_back(systemHandler);
}

void WindowEventsHandler::subscribe_4_visual_events()
{
  for (auto& handler : handlers) {
    handler->subscribe();
  }
}

void WindowEventsHandler::subscribe_4_system_events()
{
  auto myshared = shared_from_this();
  std::shared_ptr<events::events::ImagesDirProviderChangedHandler> myptr =
      myshared;

  mwctx->actx->eventer->subscribe(myptr);
  mwctx->actx->eventer->subscribe(DisplayErrorEventHandlerPtr{myshared});
}

void WindowEventsHandler::handle(
    std::shared_ptr<ImagesDirProviderChanged> event)
{
  systemHandler->handle_images_dir_provider_changed(event);
}

void WindowEventsHandler::handle(DisplayErrorEventPtr event)
{
  systemHandler->handle_display_error(event);
}

}  // namespace imagesAnnotatorWx::window
