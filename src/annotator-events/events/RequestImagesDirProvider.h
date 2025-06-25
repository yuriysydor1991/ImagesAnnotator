#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_REQUESTIMAGESDIRPROVIDER_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_REQUESTIMAGESDIRPROVIDER_CLASS_H

#include <string>

#include "src/annotator-events/events/IRequestEvent.h"
#include "src/annotator-events/events/ImagesPathsDBProvider.h"

namespace events::events
{

/**
 * @brief Request the images db object provider through the evening system.
 */
class RequestImagesDirProvider : virtual public IRequestEvent
{
 public:
  virtual ~RequestImagesDirProvider() = default;
  RequestImagesDirProvider() = default;
};

}  // namespace events::events

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_REQUESTIMAGESDIRPROVIDER_CLASS_H
