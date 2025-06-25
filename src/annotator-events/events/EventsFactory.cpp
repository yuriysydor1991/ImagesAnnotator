#include "src/annotator-events/events/EventsFactory.h"

#include <cassert>
#include <memory>
#include <set>

#include "src/annotator-events/events/AnnotationsDirChanged.h"
#include "src/annotator-events/events/AnnotationsDirChangedIHandler.h"
#include "src/annotator-events/events/ImagesDirChanged.h"
#include "src/annotator-events/events/ImagesDirChangedIHandler.h"

namespace events::events
{

std::shared_ptr<EventsFactory> EventsFactory::instance()
{
  static std::shared_ptr<EventsFactory> ins = std::make_shared<EventsFactory>();

  return ins;
}

std::shared_ptr<AnnotationsDirChanged>
EventsFactory::create_annotations_dir_changed(const std::string& newPath)
{
  assert(!newPath.empty());

  return std::make_shared<AnnotationsDirChanged>(newPath);
}

std::shared_ptr<ImagesDirChanged> EventsFactory::create_image_dir_changed(
    const std::string& newPath)
{
  assert(!newPath.empty());

  return std::make_shared<ImagesDirChanged>(newPath);
}

std::shared_ptr<RequestImagesDirProvider>
EventsFactory::create_image_dir_object_request()
{
  return std::make_shared<RequestImagesDirProvider>();
}

std::shared_ptr<ImagesDirProviderChanged>
EventsFactory::create_images_dir_provider_changed(
    std::shared_ptr<ImagesPathsDBProvider> np)
{
  assert(np != nullptr);

  return std::make_shared<ImagesDirProviderChanged>(np);
}

}  // namespace events::events
