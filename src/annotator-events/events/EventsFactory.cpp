#include "src/annotator-events/events/EventsFactory.h"

#include <memory>
#include <set>

#include "src/annotator-events/events/AnnotationsDirChanged.h"
#include "src/annotator-events/events/AnnotationsDirChangedIHandler.h"
#include "src/annotator-events/events/ImagesDirChanged.h"
#include "src/annotator-events/events/ImagesDirChangedIHandler.h"

namespace events::events
{

std::shared_ptr<AnnotationsDirChanged>
EventsFactory::create_annotations_dir_changed(const std::string& newPath)
{
  return std::make_shared<AnnotationsDirChanged>(newPath);
}

std::shared_ptr<ImagesDirChanged> EventsFactory::create_image_dir_changed(
    const std::string& newPath)
{
  return std::make_shared<ImagesDirChanged>(newPath);
}

}  // namespace events::events
