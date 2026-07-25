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

#include "src/gui/wxwidgets/WxWidgetsIniter.h"

#include <cassert>

#include "src/gui/wxwidgets/MainWindowContext.h"
#include "src/gui/wxwidgets/main-window/WindowEventsHandler.h"
#include "src/gui/wxwidgets/main-window/WindowLoader.h"
#include "src/gui/wxwidgets/wx_includes.h"
#include "src/log/log.h"

namespace templateWx
{

namespace
{
// Minimal wxApp shim. The application lifetime is driven manually from
// WxWidgetsIniter::run() - mirroring how the GtkmmIniter owns the
// Gtk::Application - so OnInit only has to succeed; the top window is created
// by prepare_widgets() through the WindowLoader.
class WxAppShim : public wxApp
{
 public:
  bool OnInit() override { return true; }
};
}  // namespace

WxWidgetsIniter::WxWidgetsIniter()
    : WxWidgetsIniter(MainWindowContext::build_context())
{
  assert(mwctx != nullptr);
  assert(mwctx->wctx != nullptr);
  assert(mwctx->wloader != nullptr);
  assert(mwctx->cwFactory != nullptr);
}

WxWidgetsIniter::WxWidgetsIniter(std::shared_ptr<MainWindowContext> nmwctx)
    : mwctx{nmwctx}
{
  assert(mwctx != nullptr);
  assert(mwctx->wctx != nullptr);
  assert(mwctx->wloader != nullptr);
  assert(mwctx->cwFactory != nullptr);
}

bool WxWidgetsIniter::run(std::shared_ptr<app::ApplicationContext> ctx)
{
  assert(ctx != nullptr);
  assert(mwctx->wctx != nullptr);
  assert(mwctx->wloader != nullptr);

  if (ctx == nullptr) {
    LOGE("No valid application context provided");
    return false;
  }

  mwctx->actx = ctx;

  // Hand the application object to wxWidgets and start its subsystems against
  // the process argv (mirrors Gtk::Application::create()).
  wxApp::SetInstance(new WxAppShim());

  if (!wxEntryStart(ctx->argc, ctx->argv)) {
    LOGE("Failure to start the wxWidgets application subsystem");
    return false;
  }

  // Pin the application / class name to the desktop application id so the
  // running window is matched to its installed .desktop launcher (the wx
  // analogue of GtkmmIniter's Glib::set_prgname()).
  const wxString appId = wxString::FromUTF8(ctx->app_dbus_url.c_str());
  wxTheApp->SetAppName(appId);
  wxTheApp->SetClassName(appId);

  wxInitAllImageHandlers();

  if (!prepare_widgets()) {
    wxEntryCleanup();
    return false;
  }

  if (!mwctx->wloader->all_widget_are_valid()) {
    LOGE("Some of the widgets were not found");
    wxEntryCleanup();
    return false;
  }

  auto* window = mwctx->wloader->get_window();
  window->Show();
  wxTheApp->SetTopWindow(window);

  if (!mwctx->mweHandler->init(mwctx)) {
    LOGE("Failure to initialize the main window event handler instance");
    wxEntryCleanup();
    return false;
  }

  const int rt = wxTheApp->OnRun();

  wxTheApp->OnExit();
  wxEntryCleanup();

  return rt == 0;
}

bool WxWidgetsIniter::prepare_widgets()
{
  assert(mwctx != nullptr);
  assert(mwctx->wctx != nullptr);
  assert(mwctx->wloader != nullptr);

  if (mwctx->wloader == nullptr) {
    LOGE("No loader available");
    return false;
  }

  if (!mwctx->wloader->load_window(mwctx)) {
    LOGE("Failure during the window load");
    return false;
  }

  return mwctx->wloader->get_window() != nullptr;
}

void WxWidgetsIniter::deinit()
{
  if (mwctx != nullptr) {
    mwctx->clear();
  }
}

}  // namespace templateWx
