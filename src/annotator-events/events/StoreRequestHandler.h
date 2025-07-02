#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_STOREREQUEST_EVENT_HANDLER_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_STOREREQUEST_EVENT_HANDLER_CLASS_H

#include <memory>

#include "src/annotator-events/events/EventIHandler.h"
#include "src/annotator-events/events/ImagesDirChanged.h"
#include "src/annotator-events/events/StoreRequest.h"

namespace events::events
{

/**
 * @brief The project store event handler interface.
 */
class StoreRequestHandler : virtual public EventIHandler
{
 public:
  virtual ~StoreRequestHandler() = default;
  StoreRequestHandler() = default;

  virtual void handle(std::shared_ptr<StoreRequest> event) = 0;
};

}  // namespace events::events

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_STOREREQUEST_EVENT_HANDLER_CLASS_H
