#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_CONTROLLER_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_CONTROLLER_CLASS_H

#include <memory>
#include <set>

#include "src/annotator-events/IImagesAnnotatorEventController.h"
#include "src/annotator-events/events/AnnotationsDirChanged.h"
#include "src/annotator-events/events/AnnotationsDirChangedIHandler.h"
#include "src/annotator-events/events/CurrentImageChanged.h"
#include "src/annotator-events/events/CurrentImageChangedHandler.h"
#include "src/annotator-events/events/EventsFactory.h"
#include "src/annotator-events/events/ImagesDirChanged.h"
#include "src/annotator-events/events/ImagesDirChangedIHandler.h"
#include "src/annotator-events/events/ImagesDirProviderChanged.h"
#include "src/annotator-events/events/ImagesDirProviderChangedHandler.h"
#include "src/annotator-events/events/RequestImagesDirProvider.h"
#include "src/annotator-events/events/RequestImagesDirProviderHandler.h"
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
  explicit ImagesAnnotatorEventController(
      std::shared_ptr<app::ApplicationContext> nactx);

  virtual std::shared_ptr<events::EventsFactory> get_events_factory() override;

  virtual bool deinit() override;

  virtual void onAnnotationsDirChanged(const std::string& newPath) override;
  virtual void submit(
      std::shared_ptr<events::AnnotationsDirChanged> event) override;
  virtual void subscribe(
      std::shared_ptr<events::AnnotationsDirChangedIHandler> handler) override;

  virtual void onImagesDirChanged(const std::string& newPath) override;
  virtual void submit(std::shared_ptr<events::ImagesDirChanged> event) override;
  virtual void subscribe(
      std::shared_ptr<events::ImagesDirChangedIHandler> handler) override;

  virtual void submit(std::shared_ptr<events::ImagesDirProviderChanged>
                          newIDBProvider) override;
  virtual void subscribe(
      std::shared_ptr<events::ImagesDirProviderChangedHandler> newIDBProvider)
      override;

  virtual void submit(
      std::shared_ptr<events::RequestImagesDirProvider> request) override;
  virtual void subscribe(
      std::shared_ptr<events::RequestImagesDirProviderHandler> request)
      override;

  virtual void submit(
      std::shared_ptr<events::CurrentImageChanged> event) override;
  virtual void subscribe(
      std::shared_ptr<events::CurrentImageChangedHandler> handler) override;

 private:
  using imagesHSet =
      std::set<std::shared_ptr<events::ImagesDirChangedIHandler>>;
  using annotationsHSet =
      std::set<std::shared_ptr<events::AnnotationsDirChangedIHandler>>;
  using imagesDBProviderSet =
      std::set<std::shared_ptr<events::ImagesDirProviderChangedHandler>>;
  using imagesDBORequestersSet =
      std::set<std::shared_ptr<events::RequestImagesDirProviderHandler>>;
  using currentImageChangedSet =
      std::set<std::shared_ptr<events::CurrentImageChangedHandler>>;

  template <class SubsQueue, class EventT>
  static void unified_submit(SubsQueue& queue, std::shared_ptr<EventT> event);

  std::shared_ptr<app::ApplicationContext> actx;

  imagesHSet images_handlers;
  annotationsHSet annotations_handlers;
  imagesDBProviderSet iprovider_handlers;
  imagesDBORequestersSet imagesDBObject_handlers;
  currentImageChangedSet currentImage_handlers;

  std::shared_ptr<events::EventsFactory> efactory;
};

}  // namespace events

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_CONTROLLER_CLASS_H
