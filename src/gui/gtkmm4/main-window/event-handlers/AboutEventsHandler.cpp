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

#include "src/gui/gtkmm4/main-window/event-handlers/AboutEventsHandler.h"

#include <cassert>
#include <memory>

#include "src/gui/gtkmm4/MainWindowContext.h"
#include "src/gui/gtkmm4/main-window/event-handlers/WindowOps.h"
#include "src/log/log.h"

namespace imagesAnnotatorGtkmm4::window::event_handlers
{

AboutEventsHandler::AboutEventsHandler(std::shared_ptr<WindowOps> nops)
    : AbstractWindowEventHandler(nops)
{
}

bool AboutEventsHandler::subscribe()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);

  // GTK4 menu entries are Gio::Actions; connect to the action activation.
  mwctx->wloader->get_about_action()->signal_activate().connect(
      [this]([[maybe_unused]] const Glib::VariantBase& param) {
        handle_about();
      });

  return true;
}

void AboutEventsHandler::handle_about()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader->get_about() != nullptr);
  assert(mwctx->cwFactory != nullptr);

  auto about = mwctx->wloader->get_about();

  mwctx->cwFactory->prepare_about(about, mwctx->wloader->get_window());

  about->present();
}

}  // namespace imagesAnnotatorGtkmm4::window::event_handlers
