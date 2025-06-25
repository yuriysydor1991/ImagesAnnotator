#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_CONTROLLER_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_CONTROLLER_CLASS_H

#include <memory>
#include <set>

#include "src/annotator-events/IImagesAnnotatorEventController.h"
#include "src/annotator-events/events/AnnotationsDirChanged.h"
#include "src/annotator-events/events/AnnotationsDirChangedIHandler.h"
#include "src/annotator-events/events/EventsFactory.h"
#include "src/annotator-events/events/ImagesDirChanged.h"
#include "src/annotator-events/events/ImagesDirChangedIHandler.h"
#include "src/annotator-events/events/ImagesDirProviderChanged.h"
#include "src/annotator-events/events/ImagesDirProviderChangedHandler.h"
#include "src/app/ApplicationContext.h"

namespace events
{

/**
 * @brief Intermediate events class interface between the components.
 */
class ImagesAnnotatorEventController
    : virtual public IImagesAnnotatorEventController
{
 public:
  virtual ~ImagesAnnotatorEventController() = default;
  ImagesAnnotatorEventController(
      std::shared_ptr<app::ApplicationContext> nactx);

  virtual void onAnnotationsDirChanged(const std::string& newPath) override;
  virtual void submit(
      std::shared_ptr<events::AnnotationsDirChanged> event) override;
  virtual void subscribe(
      std::shared_ptr<events::AnnotationsDirChangedIHandler> handler) override;

  virtual void onImagesDirChanged(const std::string& newPath) override;
  virtual void submit(std::shared_ptr<events::ImagesDirChanged> event) override;
  virtual void subscribe(
      std::shared_ptr<events::ImagesDirChangedIHandler> handler) override;

  virtual void submit(
      std::shared_ptr<events::ImagesDirProviderChanged> newIDBProvider);
  virtual void subscribe(
      std::shared_ptr<events::ImagesDirProviderChangedHandler> newIDBProvider);

  virtual bool deinit() override;

 private:
  using imagesHSet =
      std::set<std::shared_ptr<events::ImagesDirChangedIHandler>>;
  using annotationsHSet =
      std::set<std::shared_ptr<events::AnnotationsDirChangedIHandler>>;
  using imagesDBProviderSet =
      std::set<std::shared_ptr<events::ImagesDirProviderChangedHandler>>;

  template <class SubsQueue, class EventT>
  void unified_submit(SubsQueue& queue, std::shared_ptr<EventT> event);

  std::shared_ptr<app::ApplicationContext> actx;

  imagesHSet images_handlers;
  annotationsHSet annotations_handlers;
  imagesDBProviderSet iprovider_handlers;

  std::shared_ptr<events::EventsFactory> efactory;
};

}  // namespace events

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_CONTROLLER_CLASS_H
