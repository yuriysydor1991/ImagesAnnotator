#include "src/annotator-events/events/CurrentImageChanged.h"

#include <cassert>
#include <memory>
#include <string>

#include "src/annotator-events/events/IEvent.h"
#include "src/annotator-events/events/ImageRecord.h"
#include "src/log/log.h"

namespace events::events
{

CurrentImageChanged::CurrentImageChanged(
    std::shared_ptr<ImageRecord> setCurrentImage)
    : new_current_image{setCurrentImage}
{
  assert(new_current_image != nullptr);

  if (new_current_image == nullptr) {
    LOGE("No new current image record contained");
  }
}

}  // namespace events::events
