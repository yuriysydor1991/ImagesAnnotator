#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_IMAGES_DB_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_IMAGES_DB_CLASS_H

#include <filesystem>
#include <string>
#include <unordered_set>

#include "src/annotator-events/events/ImageRecord.h"
#include "src/helpers/SortHelper.h"

namespace iannotator::dbs::images
{

namespace fs = std::filesystem;

/**
 * @brief The annotator images dir db controller.
 */
class ImagesDirLoader : virtual public helpers::SortHelper
{
 public:
  using ImageRecord = events::events::ImageRecord;
  using ImageRecordsSet = events::events::ImageRecordsSet;
  using ImageRecordPtr = events::events::ImageRecordPtr;

  virtual ~ImagesDirLoader() = default;
  ImagesDirLoader() = default;

  virtual ImageRecordsSet load(const std::string& newPath);

 protected:
  virtual bool is_image(const fs::path& tpath);

  virtual std::shared_ptr<ImageRecord> create_record(const fs::path& npath,
                                                     const std::string& newAbs);
};

}  // namespace iannotator::dbs::images

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_IMAGES_DB_CLASS_H
