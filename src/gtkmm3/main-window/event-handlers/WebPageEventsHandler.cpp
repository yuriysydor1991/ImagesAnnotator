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

#include "src/gtkmm3/main-window/event-handlers/WebPageEventsHandler.h"

#include <cassert>
#include <memory>
#include <string>

#include "src/gtkmm3/MainWindowContext.h"
#include "src/gtkmm3/main-window/event-handlers/WindowOps.h"
#include "src/log/log.h"

namespace templateGtkmm3::window::event_handlers
{

WebPageEventsHandler::WebPageEventsHandler(std::shared_ptr<WindowOps> nops)
    : AbstractWindowEventHandler(nops)
{
}

bool WebPageEventsHandler::subscribe()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);

  auto* webLoadM = mwctx->wloader->get_images_web_page_open_menu_item_mi();

  webLoadM->signal_activate().connect(
      sigc::mem_fun(*this, &WebPageEventsHandler::handle_web_page_load));

  return true;
}

void WebPageEventsHandler::handle_web_page_load()
{
  assert(MainWindowContext::validate_context(mwctx));

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

  assert(mwctx->actx->eventer != nullptr);

  auto efactory = mwctx->actx->eventer->get_events_factory();

  assert(efactory != nullptr);

  auto webURLEvent = efactory->create_images_web_page_load_event(webpageurl);

  assert(webURLEvent != nullptr);

  mwctx->actx->eventer->submit(webURLEvent);
}

bool WebPageEventsHandler::ask_user_4_webpage_url(std::string& urldst)
{
  assert(MainWindowContext::validate_context(mwctx));

  if (!MainWindowContext::validate_context(mwctx)) {
    LOGE("Invalid context pointer provided");
    return false;
  }

  ops->show_spinner();

  auto* dialog = mwctx->wloader->get_web_page_url_asker();

  assert(dialog != nullptr);

  mwctx->cwFactory->prepare_url_asker(
      dialog, mwctx->wloader->get_images_web_page_cache_folder(),
      mwctx->wloader->get_window());

  dialog->show_all();

  const int result = dialog->run();

  dialog->hide();

  if (result != Gtk::RESPONSE_OK) {
    LOGD("Dialog is closed");
    ops->hide_spinner();
    return false;
  }

  auto urlEntry = mwctx->wloader->get_images_web_page_url_entry();

  assert(urlEntry != nullptr);

  urldst = urlEntry->get_text();

  ops->hide_spinner();

  LOGD("User entered web page: " << urldst);

  return !urldst.empty();
}

}  // namespace templateGtkmm3::window::event_handlers
