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

#include "src/gtkmm3/GtkmmIniter.h"

#include <cassert>

#include "project-global-decls.h"
#include "src/log/log.h"

namespace templateGtkmm3
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

  auto app = Gtk::Application::create(mwctx->actx->argc, mwctx->actx->argv,
                                      mwctx->actx->app_dbus_url);

  if (!prepare_widgets()) {
    return false;
  }

  assert(mwctx->wloader->all_widget_are_valid());

  if (!mwctx->wloader->all_widget_are_valid()) {
    LOGE("Some of the widgets were not found");
    return false;
  }

  mwctx->wloader->get_window()->show_all_children();

  if (!mwctx->mweHandler->init(mwctx)) {
    LOGE("Failure to initialize the main window event handler instance");
    return false;
  }

  const auto rt = app->run(*mwctx->wloader->get_window());

  return rt == 0;
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

}  // namespace templateGtkmm3
