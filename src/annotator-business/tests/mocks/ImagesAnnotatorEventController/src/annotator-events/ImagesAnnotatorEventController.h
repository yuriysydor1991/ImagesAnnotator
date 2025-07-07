#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_CONTROLLER_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_CONTROLLER_CLASS_H

#include <gmock/gmock.h>

#include <memory>
#include <set>

#include "src/annotator-events/IImagesAnnotatorEventController.h"
#include "src/annotator-events/events/AnnotationsDirChanged.h"
#include "src/annotator-events/events/AnnotationsDirChangedIHandler.h"
#include "src/annotator-events/events/EventsFactory.h"
#include "src/annotator-events/events/ExportPlainTxt2FolderRequest.h"
#include "src/annotator-events/events/ExportPlainTxt2FolderRequestHandler.h"
#include "src/annotator-events/events/ImagesDirChanged.h"
#include "src/annotator-events/events/ImagesDirChangedIHandler.h"
#include "src/app/ApplicationContext.h"

namespace events
{

class ImagesAnnotatorEventController : public IImagesAnnotatorEventController
{
 public:
  virtual ~ImagesAnnotatorEventController() = default;
  explicit ImagesAnnotatorEventController(
      [[maybe_unused]] std::shared_ptr<app::ApplicationContext> nactx){};

  MOCK_METHOD(bool, deinit, (), (override));

  MOCK_METHOD(void, onAnnotationsDirChanged, (const std::string& newPath),
              (override));
  MOCK_METHOD(void, submit,
              (std::shared_ptr<events::AnnotationsDirChanged> event),
              (override));
  MOCK_METHOD(void, subscribe,
              (std::shared_ptr<events::AnnotationsDirChangedIHandler> handler),
              (override));

  MOCK_METHOD(void, onImagesDirChanged, (const std::string& newPath),
              (override));
  MOCK_METHOD(void, submit, (std::shared_ptr<events::ImagesDirChanged> event),
              (override));
  MOCK_METHOD(void, subscribe,
              (std::shared_ptr<events::ImagesDirChangedIHandler> handler),
              (override));

  MOCK_METHOD(
      void, submit,
      (std::shared_ptr<events::ImagesDirProviderChanged> newIDBProvider),
      (override));
  MOCK_METHOD(
      void, subscribe,
      (std::shared_ptr<events::ImagesDirProviderChangedHandler> newIDBProvider),
      (override));

  MOCK_METHOD(void, submit,
              (std::shared_ptr<events::RequestImagesDirProvider> request),
              (override));
  MOCK_METHOD(
      void, subscribe,
      (std::shared_ptr<events::RequestImagesDirProviderHandler> request),
      (override));

  MOCK_METHOD(std::shared_ptr<events::EventsFactory>, get_events_factory, (),
              (override));

  MOCK_METHOD(void, submit,
              (std::shared_ptr<events::CurrentImageChanged> newCurrentImage),
              (override));
  MOCK_METHOD(void, subscribe,
              (std::shared_ptr<events::CurrentImageChangedHandler> request),
              (override));

  MOCK_METHOD(void, submit, (std::shared_ptr<events::StoreRequest> event),
              (override));
  MOCK_METHOD(void, subscribe,
              (std::shared_ptr<events::StoreRequestHandler> handler),
              (override));

  MOCK_METHOD(void, submit,
              (std::shared_ptr<events::CloseCurrentProject> event), (override));
  MOCK_METHOD(void, subscribe,
              (std::shared_ptr<events::CloseCurrentProjectHandler> handler),
              (override));

  MOCK_METHOD(void, submit, (events::ExportPlainTxt2FolderRequestPtr event),
              (override));
  MOCK_METHOD(void, subscribe,
              (events::ExportPlainTxt2FolderRequestHandlerPtr handler),
              (override));
};

}  // namespace events

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_CONTROLLER_CLASS_H
