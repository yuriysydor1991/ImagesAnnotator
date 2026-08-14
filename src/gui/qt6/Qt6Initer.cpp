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

#include "src/gui/qt6/Qt6Initer.h"

#include <cassert>

#include "project-global-decls.h"
#include "src/gui/qt6/MainWindowContext.h"
#include "src/gui/qt6/main-window/WindowEventsHandler.h"
#include "src/gui/qt6/main-window/WindowLoader.h"
#include "src/gui/qt6/qt_includes.h"
#include "src/gui/qt6/qt_str_utils.h"
#include "src/log/log.h"

namespace imagesAnnotatorQt6
{

Qt6Initer::Qt6Initer() : Qt6Initer(MainWindowContext::build_context())
{
  assert(mwctx != nullptr);
  assert(mwctx->wctx != nullptr);
  assert(mwctx->wloader != nullptr);
  assert(mwctx->cwFactory != nullptr);
}

Qt6Initer::Qt6Initer(std::shared_ptr<MainWindowContext> nmwctx) : mwctx{nmwctx}
{
  assert(mwctx != nullptr);
  assert(mwctx->wctx != nullptr);
  assert(mwctx->wloader != nullptr);
  assert(mwctx->cwFactory != nullptr);
}

bool Qt6Initer::run(std::shared_ptr<app::ApplicationContext> ctx)
{
  assert(ctx != nullptr);
  assert(mwctx->wctx != nullptr);
  assert(mwctx->wloader != nullptr);

  if (ctx == nullptr) {
    LOGE("No valid application context provided");
    return false;
  }

  mwctx->actx = ctx;

  // QApplication parses (and strips) the Qt specific command line options from
  // the very argc/argv the application was started with, mirroring what
  // Gtk::Application::create() does for the GTK ones. It is a stack object so
  // it outlives every widget created below and is destroyed only after exec()
  // has returned and the window is gone.
  QApplication app{ctx->argc, ctx->argv};

  // Pin the desktop file name to the application id. On Wayland Qt derives the
  // surface app_id from it (falling back to the binary name otherwise); the
  // compositor / GNOME Shell match that app_id against the installed
  // <app-id>.desktop to pick the taskbar/dash icon, so it has to equal the
  // application id (APPLICATION_ID) the .desktop is named after. This is the Qt
  // analogue of GtkmmIniter's Glib::set_prgname().
  const QString appId = u8(mwctx->actx->app_dbus_url);

  QGuiApplication::setDesktopFileName(appId);
  QCoreApplication::setApplicationName(u8(project_decls::PROJECT_NAME));
  QCoreApplication::setApplicationVersion(
      u8(project_decls::PROJECT_BUILD_VERSION));

  if (!prepare_widgets()) {
    return false;
  }

  if (!mwctx->wloader->all_widget_are_valid()) {
    LOGE("Some of the widgets were not found");
    return false;
  }

  if (!mwctx->mweHandler->init(mwctx)) {
    LOGE("Failure to initialize the main window event handler instance");
    return false;
  }

  const int rt = QApplication::exec();

  // Tear the widget tree down here rather than leaving it to deinit(): Qt
  // requires its application instance to still exist while widgets are
  // destroyed, and the application object above is a local of this function,
  // whereas deinit() is called by app::Application only after run() returned.
  // clear() is safe to run twice, so the deinit() that follows stays correct.
  deinit();

  return rt == 0;
}

bool Qt6Initer::prepare_widgets()
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

  assert(mwctx->wloader->get_window() != nullptr);
  assert(mwctx->wloader->all_widget_are_valid());

  if (!mwctx->wloader->all_widget_are_valid()) {
    LOGE("Window builder returned invalid status");
    return false;
  }

  return mwctx->wloader->get_window() != nullptr;
}

void Qt6Initer::deinit()
{
  if (mwctx != nullptr) {
    mwctx->clear();
  }
}

}  // namespace imagesAnnotatorQt6
