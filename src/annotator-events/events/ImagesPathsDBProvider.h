#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_IMAGESPATHSDBPROVIDER_ABSTRACT_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_IMAGESPATHSDBPROVIDER_ABSTRACT_CLASS_H

#include <memory>
#include <string>
#include <unordered_set>

#include "src/annotator-events/events/IProvider.h"
#include "src/annotator-events/events/ImageRecord.h"

namespace events::events
{

/**
 * @brief The images dir changed event.
 */
class ImagesPathsDBProvider : virtual public IProvider
{
 public:
  using ImagesDB = ImageRecordsSet;

  virtual ~ImagesPathsDBProvider() = default;
  ImagesPathsDBProvider() = default;

  virtual ImagesDB& get_images_db() = 0;
};

}  // namespace events::events

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_IMAGESPATHSDBPROVIDER_ABSTRACT_CLASS_H
