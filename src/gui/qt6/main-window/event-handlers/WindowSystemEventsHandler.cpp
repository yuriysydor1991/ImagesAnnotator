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

#include "src/gui/qt6/main-window/event-handlers/WindowSystemEventsHandler.h"

#include <cassert>
#include <functional>

#include "src/gui/qt6/MainWindowContext.h"
#include "src/gui/qt6/main-window/event-handlers/WindowOps.h"
#include "src/gui/qt6/qt_includes.h"
#include "src/log/log.h"

namespace imagesAnnotatorQt6::window::event_handlers
{

namespace
{
/**
 * @brief Turns the window's QCloseEvent into the handler's veto decision.
 *
 * Qt reports a close request by delivering a QCloseEvent to the window rather
 * than by emitting a signal, so - unlike every other event this component
 * handles - it cannot be reached with QObject::connect(). A filter is the
 * documented route; it is a plain QObject (no signals or slots of its own, so
 * no Q_OBJECT / moc needed) parented to the window, which therefore destroys
 * it.
 */
class WindowCloseFilter : public QObject
{
 public:
  WindowCloseFilter(QObject* parent, std::function<bool()> nveto)
      : QObject{parent}, veto{std::move(nveto)}
  {
  }

 protected:
  bool eventFilter(QObject* watched, QEvent* event) override
  {
    if (event->type() == QEvent::Close && veto && veto()) {
      // Consuming the event here is Qt's equivalent of the wx Veto(): the
      // window stays open and nothing else sees the request.
      event->ignore();
      return true;
    }

    return QObject::eventFilter(watched, event);
  }

 private:
  std::function<bool()> veto;
};
}  // namespace

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

  auto* window = mwctx->wloader->get_window();

  window->installEventFilter(
      new WindowCloseFilter{window, [this]() { return veto_window_close(); }});

  return true;
}

bool WindowSystemEventsHandler::veto_window_close()
{
  if (!ops->lastChangedStatus) {
    LOGT("Seems like no changes made - allowing window close");
    return false;
  }

  if (ops->ask_about_unsaved_changes()) {
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

}  // namespace imagesAnnotatorQt6::window::event_handlers
