#include "src/gtkmm3/GtkmmIniter.h"

#include <cassert>

#include "project-global-decls.h"

namespace templateGtkmm3
{

int GtkmmIniter::run(int& argc, char**& argv)
{
  auto app =
      Gtk::Application::create(argc, argv, project_decls::PROJECT_FLATPAK_URL);

  prepare_widgets();

  // assert(window != nullptr);
  // assert(image != nullptr);

  // if (window == nullptr || image == nullptr) {
  //   throw std::runtime_error("Unable to find some of the main widgets");
  // }

  // prepare_random_logo();

  window->show_all_children();

  return app->run(*window);
}

void GtkmmIniter::prepare_widgets()
{
  builder = Gtk::Builder::create_from_resource(UI_res_path);

  if (!builder) {
    throw std::runtime_error("Failed to create the builder");
  }

  builder->get_widget("main_window", window);

  // if (window == nullptr) {
  //   throw std::runtime_error("Failed to get the main window");
  // }

  // builder->get_widget("random_image", image);

  // if (image == nullptr) {
  //   throw std::runtime_error("Failed to get an image");
  // }
}

void GtkmmIniter::prepare_random_logo()
{
  assert(image != nullptr);

  if (image == nullptr) {
    return;
  }

  image->set_from_resource(logo_res_path);
}

}  // namespace templateGtkmm3
