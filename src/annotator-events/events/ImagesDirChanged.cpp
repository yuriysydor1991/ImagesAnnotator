#include "src/annotator-events/events/ImagesDirChanged.h"

#include <string>

#include "src/annotator-events/events/IEvent.h"

namespace events::events
{

ImagesDirChanged::ImagesDirChanged(const std::string& newPath)
    : images_dir{newPath}
{
}

}  // namespace events::events
