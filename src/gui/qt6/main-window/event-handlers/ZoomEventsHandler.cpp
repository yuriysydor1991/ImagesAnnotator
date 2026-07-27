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

#include "src/gui/qt6/main-window/event-handlers/ZoomEventsHandler.h"

#include <cassert>

#include "src/gui/qt6/MainWindowContext.h"
#include "src/gui/qt6/main-window/event-handlers/WindowOps.h"
#include "src/log/log.h"

namespace templateQt6::window::event_handlers
{

ZoomEventsHandler::ZoomEventsHandler(std::shared_ptr<WindowOps> nops)
    : AbstractWindowEventHandler(nops)
{
}

bool ZoomEventsHandler::subscribe()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);
  assert(mwctx->centralCanvas != nullptr);

  auto* window = mwctx->wloader->get_window();

  QObject::connect(mwctx->wloader->get_current_image_zoom_in(),
                   &QPushButton::clicked, window,
                   [this]() { handle_zoom_in(); });
  QObject::connect(mwctx->wloader->get_current_image_zoom_out(),
                   &QPushButton::clicked, window,
                   [this]() { handle_zoom_out(); });

  // Shift + mouse wheel over the image zooms it in/out; the canvas only emits
  // this signal for a Shift-modified wheel and otherwise leaves the event to
  // the enclosing QScrollArea, which scrolls as usual.
  QObject::connect(mwctx->centralCanvas,
                   &custom_widgets::CentralWorkingCanvas::zoomStepRequested,
                   mwctx->centralCanvas,
                   [this](int steps) { handle_image_scroll(steps); });

  return true;
}

void ZoomEventsHandler::handle_zoom_in()
{
  if (!MainWindowContext::validate_context(mwctx)) {
    LOGE("Invalid context pointer provided");
    return;
  }

  if (mwctx->current_image == nullptr) {
    LOGT("No image available");
    return;
  }

  mwctx->current_irecord()->scaleStepIn();

  ops->update_image_zoom();
  ops->update_statuses();
}

void ZoomEventsHandler::handle_zoom_out()
{
  if (!MainWindowContext::validate_context(mwctx)) {
    LOGE("Invalid context pointer provided");
    return;
  }

  if (mwctx->current_image == nullptr) {
    LOGT("No image available");
    return;
  }

  mwctx->current_irecord()->scaleStepOut();

  ops->update_image_zoom();
  ops->update_statuses();
}

void ZoomEventsHandler::handle_image_scroll(int steps)
{
  if (steps > 0) {
    handle_zoom_in();
  } else if (steps < 0) {
    handle_zoom_out();
  }
}

}  // namespace templateQt6::window::event_handlers
