#include "src/annotator-business/dbs/ImagesDBs/ImagesDirDB.h"

#include <cassert>
#include <filesystem>

#include "src/log/log.h"

namespace iannotator::dbs
{

bool ImagesDirDB::load_directory(const std::string& newPath)
{
  LOGD("Trying to load the directory: " << newPath);

  assert(!newPath.empty());

  if (newPath.empty()) {
    LOGE("No path provided");
    return false;
  }

  images_found.clear();

  current_path = newPath;

  for (const auto& entry : fs::recursive_directory_iterator(current_path)) {
    LOGT("Found entry: " << entry.path().string());

    if (entry.is_directory()) {
      LOGT("it's directory: " << entry.path().string());
      continue;
    }

    if (!is_image(entry)) {
      LOGT("Not an image: " << entry);
      continue;
    }

    LOGT("Image found: " << entry.path().string());

    images_found.insert(create_record(entry));
  }

  return true;
}

std::shared_ptr<ImagesDirDB::ImageRecord> ImagesDirDB::create_record(
    const fs::path& npath)
{
  return std::make_shared<ImageRecord>(npath.string());
}

bool ImagesDirDB::is_image(const fs::path& tpath)
{
  static const std::unordered_set<std::string> extsDB{
      ".png", ".jpg",  ".jpeg", ".webp", ".jpe", ".jp2", ".avif", ".bmp",
      ".dib", ".pbm",  ".pgm",  ".ppm",  ".pxm", ".pnm", ".pfm",  ".sr",
      ".ras", ".tiff", ".tif",  ".exr",  ".hdr", ".pic", ".gif"};

  const auto& pathE = tpath.extension().string();

  return extsDB.find(pathE) != extsDB.end();
}

ImagesDirDB::ImagesDB& ImagesDirDB::get_images_db() { return images_found; }

}  // namespace iannotator::dbs
