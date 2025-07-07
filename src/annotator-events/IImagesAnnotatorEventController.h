#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_CONTROLLER_INTERFACE_ABSTRACT_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_CONTROLLER_INTERFACE_ABSTRACT_CLASS_H

#include <memory>

#include "src/annotator-events/events/AnnotationsDirChanged.h"
#include "src/annotator-events/events/AnnotationsDirChangedIHandler.h"
#include "src/annotator-events/events/CloseCurrentProject.h"
#include "src/annotator-events/events/CloseCurrentProjectHandler.h"
#include "src/annotator-events/events/CurrentImageChanged.h"
#include "src/annotator-events/events/CurrentImageChangedHandler.h"
#include "src/annotator-events/events/EventsFactory.h"
#include "src/annotator-events/events/ExportPlainTxt2FolderRequest.h"
#include "src/annotator-events/events/ExportPlainTxt2FolderRequestHandler.h"
#include "src/annotator-events/events/ImagesDirChanged.h"
#include "src/annotator-events/events/ImagesDirChangedIHandler.h"
#include "src/annotator-events/events/ImagesDirProviderChanged.h"
#include "src/annotator-events/events/ImagesDirProviderChangedHandler.h"
#include "src/annotator-events/events/RequestImagesDirProvider.h"
#include "src/annotator-events/events/RequestImagesDirProviderHandler.h"
#include "src/annotator-events/events/StoreRequest.h"
#include "src/annotator-events/events/StoreRequestHandler.h"

namespace events
{

/**
 * @brief An abstract class for the images annotator event controller classes.
 */
class IImagesAnnotatorEventController
{
 public:
  virtual ~IImagesAnnotatorEventController() = default;
  IImagesAnnotatorEventController() = default;

  virtual bool deinit() = 0;

  virtual std::shared_ptr<events::EventsFactory> get_events_factory() = 0;

  virtual void onAnnotationsDirChanged(const std::string& newPath) = 0;
  virtual void submit(std::shared_ptr<events::AnnotationsDirChanged> event) = 0;
  virtual void subscribe(
      std::shared_ptr<events::AnnotationsDirChangedIHandler> handler) = 0;

  virtual void onImagesDirChanged(const std::string& newPath) = 0;
  virtual void submit(std::shared_ptr<events::ImagesDirChanged> event) = 0;
  virtual void subscribe(
      std::shared_ptr<events::ImagesDirChangedIHandler> handler) = 0;

  virtual void submit(
      std::shared_ptr<events::ImagesDirProviderChanged> newIDBProvider) = 0;
  virtual void subscribe(
      std::shared_ptr<events::ImagesDirProviderChangedHandler>
          newIDBProvider) = 0;

  virtual void submit(
      std::shared_ptr<events::RequestImagesDirProvider> request) = 0;
  virtual void subscribe(
      std::shared_ptr<events::RequestImagesDirProviderHandler> request) = 0;

  virtual void submit(
      std::shared_ptr<events::CurrentImageChanged> newCurrentImage) = 0;
  virtual void subscribe(
      std::shared_ptr<events::CurrentImageChangedHandler> request) = 0;

  virtual void submit(std::shared_ptr<events::StoreRequest> event) = 0;
  virtual void subscribe(
      std::shared_ptr<events::StoreRequestHandler> handler) = 0;

  virtual void submit(std::shared_ptr<events::CloseCurrentProject> event) = 0;
  virtual void subscribe(
      std::shared_ptr<events::CloseCurrentProjectHandler> handler) = 0;

  virtual void submit(events::ExportPlainTxt2FolderRequestPtr event) = 0;
  virtual void subscribe(
      events::ExportPlainTxt2FolderRequestHandlerPtr handler) = 0;
};

}  // namespace events

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_CONTROLLER_INTERFACE_ABSTRACT_CLASS_H
