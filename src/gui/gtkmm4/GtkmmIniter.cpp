/**
 * The "Simplified BSD License"
 *
 * Copyright (c) 2025, Yurii Sydor (yuriysydor1991@gmail.com)
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

#include "src/gui/gtkmm4/GtkmmIniter.h"

#include <cassert>

#include <glibmm/miscutils.h>

#include "project-global-decls.h"
#include "src/log/log.h"

namespace imagesAnnotatorGtkmm4
{

GtkmmIniter::GtkmmIniter() : GtkmmIniter(MainWindowContext::build_context())
{
  assert(mwctx != nullptr);
  assert(mwctx->wctx != nullptr);
  assert(mwctx->wloader != nullptr);
  assert(mwctx->cwFactory != nullptr);
}

GtkmmIniter::GtkmmIniter(std::shared_ptr<MainWindowContext> nmwctx)
    : mwctx{nmwctx}
{
  assert(mwctx != nullptr);
  assert(mwctx->wctx != nullptr);
  assert(mwctx->wloader != nullptr);
  assert(mwctx->cwFactory != nullptr);
}

bool GtkmmIniter::run(std::shared_ptr<app::ApplicationContext> ctx)
{
  assert(ctx != nullptr);
  assert(mwctx->wctx != nullptr);
  assert(mwctx->wloader != nullptr);

  if (ctx == nullptr) {
    return false;
  }

  mwctx->actx = ctx;

  // Pin the program name to the application id BEFORE the application (and thus
  // the display connection) is created. On Wayland GTK derives the surface
  // app_id from g_get_prgname(), which otherwise defaults to the binary name;
  // the compositor / GNOME Shell match that app_id against the installed
  // <app-id>.desktop to pick the taskbar/dash icon, so it has to equal the
  // application id (APPLICATION_ID) the .desktop is named after.
  Glib::set_prgname(mwctx->actx->app_dbus_url);

  auto app = Gtk::Application::create(mwctx->actx->app_dbus_url);

  bool setupOk = true;

  // GTK4 builds, wires and presents the window from the activate handler, once
  // the application (and therefore GTK) has been initialized. The window is no
  // longer passed to Application::run(); it is registered with add_window().
  app->signal_activate().connect([this, app, &setupOk]() {
    if (!prepare_widgets()) {
      LOGE("Failure during the window widgets preparation");
      setupOk = false;
      app->quit();
      return;
    }

    assert(mwctx->wloader->all_widget_are_valid());

    if (!mwctx->wloader->all_widget_are_valid()) {
      LOGE("Some of the widgets were not found");
      setupOk = false;
      app->quit();
      return;
    }

    if (!mwctx->mweHandler->init(mwctx)) {
      LOGE("Failure to initialize the main window event handler instance");
      setupOk = false;
      app->quit();
      return;
    }

    setup_accelerators(app);

    auto* window = mwctx->wloader->get_window();

    app->add_window(*window);
    window->set_visible(true);
    window->present();
  });

  const int rt = app->run(mwctx->actx->argc, mwctx->actx->argv);

  return rt == 0 && setupOk;
}

void GtkmmIniter::setup_accelerators(const Glib::RefPtr<Gtk::Application>& app)
{
  assert(app);

  // GTK4 replaces GtkAccelGroup with application level accelerators bound to
  // the window "win.*" actions created by WindowLoader.
  using WDC = window::WindowDataContext;

  const std::string p = std::string{WDC::action_group_prefix} + ".";

  app->set_accel_for_action(p + WDC::action_annotations_db_open, "<Control>o");
  app->set_accel_for_action(p + WDC::action_annotations_db_save, "<Control>s");
  app->set_accel_for_action(p + WDC::action_annotations_db_saveas,
                            "<Control><Shift>s");
  app->set_accel_for_action(p + WDC::action_annotations_project_close,
                            "<Control>w");
  app->set_accel_for_action(p + WDC::action_images_open, "<Control><Shift>o");
  app->set_accel_for_action(p + WDC::action_images_web_page_open,
                            "<Control><Shift>u");
  app->set_accel_for_action(p + WDC::action_auto_insert_annotation,
                            "<Control><Shift>f");
}

bool GtkmmIniter::prepare_widgets()
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

void GtkmmIniter::deinit() { mwctx->clear(); }

}  // namespace imagesAnnotatorGtkmm4
