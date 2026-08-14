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

#include "src/gui/wxwidgets/main-window/event-handlers/WebPageEventsHandler.h"

#include <cassert>

#include "src/gui/wxwidgets/MainWindowContext.h"
#include "src/gui/wxwidgets/main-window/WindowDataContext.h"
#include "src/gui/wxwidgets/main-window/event-handlers/WindowOps.h"
#include "src/log/log.h"

namespace imagesAnnotatorWx::window::event_handlers
{

WebPageEventsHandler::WebPageEventsHandler(std::shared_ptr<WindowOps> nops)
    : AbstractWindowEventHandler(nops)
{
}

bool WebPageEventsHandler::subscribe()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);

  mwctx->wloader->get_window()->Bind(
      wxEVT_MENU, [this](wxCommandEvent&) { handle_web_page_load(); },
      WindowDataContext::ID_IMAGES_WEB_PAGE_OPEN);

  return true;
}

void WebPageEventsHandler::handle_web_page_load()
{
  if (!MainWindowContext::validate_context(mwctx)) {
    LOGE("Invalid context pointer provided");
    return;
  }

  std::string webpageurl;

  if (!ask_user_4_webpage_url(webpageurl)) {
    LOGD("User closed the URL dialog");
    return;
  }

  if (webpageurl.empty()) {
    LOGE("No Web page URL retrieved");
    return;
  }

  auto efactory = mwctx->actx->eventer->get_events_factory();

  assert(efactory != nullptr);

  auto webURLEvent = efactory->create_images_web_page_load_event(webpageurl);

  assert(webURLEvent != nullptr);

  mwctx->actx->eventer->submit(webURLEvent);
}

bool WebPageEventsHandler::ask_user_4_webpage_url(std::string& urldst)
{
  if (!MainWindowContext::validate_context(mwctx)) {
    LOGE("Invalid context pointer provided");
    return false;
  }

  ops->show_spinner();

  const bool ok =
      mwctx->cwFactory->ask_webpage_url(mwctx->wloader->get_window(), urldst);

  ops->hide_spinner();

  if (!ok) {
    LOGD("Dialog is closed");
    return false;
  }

  LOGD("User entered web page: " << urldst);

  return !urldst.empty();
}

}  // namespace imagesAnnotatorWx::window::event_handlers
