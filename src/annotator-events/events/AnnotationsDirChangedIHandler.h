#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_ANNOTATIONSDIRCHANGEDIHANDLER_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_ANNOTATIONSDIRCHANGEDIHANDLER_CLASS_H

#include <memory>

#include "src/annotator-events/events/AnnotationsDirChanged.h"
#include "src/annotator-events/events/EventIHandler.h"

namespace events::events
{

/**
 * @brief The annotations dir changed event handler interface.
 */
class AnnotationsDirChangedIHandler : virtual public EventIHandler
{
 public:
  virtual ~AnnotationsDirChangedIHandler() = default;
  AnnotationsDirChangedIHandler() = default;

  virtual void handle(std::shared_ptr<AnnotationsDirChanged> event) = 0;
};

}  // namespace events::events

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_ANNOTATIONSDIRCHANGEDIHANDLER_CLASS_H
