#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_APPLICATION_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_APPLICATION_CLASS_H

#include <memory>

#include "src/annotator-business/AnnotatorController.h"
#include "src/annotator-events/ImagesAnnotatorEventController.h"
#include "src/app/ApplicationContext.h"
#include "src/app/IApplication.h"
#include "src/gtkmm3/GtkmmIniter.h"

namespace app
{

/**
 * @brief The default application implementation should be placed into
 * into current class.
 */
class Application : public IApplication
{
 public:
  virtual ~Application() = default;
  Application() = default;

  /**
   * @brief Implemented default application. See IApplication interface.
   *
   * @param ctx Application's run context with command line parameters etc.
   *
   * @return Returns the application return status. A zero value on the success
   * and other value otherwise.
   */
  virtual int run(std::shared_ptr<ApplicationContext> ctx) override;

 protected:
  virtual std::shared_ptr<templateGtkmm3::GtkmmIniter> create_gtkmm_initer();
  virtual std::shared_ptr<iannotator::AnnotatorController>
  create_annotator_b_controller();
  virtual std::shared_ptr<events::ImagesAnnotatorEventController>
  create_event_controller(std::shared_ptr<ApplicationContext> ctx);

  std::shared_ptr<templateGtkmm3::GtkmmIniter> gtkmmIniter;
  std::shared_ptr<iannotator::AnnotatorController> abcontroller;
};

}  // namespace app

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_APPLICATION_CLASS_H
