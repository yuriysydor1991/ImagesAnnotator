#include "src/annotator-events/events/ImageRecord.h"

#include <string>

#include "src/annotator-events/events/IRecord.h"
#include "src/log/log.h"

namespace events::events
{

ImageRecord::ImageRecord(const std::string& npath, const std::string& nabspath)
    : abs_dir_path{nabspath}, path{npath}
{
}

void ImageRecord::scaleStepIn() { imageScale += defaultScaleStep; }

void ImageRecord::scaleStepOut() { imageScale -= defaultScaleStep; }

std::string ImageRecord::get_full_path() const
{
  return abs_dir_path.empty() ? path : abs_dir_path + "/" + path;
}

bool ImageRecord::erase_current_rect()
{
  if (current_rect == nullptr) {
    LOGT("No current rect avaiable");
    return false;
  }

  auto rIter = rects.find(current_rect);

  if (rIter == rects.end()) {
    LOGE("Current rect is not in the rects queue");
    return false;
  }

  rects.erase(rIter);
  current_rect.reset();

  return true;
}

}  // namespace events::events
