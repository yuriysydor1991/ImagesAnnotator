#include "src/helpers/SortHelper.h"

#include <cassert>

#include "src/log/log.h"

namespace helpers
{

SortHelper::ImageRecordSorterFcn SortHelper::get_image_records_sorter()
{
  return [](const ImageRecordPtr& l, const ImageRecordPtr& r) {
    assert(l != nullptr);
    assert(r != nullptr);

    if (r == nullptr || l == nullptr) {
      LOGE("Invalid image records pointers given");
      return false;
    }

    return l->get_full_path() < r->get_full_path();
  };
}

}  // namespace helpers
