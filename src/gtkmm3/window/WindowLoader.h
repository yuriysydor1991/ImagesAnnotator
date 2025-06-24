#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM_WINDOWLOADER_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM_WINDOWLOADER_CLASS_H

#include <memory>

#include "src/gtkmm3/gtkmm_includes.h"
#include "src/gtkmm3/window/WindowDataContext.h"

namespace templateGtkmm3::window
{

/**
 * @brief Class to hold the window loader routines from the UI xml files.
 */
class WindowLoader
{
 public:
  virtual ~WindowLoader() = default;
  WindowLoader() = default;

  virtual bool load_window(std::shared_ptr<WindowDataContext> wctx);

 private:
  Glib::RefPtr<Gtk::Builder> builder;
};

}  // namespace templateGtkmm3::window

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM_WINDOWLOADER_CLASS_H
