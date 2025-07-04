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
