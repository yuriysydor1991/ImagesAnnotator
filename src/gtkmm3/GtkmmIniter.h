#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMMINITER_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMMINITER_CLASS_H

#include <memory>

#include "src/app/ApplicationContext.h"
#include "src/gtkmm3/gtkmm_includes.h"
#include "src/gtkmm3/window/WindowDataContext.h"
#include "src/gtkmm3/window/WindowLoader.h"

namespace templateGtkmm3
{

/**
 * @brief The root GTKmm window class to show
 * templated GUI window.
 */
class GtkmmIniter
{
 public:
  virtual ~GtkmmIniter() = default;
  GtkmmIniter();

  virtual bool run(std::shared_ptr<app::ApplicationContext> ctx);

 protected:
  bool prepare_widgets();

 private:
  void prepare_random_logo();

  std::shared_ptr<window::WindowDataContext> wctx;
  std::shared_ptr<window::WindowLoader> wloader;
};

}  // namespace templateGtkmm3

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMMINITER_CLASS_H
