#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_STOREREQUEST_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_STOREREQUEST_CLASS_H

#include <string>

#include "src/annotator-events/events/IRequestEvent.h"

namespace events::events
{

/**
 * @brief Fired event mean to store current project data into available file.
 */
class StoreRequest : virtual public IRequestEvent
{
 public:
  virtual ~StoreRequest() = default;
  StoreRequest(const std::string& newdbpath);

  std::string dbpath;
};

}  // namespace events::events

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_STOREREQUEST_CLASS_H
