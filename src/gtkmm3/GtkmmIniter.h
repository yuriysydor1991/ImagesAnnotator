#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMMINITER_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMMINITER_CLASS_H

#include <memory>

#include "src/annotator-events/events/ImagesDirProviderChanged.h"
#include "src/annotator-events/events/ImagesDirProviderChangedHandler.h"
#include "src/app/ApplicationContext.h"
#include "src/gtkmm3/ComponentTypesAliases.h"
#include "src/gtkmm3/MainWindowContext.h"
#include "src/gtkmm3/gtkmm_includes.h"
#include "src/gtkmm3/main-window/WindowDataContext.h"
#include "src/gtkmm3/main-window/WindowLoader.h"
#include "src/gtkmm3/main-window/custom-widgets/CustomWidgetsFactory.h"

namespace templateGtkmm3
{

/**
 * @brief The root GTKmm window class to show
 * templated GUI window.
 */
class GtkmmIniter
    : virtual public events::events::ImagesDirProviderChangedHandler,
      public std::enable_shared_from_this<GtkmmIniter>,
      virtual public ComponentTypesAliases
{
 public:
  virtual ~GtkmmIniter() = default;
  GtkmmIniter();
  GtkmmIniter(std::shared_ptr<MainWindowContext> nmwctx);

  virtual bool run(std::shared_ptr<app::ApplicationContext> ctx);

  virtual void deinit();

 protected:
  using ImagesDirProviderChanged = events::events::ImagesDirProviderChanged;
  using ImagesDirProviderChangedHandler =
      events::events::ImagesDirProviderChangedHandler;

  virtual bool prepare_widgets();
  virtual bool subscribe_4_events();

  virtual void handle(std::shared_ptr<ImagesDirProviderChanged> event) override;

 private:
  std::shared_ptr<MainWindowContext> mwctx;
};

}  // namespace templateGtkmm3

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMMINITER_CLASS_H
