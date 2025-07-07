#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_IMAGESPATHSDBPROVIDER_ABSTRACT_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_IMAGESPATHSDBPROVIDER_ABSTRACT_CLASS_H

#include <memory>
#include <set>
#include <string>

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
  using AnnotationsList = std::set<std::string>;

  virtual ~ImagesPathsDBProvider() = default;
  ImagesPathsDBProvider() = default;

  virtual ImageRecordsSet& get_images_db() = 0;

  virtual std::string get_db_path() = 0;

  virtual AnnotationsList get_available_annotations() = 0;
};

using ImagesPathsDBProviderPtr = std::shared_ptr<ImagesPathsDBProvider>;

}  // namespace events::events

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_IMAGESPATHSDBPROVIDER_ABSTRACT_CLASS_H
