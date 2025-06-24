#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_FACTORY_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_FACTORY_CLASS_H

#include <memory>
#include <string>

#include "src/annotator-events/events/AnnotationsDirChanged.h"
#include "src/annotator-events/events/AnnotationsDirChangedIHandler.h"
#include "src/annotator-events/events/ImagesDirChanged.h"
#include "src/annotator-events/events/ImagesDirChangedIHandler.h"

namespace events::events
{

/**
 * @brief All the events objects factory.
 */
class EventsFactory
{
 public:
  virtual ~EventsFactory() = default;
  EventsFactory() = default;

  virtual std::shared_ptr<AnnotationsDirChanged> create_annotations_dir_changed(
      const std::string& newPath);
  virtual std::shared_ptr<ImagesDirChanged> create_image_dir_changed(
      const std::string& newPath);
};

}  // namespace events::events

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_FACTORY_CLASS_H
