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

#include "src/gtkmm3/main-window/event-handlers/WindowSystemEventsHandler.h"

#include <cassert>
#include <memory>

#include "src/gtkmm3/MainWindowContext.h"
#include "src/gtkmm3/main-window/event-handlers/WindowOps.h"
#include "src/log/log.h"

namespace templateGtkmm3::window::event_handlers
{

WindowSystemEventsHandler::WindowSystemEventsHandler(
    std::shared_ptr<WindowOps> nops)
    : AbstractWindowEventHandler(nops)
{
}

bool WindowSystemEventsHandler::subscribe()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);
  assert(mwctx->wloader->get_window() != nullptr);

  mwctx->wloader->get_window()->signal_delete_event().connect(
      sigc::mem_fun(*this, &WindowSystemEventsHandler::handle_window_close),
      false);

  return true;
}

bool WindowSystemEventsHandler::handle_window_close(GdkEventAny*)
{
  if (!ops->lastChangedStatus) {
    LOGT("Seems like no changes made - allowing window close");
    return false;
  }

  const bool continueWClose = ops->ask_about_unsaved_changes();

  if (continueWClose) {
    LOGT("user alowed window to close");
    return false;
  }

  LOGT("Interupting window close");
  return true;
}

void WindowSystemEventsHandler::handle_images_dir_provider_changed(
    std::shared_ptr<ImagesDirProviderChanged> event)
{
  LOGT("The images db provider have changed");

  assert(event != nullptr);
  assert(event->images_provider != nullptr);
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);
  assert(mwctx->wloader->get_images_list() != nullptr);
  assert(mwctx->cwFactory != nullptr);

  if (event == nullptr || event->images_provider == nullptr) {
    LOGE("No valid event object provided");
    return;
  }

  mwctx->images_provider = event->images_provider;

  ops->update_images_list();
  ops->update_annotations_list();
  ops->update_current_rects_list();
  ops->update_statuses();

  ops->hide_spinner();
}

void WindowSystemEventsHandler::handle_display_error(DisplayErrorEventPtr event)
{
  assert(event != nullptr);

  if (event == nullptr) {
    LOGE("Invalid event pointer provided");
    return;
  }

  LOGD("Handle error: " << event->description);

  ops->show_error_dialog(event->description);
}

}  // namespace templateGtkmm3::window::event_handlers
