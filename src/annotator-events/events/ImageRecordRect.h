#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_IMAGERECORDRECT_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_IMAGERECORDRECT_CLASS_H

#include <memory>
#include <string>
#include <unordered_set>

#include "src/annotator-events/events/IRecord.h"

namespace events::events
{

/**
 * @brief The particular image rectangular record class.
 */
class ImageRecordRect : virtual public IRecord
{
 public:
  virtual ~ImageRecordRect() = default;
  ImageRecordRect() = default;

  std::string name;

  int x{0};
  int y{0};
  int width{0};
  int height{0};
};

using ImageRecordRectSet = std::unordered_set<std::shared_ptr<ImageRecordRect>>;

}  // namespace events::events

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_IMAGERECORDRECT_CLASS_H
