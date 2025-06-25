#include "src/annotator-events/events/ImagesDirProviderChanged.h"

#include <string>

#include "src/annotator-events/events/IEvent.h"
#include "src/annotator-events/events/ImagesPathsDBProvider.h"

namespace events::events
{

ImagesDirProviderChanged ::ImagesDirProviderChanged(
    std::shared_ptr<ImagesPathsDBProvider> nProvider)
    : images_provider{nProvider}
{
}

}  // namespace events::events
