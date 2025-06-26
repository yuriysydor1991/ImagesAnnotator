#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_IMAGERECORD_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_IMAGERECORD_CLASS_H

#include <string>
#include <vector>

#include "src/annotator-events/events/IRecord.h"
#include "src/annotator-events/events/ImageRecordRect.h"

namespace events::events
{

/**
 * @brief The particular image record class.
 */
class ImageRecord : virtual public IRecord
{
 public:
  virtual ~ImageRecord() = default;
  ImageRecord(const std::string& npath = "");

  std::string path;

  ImageRecordRectSet rects;
};

}  // namespace events::events

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_IMAGERECORD_CLASS_H
