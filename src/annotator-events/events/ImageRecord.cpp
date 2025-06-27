#include "src/annotator-events/events/ImageRecord.h"

#include <string>

#include "src/annotator-events/events/IRecord.h"

namespace events::events
{

ImageRecord::ImageRecord(const std::string& npath) : path{npath} {}

void ImageRecord::scaleStepIn() { imageScale += defaultScaleStep; }

void ImageRecord::scaleStepOut() { imageScale -= defaultScaleStep; }

}  // namespace events::events
