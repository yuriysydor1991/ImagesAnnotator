#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_IMAGES_DB_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_IMAGES_DB_CLASS_H

#include <filesystem>
#include <string>
#include <unordered_set>

#include "src/annotator-events/events/ImageRecord.h"
#include "src/annotator-events/events/ImagesPathsDBProvider.h"

namespace iannotator::dbs::images
{

namespace fs = std::filesystem;

/**
 * @brief The annotator images dir db controller.
 */
class ImagesDirDB : virtual public events::events::ImagesPathsDBProvider
{
 public:
  using ImagesPathsDBProvider = events::events::ImagesPathsDBProvider;
  using ImagesDB = ImagesPathsDBProvider::ImagesDB;
  using ImageRecord = events::events::ImageRecord;

  virtual ~ImagesDirDB() = default;
  ImagesDirDB() = default;

  virtual bool load_directory(const std::string& newPath);

  virtual ImagesDB& get_images_db() override;

 protected:
  virtual bool is_image(const fs::path& tpath);

  virtual std::shared_ptr<ImageRecord> create_record(const fs::path& npath);

  fs::path current_path;
  ImagesDB images_found;
};

}  // namespace iannotator::dbs::images

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_IMAGES_DB_CLASS_H
