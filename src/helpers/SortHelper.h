#ifndef IMAGES_ANNOTATOR_PROJECT_SORT_HELPER_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_SORT_HELPER_CLASS_H

#include <functional>

#include "src/annotator-events/events/ImageRecord.h"
#include "src/helpers/IHelper.h"

namespace helpers
{

/**
 * @brief The helper class to encapsulate the sorting methods.
 */
class SortHelper : virtual public IHelper
{
 public:
  using ImageRecordPtr = events::events::ImageRecordPtr;
  using ImageRecordSorterFcn =
      std::function<bool(const ImageRecordPtr& l, const ImageRecordPtr& r)>;

  virtual ~SortHelper() = default;
  SortHelper() = default;

  static ImageRecordSorterFcn get_image_records_sorter();
};

}  // namespace helpers

#endif  // IMAGES_ANNOTATOR_PROJECT_SORT_HELPER_CLASS_H
