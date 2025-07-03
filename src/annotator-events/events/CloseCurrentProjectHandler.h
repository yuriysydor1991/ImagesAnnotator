#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_CLOSECURRENTPROJECTHANDLER_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_CLOSECURRENTPROJECTHANDLER_CLASS_H

#include <memory>

#include "src/annotator-events/events/CloseCurrentProject.h"
#include "src/annotator-events/events/EventIHandler.h"
#include "src/annotator-events/events/ImagesDirChanged.h"

namespace events::events
{

/**
 * @brief Handler to close the current project.
 */
class CloseCurrentProjectHandler : virtual public EventIHandler
{
 public:
  virtual ~CloseCurrentProjectHandler() = default;
  CloseCurrentProjectHandler() = default;

  virtual void handle(std::shared_ptr<CloseCurrentProject> event) = 0;
};

}  // namespace events::events

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_CLOSECURRENTPROJECTHANDLER_CLASS_H
