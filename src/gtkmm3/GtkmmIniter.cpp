#include "src/gtkmm3/GtkmmIniter.h"

#include <cassert>

#include "project-global-decls.h"
#include "src/app/IApplication.h"
#include "src/gtkmm3/window/WindowLoader.h"
#include "src/log/log.h"

namespace templateGtkmm3
{

int GtkmmIniter::run(int& argc, char**& argv)
{
  auto app =
      Gtk::Application::create(argc, argv, project_decls::PROJECT_FLATPAK_URL);

  if (!prepare_widgets()) {
    return app::IApplication::INVALID;
  }

  assert(wctx != nullptr);
  assert(wctx->window != nullptr);
  assert(wctx->image != nullptr);

  if (wctx == nullptr) {
    LOGE("No valid window context was found");
    return app::IApplication::INVALID;
  }

  if (!wctx->all_widget_pointers_valid()) {
    LOGE("Some of the widgets were not found");
    return app::IApplication::INVALID;
  }

  // prepare_random_logo();

  wctx->window->show_all_children();

  return app->run(*wctx->window);
}

bool GtkmmIniter::prepare_widgets()
{
  auto wloader = std::make_shared<window::WindowLoader>();
  wctx = std::make_shared<window::WindowDataContext>();

  return wloader->load_window(wctx);
}

void GtkmmIniter::prepare_random_logo()
{
  assert(wctx != nullptr);
  assert(wctx->image != nullptr);

  if (wctx == nullptr || wctx->image == nullptr) {
    LOGE("No valid context found");
    return;
  }

  wctx->image->set_from_resource(wctx->logo_res_path);
}

}  // namespace templateGtkmm3
