#include "src/gtkmm3/GtkmmIniter.h"

#include <cassert>

#include "project-global-decls.h"
#include "src/log/log.h"

namespace templateGtkmm3
{
GtkmmIniter::GtkmmIniter()
    : wctx{std::make_shared<window::WindowDataContext>()},
      wloader{std::make_shared<window::WindowLoader>()},
      cwFactory{std::make_shared<CWFactory>()},
      imagesVDB{}
{
  assert(wctx != nullptr);
  assert(wloader != nullptr);
  assert(cwFactory != nullptr);
}

bool GtkmmIniter::run(std::shared_ptr<app::ApplicationContext> ctx)
{
  assert(ctx != nullptr);
  assert(wctx != nullptr);
  assert(wloader != nullptr);

  if (ctx == nullptr) {
    return false;
  }

  auto app = Gtk::Application::create(ctx->argc, ctx->argv,
                                      project_decls::PROJECT_FLATPAK_URL);

  if (!prepare_widgets()) {
    return false;
  }

  assert(wloader->all_widget_are_valid());

  if (!wloader->all_widget_are_valid()) {
    LOGE("Some of the widgets were not found");
    return false;
  }

  // prepare_random_logo();

  wloader->get_window()->show_all_children();

  const auto rt = app->run(*wloader->get_window());

  return rt == 0;
}

bool GtkmmIniter::prepare_widgets()
{
  assert(wctx != nullptr);
  assert(wloader != nullptr);

  if (wloader == nullptr) {
    LOGE("No loader available");
    return false;
  }

  if (!wloader->load_window(wctx)) {
    LOGE("Failure during the window load");
    return false;
  }

  assert(wloader->get_window() != nullptr);
  assert(wloader->all_widget_are_valid());

  wloader->get_window()->maximize();

  return wloader->get_window() != nullptr;
}

void GtkmmIniter::prepare_random_logo()
{
  assert(wctx != nullptr);
  assert(wloader != nullptr);

  if (wloader == nullptr || wloader->get_image() == nullptr) {
    LOGE("No valid widget pointer found");
    return;
  }

  wloader->get_image()->set_from_resource(wctx->logo_res_path);
}

void GtkmmIniter::handle(std::shared_ptr<ImagesDirProviderChanged> event)
{
  assert(event != nullptr);
  assert(event->images_provider != nullptr);
  assert(wloader != nullptr);
  assert(wloader->get_images_list() != nullptr);
  assert(cwFactory != nullptr);

  if (event == nullptr || event->images_provider == nullptr) {
    LOGE("No valid event object provided");
    return;
  }

  auto& imagesDB = event->images_provider->get_images_db();

  imagesVDB = cwFactory->create_images_visual_db(imagesDB);

  auto& imagesListView = *wloader->get_images_list();

  auto children = imagesListView.get_children();

  for (auto* child : children) {
    imagesListView.remove(*child);
  }

  for (auto& r : imagesVDB) {
    imagesListView.append(*r);
  }
}

}  // namespace templateGtkmm3
