#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_CURRENTIMAGECHANGEDHANDLER_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_CURRENTIMAGECHANGEDHANDLER_CLASS_H

#include <memory>
#include <string>

#include "src/annotator-events/events/CurrentImageChanged.h"
#include "src/annotator-events/events/EventIHandler.h"

namespace events::events
{

/**
 * @brief The images dir changed event.
 */
class CurrentImageChangedHandler : virtual public EventIHandler
{
 public:
  virtual ~CurrentImageChangedHandler() = default;
  CurrentImageChangedHandler() = default;

  virtual void handle(
      std::shared_ptr<CurrentImageChanged> new_current_image) = 0;
};

}  // namespace events::events

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_CURRENTIMAGECHANGEDHANDLER_CLASS_H
