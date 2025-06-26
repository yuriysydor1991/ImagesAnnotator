#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_ANNOTATIONSDIRCHANGED_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_ANNOTATIONSDIRCHANGED_CLASS_H

#include <string>

#include "src/annotator-events/events/IEvent.h"

namespace events::events
{

/**
 * @brief The annotations dir changed event.
 */
class AnnotationsDirChanged : virtual public IEvent
{
 public:
  virtual ~AnnotationsDirChanged() = default;
  explicit AnnotationsDirChanged(const std::string& newPath);

  std::string annotations_dir;
};

}  // namespace events::events

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_ANNOTATIONSDIRCHANGED_CLASS_H
