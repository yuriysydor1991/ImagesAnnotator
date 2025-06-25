#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_CONTROLLER_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_CONTROLLER_CLASS_H

#include <gmock/gmock.h>

#include <memory>
#include <set>

#include "src/annotator-events/IImagesAnnotatorEventController.h"
#include "src/annotator-events/events/AnnotationsDirChanged.h"
#include "src/annotator-events/events/AnnotationsDirChangedIHandler.h"
#include "src/annotator-events/events/EventsFactory.h"
#include "src/annotator-events/events/ImagesDirChanged.h"
#include "src/annotator-events/events/ImagesDirChangedIHandler.h"
#include "src/app/ApplicationContext.h"

namespace events
{

class ImagesAnnotatorEventController : public IImagesAnnotatorEventController
{
 public:
  virtual ~ImagesAnnotatorEventController() = default;
  ImagesAnnotatorEventController(
      [[maybe_unused]] std::shared_ptr<app::ApplicationContext> nactx){};

  MOCK_METHOD(void, onAnnotationsDirChanged, (const std::string& newPath),
              (override));
  MOCK_METHOD(void, onDirChanged,
              (std::shared_ptr<events::AnnotationsDirChanged> event),
              (override));
  MOCK_METHOD(void, subscribe,
              (std::shared_ptr<events::AnnotationsDirChangedIHandler> handler),
              (override));

  MOCK_METHOD(void, onImagesDirChanged, (const std::string& newPath),
              (override));
  MOCK_METHOD(void, onDirChanged,
              (std::shared_ptr<events::ImagesDirChanged> event), (override));
  MOCK_METHOD(void, subscribe,
              (std::shared_ptr<events::ImagesDirChangedIHandler> handler),
              (override));

  MOCK_METHOD(bool, deinit, (), (override));
};

}  // namespace events

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_CONTROLLER_CLASS_H
