#include "src/annotator-events/events/AnnotationsDirChanged.h"

#include <string>

#include "src/annotator-events/events/IEvent.h"

namespace events::events
{

AnnotationsDirChanged::AnnotationsDirChanged(const std::string& newPath)
    : annotations_dir{newPath}
{
}

}  // namespace events::events
