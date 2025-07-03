#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_CLOSECURRENTPROJECT_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_CLOSECURRENTPROJECT_CLASS_H

#include <string>

#include "src/annotator-events/events/IEvent.h"

namespace events::events
{

/**
 * @brief The images dir changed event.
 */
class CloseCurrentProject : virtual public IEvent
{
 public:
  virtual ~CloseCurrentProject() = default;
  CloseCurrentProject() = default;
};

}  // namespace events::events

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_CLOSECURRENTPROJECT_CLASS_H
