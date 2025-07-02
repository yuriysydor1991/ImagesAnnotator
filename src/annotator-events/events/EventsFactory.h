#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_FACTORY_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_FACTORY_CLASS_H

#include <memory>
#include <string>

#include "src/annotator-events/events/AnnotationsDirChanged.h"
#include "src/annotator-events/events/AnnotationsDirChangedIHandler.h"
#include "src/annotator-events/events/CurrentImageChanged.h"
#include "src/annotator-events/events/ImageRecord.h"
#include "src/annotator-events/events/ImageRecordRect.h"
#include "src/annotator-events/events/ImagesDirChanged.h"
#include "src/annotator-events/events/ImagesDirChangedIHandler.h"
#include "src/annotator-events/events/ImagesDirProviderChanged.h"
#include "src/annotator-events/events/ImagesPathsDBProvider.h"
#include "src/annotator-events/events/RequestImagesDirProvider.h"
#include "src/annotator-events/events/StoreRequest.h"

namespace events::events
{

/**
 * @brief All the events objects factory.
 */
class EventsFactory : public std::enable_shared_from_this<EventsFactory>
{
 public:
  virtual ~EventsFactory() = default;
  EventsFactory() = default;

  static std::shared_ptr<EventsFactory> instance();

  virtual std::shared_ptr<AnnotationsDirChanged> create_annotations_dir_changed(
      const std::string& newPath);

  virtual std::shared_ptr<ImagesDirChanged> create_image_dir_changed(
      const std::string& newPath);

  virtual std::shared_ptr<RequestImagesDirProvider>
  create_image_dir_object_request();

  virtual std::shared_ptr<ImagesDirProviderChanged>
  create_images_dir_provider_changed(std::shared_ptr<ImagesPathsDBProvider> np);

  virtual std::shared_ptr<CurrentImageChanged> create_current_image_changed(
      std::shared_ptr<ImageRecord>);

  virtual std::shared_ptr<ImageRecordRect> create_image_rect_record();
  virtual std::shared_ptr<ImageRecordRect> create_image_rect_record(
      const std::string& npath, const int& nx, const int& ny, const int& nwidth,
      const int& nheight);

  virtual std::shared_ptr<ImageRecord> create_image_record(
      const std::string& nirpath = "");

  virtual std::shared_ptr<StoreRequest> create_store_request(
      const std::string& newdbpath = "");
};

}  // namespace events::events

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_FACTORY_CLASS_H
