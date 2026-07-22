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

#include "src/gui/gtkmm4/main-window/event-handlers/KeyboardEventsHandler.h"

#include <cassert>
#include <memory>

#include "src/gui/gtkmm4/MainWindowContext.h"
#include "src/gui/gtkmm4/main-window/event-handlers/WindowOps.h"
#include "src/log/log.h"

namespace templateGtkmm4::window::event_handlers
{

KeyboardEventsHandler::KeyboardEventsHandler(std::shared_ptr<WindowOps> nops)
    : AbstractWindowEventHandler(nops)
{
}

bool KeyboardEventsHandler::subscribe()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);
  assert(mwctx->wloader->get_window() != nullptr);

  // GTK4 keyboard input is delivered through an event controller attached to
  // the window instead of the removed signal_key_press_event()/event masks.
  auto keyController = Gtk::EventControllerKey::create();
  keyController->signal_key_pressed().connect(
      sigc::mem_fun(*this, &KeyboardEventsHandler::handle_key_press), false);
  mwctx->wloader->get_window()->add_controller(keyController);

  return true;
}

bool KeyboardEventsHandler::handle_key_press(
    guint keyval, [[maybe_unused]] guint keycode,
    [[maybe_unused]] Gdk::ModifierType state)
{
  if (keyval == GDK_KEY_Up) {
    LOGD("User pressed up key");
    // GTK4 removed Gtk::Button::clicked(); Widget::activate() triggers it.
    mwctx->wloader->get_prev_file_button()->activate();
    return true;  // handled
  }

  if (keyval == GDK_KEY_Down) {
    LOGD("User pressed down key");
    mwctx->wloader->get_next_file_button()->activate();
    return true;  // handled
  }

  return false;  // let other handlers process it
}

}  // namespace templateGtkmm4::window::event_handlers
