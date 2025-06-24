#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_CONTROLLER_INTERFACE_ABSTRACT_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_CONTROLLER_INTERFACE_ABSTRACT_CLASS_H

#include <memory>

#include "src/annotator-events/events/AnnotationsDirChanged.h"
#include "src/annotator-events/events/AnnotationsDirChangedIHandler.h"
#include "src/annotator-events/events/EventsFactory.h"
#include "src/annotator-events/events/ImagesDirChanged.h"
#include "src/annotator-events/events/ImagesDirChangedIHandler.h"

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

  virtual void onAnnotationsDirChanged(const std::string& newPath) = 0;
  virtual void onDirChanged(
      std::shared_ptr<events::AnnotationsDirChanged> event) = 0;
  virtual void subscribe(
      std::shared_ptr<events::AnnotationsDirChangedIHandler> handler) = 0;

  virtual void onImagesDirChanged(const std::string& newPath) = 0;
  virtual void onDirChanged(
      std::shared_ptr<events::ImagesDirChanged> event) = 0;
  virtual void subscribe(
      std::shared_ptr<events::ImagesDirChangedIHandler> handler) = 0;

  virtual bool deinit() = 0;
};

}  // namespace events

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_CONTROLLER_INTERFACE_ABSTRACT_CLASS_H
