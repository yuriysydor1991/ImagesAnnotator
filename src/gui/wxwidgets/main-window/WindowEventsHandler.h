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

#ifndef IMAGES_ANNOTATOR_PROJECT_WX_WINDOWEVENTSHANDLER_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_WX_WINDOWEVENTSHANDLER_CLASS_H

#include <memory>
#include <vector>

#include "src/annotator-events/events/ImagesDirProviderChangedHandler.h"
#include "src/gui/wxwidgets/ComponentTypesAliases.h"

namespace imagesAnnotatorWx
{
class MainWindowContext;
}  // namespace imagesAnnotatorWx

namespace imagesAnnotatorWx::window::event_handlers
{
class WindowOps;
class WindowEventHandlersFactory;
class AbstractWindowEventHandler;
class WindowSystemEventsHandler;
}  // namespace imagesAnnotatorWx::window::event_handlers

namespace imagesAnnotatorWx::window
{

/**
 * @brief Coordinates the decomposed main window event handlers - the wxWidgets
 * analogue of imagesAnnotatorGtkmm3's WindowEventsHandler. It builds the shared
 * WindowOps, uses the WindowEventHandlersFactory to create every category
 * handler, asks each to bind its wx events, and forwards the application
 * controller (system) events to the system events handler.
 */
class WindowEventsHandler
    : virtual public ComponentTypesAliases,
      virtual public events::events::ImagesDirProviderChangedHandler,
      public std::enable_shared_from_this<WindowEventsHandler>,
      virtual public events::events::DisplayErrorEventHandler
{
 public:
  using ImagesDirProviderChanged = events::events::ImagesDirProviderChanged;

  virtual ~WindowEventsHandler() = default;
  WindowEventsHandler() = default;

  virtual bool init(std::shared_ptr<MainWindowContext> nmwctx);

 protected:
  virtual void subscribe_4_visual_events();
  virtual void subscribe_4_system_events();

  virtual void build_event_handlers();

  virtual void handle(std::shared_ptr<ImagesDirProviderChanged> event) override;
  virtual void handle(DisplayErrorEventPtr event) override;

 private:
  std::shared_ptr<MainWindowContext> mwctx;
  std::shared_ptr<event_handlers::WindowOps> ops;
  std::shared_ptr<event_handlers::WindowEventHandlersFactory> ehFactory;
  std::vector<std::shared_ptr<event_handlers::AbstractWindowEventHandler>>
      handlers;
  std::shared_ptr<event_handlers::WindowSystemEventsHandler> systemHandler;
};

}  // namespace imagesAnnotatorWx::window

#endif  // IMAGES_ANNOTATOR_PROJECT_WX_WINDOWEVENTSHANDLER_CLASS_H
