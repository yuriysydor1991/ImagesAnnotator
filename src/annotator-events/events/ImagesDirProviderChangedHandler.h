#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_IMAGESDIRPROVIDERCHANGEDIHANDLER_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_IMAGESDIRPROVIDERCHANGEDIHANDLER_CLASS_H

#include <memory>

#include "src/annotator-events/events/EventIHandler.h"
#include "src/annotator-events/events/ImagesDirProviderChanged.h"

namespace events::events
{

/**
 * @brief The images dir changed event handler interface.
 */
class ImagesDirProviderChangedHandler : virtual public EventIHandler
{
 public:
  virtual ~ImagesDirProviderChangedHandler() = default;
  ImagesDirProviderChangedHandler() = default;

  virtual void handle(std::shared_ptr<ImagesDirProviderChanged> event) = 0;
};

}  // namespace events::events

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_IMAGESDIRPROVIDERCHANGEDIHANDLER_CLASS_H
