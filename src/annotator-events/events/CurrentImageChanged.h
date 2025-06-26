#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_CURRENTIMAGECHANGED_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_CURRENTIMAGECHANGED_CLASS_H

#include <memory>
#include <string>

#include "src/annotator-events/events/IEvent.h"
#include "src/annotator-events/events/ImageRecord.h"

namespace events::events
{

/**
 * @brief The images dir changed event.
 */
class CurrentImageChanged : virtual public IEvent
{
 public:
  virtual ~CurrentImageChanged() = default;
  explicit CurrentImageChanged(std::shared_ptr<ImageRecord> setCurrentImage);

  std::shared_ptr<ImageRecord> new_current_image;
};

}  // namespace events::events

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_CURRENTIMAGECHANGED_CLASS_H
