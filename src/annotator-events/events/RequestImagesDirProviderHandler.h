#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_REQUESTIMAGESDIRPROVIDERHANDLER_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_REQUESTIMAGESDIRPROVIDERHANDLER_CLASS_H

#include <string>

#include "src/annotator-events/events/EventIHandler.h"
#include "src/annotator-events/events/RequestImagesDirProvider.h"

namespace events::events
{

/**
 * @brief Request the images db object provider through the evening system.
 */
class RequestImagesDirProviderHandler : virtual public EventIHandler
{
 public:
  virtual ~RequestImagesDirProviderHandler() = default;
  RequestImagesDirProviderHandler() = default;

  virtual void handle(std::shared_ptr<RequestImagesDirProvider> event) = 0;
};

}  // namespace events::events

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_REQUESTIMAGESDIRPROVIDERHANDLER_CLASS_H
