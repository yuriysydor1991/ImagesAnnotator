#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_IREQUESTEVENT_ABSTRACT_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_IREQUESTEVENT_ABSTRACT_CLASS_H

#include "src/annotator-events/events/IEvent.h"

namespace events::events
{

/**
 * @brief The general abstract class for the request-type events.
 */
class IRequestEvent : virtual public IEvent
{
 public:
  virtual ~IRequestEvent() = default;
  IRequestEvent() = default;
};

}  // namespace events::events

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_IREQUESTEVENT_ABSTRACT_CLASS_H
