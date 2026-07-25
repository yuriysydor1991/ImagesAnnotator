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

#include "src/gui/wxwidgets/main-window/event-handlers/KeyboardEventsHandler.h"

#include <cassert>

#include "src/gui/wxwidgets/MainWindowContext.h"
#include "src/gui/wxwidgets/main-window/event-handlers/WindowOps.h"
#include "src/log/log.h"

namespace templateWx::window::event_handlers
{

namespace
{
// Fire a button's wxEVT_BUTTON handler, so Up / Down reuse the exact previous /
// next navigation the buttons run (the wx analogue of GTK's button->clicked()).
void click_button(wxButton* button)
{
  if (button == nullptr || !button->IsThisEnabled()) {
    return;
  }

  wxCommandEvent event(wxEVT_BUTTON, button->GetId());
  event.SetEventObject(button);
  button->GetEventHandler()->ProcessEvent(event);
}
}  // namespace

KeyboardEventsHandler::KeyboardEventsHandler(std::shared_ptr<WindowOps> nops)
    : AbstractWindowEventHandler(nops)
{
}

bool KeyboardEventsHandler::subscribe()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);
  assert(mwctx->wloader->get_window() != nullptr);

  mwctx->wloader->get_window()->Bind(
      wxEVT_CHAR_HOOK, &KeyboardEventsHandler::handle_key_press, this);

  return true;
}

void KeyboardEventsHandler::handle_key_press(wxKeyEvent& event)
{
  switch (event.GetKeyCode()) {
    case WXK_UP:
      LOGD("User pressed up key");
      click_button(mwctx->wloader->get_prev_file_button());
      return;  // handled - consumed so the list does not also move
    case WXK_DOWN:
      LOGD("User pressed down key");
      click_button(mwctx->wloader->get_next_file_button());
      return;  // handled
    default:
      event.Skip();  // let other handlers process it
  }
}

}  // namespace templateWx::window::event_handlers
