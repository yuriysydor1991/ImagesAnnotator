#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_IMAGESDIRCHANGEDIHANDLER_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_IMAGESDIRCHANGEDIHANDLER_CLASS_H

#include <memory>

#include "src/annotator-events/events/EventIHandler.h"
#include "src/annotator-events/events/ImagesDirChanged.h"

namespace events::events
{

/**
 * @brief The images dir changed event handler interface.
 */
class ImagesDirChangedIHandler : virtual public EventIHandler
{
 public:
  virtual ~ImagesDirChangedIHandler() = default;
  ImagesDirChangedIHandler() = default;

  virtual void handle(std::shared_ptr<ImagesDirChanged> event) = 0;
};

}  // namespace events::events

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_IMAGESDIRCHANGEDIHANDLER_CLASS_H
