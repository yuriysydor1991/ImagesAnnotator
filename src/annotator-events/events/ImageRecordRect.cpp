#include "src/annotator-events/events/ImageRecordRect.h"

#include <memory>
#include <string>
#include <unordered_set>

#include "src/annotator-events/events/IRecord.h"

namespace events::events
{

ImageRecordRect::ImageRecordRect() {}

ImageRecordRect::ImageRecordRect(const std::string& nname, const int& nx,
                                 const int& ny, const int& nwidth,
                                 const int& nheight)
    : name{nname}, x{nx}, y{ny}, width{nwidth}, height{nheight}
{
}

ImageRecordRectPtr ImageRecordRect::duplicate_shared()
{
  return std::make_shared<ImageRecordRect>(*this);
}

}  // namespace events::events
