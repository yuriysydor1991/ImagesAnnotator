#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_IMAGESDIRCHANGED_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_IMAGESDIRCHANGED_CLASS_H

#include <string>

#include "src/annotator-events/events/IEvent.h"

namespace events::events
{

/**
 * @brief The images dir changed event.
 */
class ImagesDirChanged : virtual public IEvent
{
 public:
  virtual ~ImagesDirChanged() = default;
  explicit ImagesDirChanged(const std::string& newPath);

  std::string images_dir;
};

}  // namespace events::events

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_IMAGESDIRCHANGED_CLASS_H
