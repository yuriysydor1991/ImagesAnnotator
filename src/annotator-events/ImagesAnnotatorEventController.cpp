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
      efactory{events::EventsFactory::instance()}
{
}

void ImagesAnnotatorEventController::onAnnotationsDirChanged(
    const std::string& newPath)
{
  auto annDE = efactory->create_annotations_dir_changed(newPath);

  submit(annDE);
}

void ImagesAnnotatorEventController::submit(
    std::shared_ptr<events::AnnotationsDirChanged> event)
{
  unified_submit(annotations_handlers, event);
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

  submit(imagesDE);
}

void ImagesAnnotatorEventController::submit(
    std::shared_ptr<events::ImagesDirChanged> event)
{
  unified_submit(images_handlers, event);
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

void ImagesAnnotatorEventController::submit(
    std::shared_ptr<events::ImagesDirProviderChanged> newIDBProvider)
{
  unified_submit(iprovider_handlers, newIDBProvider);
}

void ImagesAnnotatorEventController::subscribe(
    std::shared_ptr<events::ImagesDirProviderChangedHandler> newIDBProvider)
{
  iprovider_handlers.insert(newIDBProvider);
}

std::shared_ptr<events::EventsFactory>
ImagesAnnotatorEventController::get_events_factory()
{
  return efactory;
}

template <class SubsQueue, class EventT>
inline void ImagesAnnotatorEventController::unified_submit(
    SubsQueue& queue, std::shared_ptr<EventT> event)
{
  assert(event != nullptr);

  if (event == nullptr) {
    LOGE("No valid event object pointer was given");
    return;
  }

  for (auto& ih : queue) {
    ih->handle(event);
  }
}

void ImagesAnnotatorEventController::submit(
    std::shared_ptr<events::RequestImagesDirProvider> request)
{
  unified_submit(imagesDBObject_handlers, request);
}

void ImagesAnnotatorEventController::subscribe(
    std::shared_ptr<events::RequestImagesDirProviderHandler> request)
{
  imagesDBObject_handlers.insert(request);
}

void ImagesAnnotatorEventController::submit(
    std::shared_ptr<events::CurrentImageChanged> event)
{
  unified_submit(currentImage_handlers, event);
}

void ImagesAnnotatorEventController::subscribe(
    std::shared_ptr<events::CurrentImageChangedHandler> handler)
{
  currentImage_handlers.insert(handler);
}

void ImagesAnnotatorEventController::submit(
    std::shared_ptr<events::StoreRequest> event)
{
  unified_submit(store_handlers, event);
}

void ImagesAnnotatorEventController::subscribe(
    std::shared_ptr<events::StoreRequestHandler> handler)
{
  store_handlers.insert(handler);
}

}  // namespace events
