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

#include "src/gtkmm3/main-window/event-handlers/ZoomEventsHandler.h"

#include <cassert>
#include <memory>

#include "src/gtkmm3/MainWindowContext.h"
#include "src/gtkmm3/main-window/event-handlers/WindowOps.h"
#include "src/log/log.h"

namespace templateGtkmm3::window::event_handlers
{

ZoomEventsHandler::ZoomEventsHandler(std::shared_ptr<WindowOps> nops)
    : AbstractWindowEventHandler(nops)
{
}

bool ZoomEventsHandler::subscribe()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);

  auto* zoomInB = mwctx->wloader->get_current_image_zoom_in();
  auto* zoomOutB = mwctx->wloader->get_current_image_zoom_out();

  zoomInB->signal_clicked().connect(
      sigc::mem_fun(*this, &ZoomEventsHandler::handle_zoom_in));
  zoomOutB->signal_clicked().connect(
      sigc::mem_fun(*this, &ZoomEventsHandler::handle_zoom_out));

  return true;
}

void ZoomEventsHandler::handle_zoom_in()
{
  assert(MainWindowContext::validate_context(mwctx));

  if (!MainWindowContext::validate_context(mwctx)) {
    LOGE("Invalid context pointer provided");
    return;
  }

  if (mwctx->current_image == nullptr) {
    LOGT("No image available");
    return;
  }

  assert(mwctx->current_irecord() != nullptr);

  mwctx->current_irecord()->scaleStepIn();

  ops->update_image_zoom();
  ops->update_statuses();
}

void ZoomEventsHandler::handle_zoom_out()
{
  assert(MainWindowContext::validate_context(mwctx));

  if (!MainWindowContext::validate_context(mwctx)) {
    LOGE("Invalid context pointer provided");
    return;
  }

  if (mwctx->current_image == nullptr) {
    LOGT("No image available");
    return;
  }

  assert(mwctx->current_irecord() != nullptr);

  mwctx->current_irecord()->scaleStepOut();

  ops->update_image_zoom();
  ops->update_statuses();
}

bool ZoomEventsHandler::handle_image_scroll(GdkEventScroll* scroll_event)
{
  assert(scroll_event != nullptr);

  if (scroll_event == nullptr) {
    LOGE("No valid event pointer provided");
    return false;
  }

  if (!(scroll_event->state & GDK_SHIFT_MASK)) {
    return false;
  }

  if (scroll_event->direction == GDK_SCROLL_UP) {
    handle_zoom_in();
  } else if (scroll_event->direction == GDK_SCROLL_DOWN) {
    handle_zoom_out();
  } else {
    LOGE("Unknown scroll event type");
    return false;
  }

  return true;
}

}  // namespace templateGtkmm3::window::event_handlers
