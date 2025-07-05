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
class ImageRecordRect : virtual public IRecord,
                        std::enable_shared_from_this<ImageRecordRect>
{
 public:
  using ImageRecordRectPtr = std::shared_ptr<ImageRecordRect>;

  virtual ~ImageRecordRect() = default;
  ImageRecordRect();
  ImageRecordRect(const std::string& nname, const int& nx, const int& ny,
                  const int& nwidth, const int& nheight);

  std::string name;

  int x{0};
  int y{0};
  int width{0};
  int height{0};

  ImageRecordRectPtr duplicate_shared();
};

using ImageRecordRectPtr = ImageRecordRect::ImageRecordRectPtr;
using ImageRecordRectSet = std::unordered_set<ImageRecordRectPtr>;

}  // namespace events::events

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_EVENTS_IMAGERECORDRECT_CLASS_H
