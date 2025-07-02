#include "src/annotator-events/events/ImageRecord.h"

#include <string>

#include "src/annotator-events/events/IRecord.h"

namespace events::events
{

ImageRecord::ImageRecord(const std::string& npath, const std::string& nabspath)
    : abs_dir_path{nabspath}, path{npath}
{
}

void ImageRecord::scaleStepIn() { imageScale += defaultScaleStep; }

void ImageRecord::scaleStepOut() { imageScale -= defaultScaleStep; }

std::string ImageRecord::get_full_path()
{
  return abs_dir_path.empty() ? path : abs_dir_path + "/" + path;
}

}  // namespace events::events
