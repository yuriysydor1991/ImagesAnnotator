#include "src/annotator-events/ImagesAnnotatorEventController.h"

#include <cassert>
#include <memory>
#include <set>

#include "src/annotator-events/events/AnnotationsDirChanged.h"
#include "src/annotator-events/events/AnnotationsDirChangedIHandler.h"
#include "src/annotator-events/events/ImagesDirChanged.h"
#include "src/annotator-events/events/ImagesDirChangedIHandler.h"
#include "src/log/log.h"

namespace events
{

ImagesAnnotatorEventController::ImagesAnnotatorEventController(
    std::shared_ptr<app::ApplicationContext> nactx)
    : actx{nactx},
      images_handlers{},
      annotations_handlers{},
      efactory{std::make_shared<events::EventsFactory>()}
{
}

void ImagesAnnotatorEventController::onAnnotationsDirChanged(
    const std::string& newPath)
{
  auto annDE = efactory->create_annotations_dir_changed(newPath);

  onDirChanged(annDE);
}

void ImagesAnnotatorEventController::onDirChanged(
    std::shared_ptr<events::AnnotationsDirChanged> event)
{
  assert(event != nullptr);

  if (event == nullptr) {
    LOGE("No valid event object pointer was given");
    return;
  }

  for (auto& ih : annotations_handlers) {
    ih->handle(event);
  }
}

void ImagesAnnotatorEventController::subscribe(
    std::shared_ptr<events::AnnotationsDirChangedIHandler> handler)
{
  assert(handler != nullptr);

  if (handler == nullptr) {
    LOGE("No valid handler pointer was given");
    return;
  }

  annotations_handlers.insert(handler);
}

void ImagesAnnotatorEventController::onImagesDirChanged(
    const std::string& newPath)
{
  auto imagesDE = efactory->create_image_dir_changed(newPath);

  onDirChanged(imagesDE);
}

void ImagesAnnotatorEventController::onDirChanged(
    std::shared_ptr<events::ImagesDirChanged> event)
{
  assert(event != nullptr);

  if (event == nullptr) {
    LOGE("No valid event object pointer was given");
    return;
  }

  for (auto& ih : images_handlers) {
    ih->handle(event);
  }
}

void ImagesAnnotatorEventController::subscribe(
    std::shared_ptr<events::ImagesDirChangedIHandler> handler)
{
  assert(handler != nullptr);

  if (handler == nullptr) {
    LOGE("No valid handler pointer was given");
    return;
  }

  images_handlers.insert(handler);
}

bool ImagesAnnotatorEventController::deinit()
{
  actx.reset();
  return true;
}

}  // namespace events
