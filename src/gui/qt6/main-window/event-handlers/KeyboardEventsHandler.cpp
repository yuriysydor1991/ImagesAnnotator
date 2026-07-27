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

#include "src/gui/qt6/main-window/event-handlers/KeyboardEventsHandler.h"

#include <cassert>

#include "src/gui/qt6/MainWindowContext.h"
#include "src/gui/qt6/main-window/event-handlers/WindowOps.h"
#include "src/log/log.h"

namespace templateQt6::window::event_handlers
{

namespace
{
// Run a button's clicked() handlers, so Up / Down reuse the exact previous /
// next navigation the buttons run (the Qt analogue of GTK's button->clicked()).
// QAbstractButton::click() is a no-op on a disabled button, which matches the
// bounds the navigation buttons already express.
void click_button(QPushButton* button)
{
  if (button == nullptr) {
    return;
  }

  button->click();
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

  auto* window = mwctx->wloader->get_window();

  // A window-context QShortcut is Qt's route for a key that has to be seen
  // before the focused widget consumes it, which is exactly what the GTKmm
  // handler achieves by connecting to the window's key-press-event with
  // after=false. Both shortcuts are parented to the window, so they live and
  // die with it.
  auto* prevShortcut = new QShortcut{QKeySequence{Qt::Key_Up}, window};
  auto* nextShortcut = new QShortcut{QKeySequence{Qt::Key_Down}, window};

  prevShortcut->setContext(Qt::WindowShortcut);
  nextShortcut->setContext(Qt::WindowShortcut);

  QObject::connect(prevShortcut, &QShortcut::activated, window,
                   [this]() { handle_prev_key(); });
  QObject::connect(nextShortcut, &QShortcut::activated, window,
                   [this]() { handle_next_key(); });

  return true;
}

void KeyboardEventsHandler::handle_prev_key()
{
  LOGD("User pressed up key");

  click_button(mwctx->wloader->get_prev_file_button());
}

void KeyboardEventsHandler::handle_next_key()
{
  LOGD("User pressed down key");

  click_button(mwctx->wloader->get_next_file_button());
}

}  // namespace templateQt6::window::event_handlers
