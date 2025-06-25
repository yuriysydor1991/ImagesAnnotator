#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_IMAGESDIRPROVIDERCHANGED_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_IMAGESDIRPROVIDERCHANGED_CLASS_H

#include <string>

#include "src/annotator-events/events/IEvent.h"
#include "src/annotator-events/events/ImagesPathsDBProvider.h"

namespace events::events
{

/**
 * @brief The images dir changed event.
 */
class ImagesDirProviderChanged : virtual public IEvent
{
 public:
  virtual ~ImagesDirProviderChanged() = default;
  ImagesDirProviderChanged(std::shared_ptr<ImagesPathsDBProvider> nProvider);

  std::shared_ptr<ImagesPathsDBProvider> images_provider;
};

}  // namespace events::events

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_IMAGESDIRPROVIDERCHANGED_CLASS_H
