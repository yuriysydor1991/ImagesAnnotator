#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_IMAGERECORD_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_IMAGERECORD_CLASS_H

#include <string>

#include "src/annotator-events/events/IRecord.h"

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
};

}  // namespace events::events

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_IMAGERECORD_CLASS_H
