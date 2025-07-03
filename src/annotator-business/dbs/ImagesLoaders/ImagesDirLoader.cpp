#include "src/annotator-business/dbs/ImagesLoaders/ImagesDirLoader.h"

#include <algorithm>
#include <cassert>
#include <filesystem>

#include "src/log/log.h"

namespace iannotator::dbs::images
{

ImagesDirLoader::ImageRecordsSet ImagesDirLoader::load(
    const std::string& newPath)
{
  LOGD("Trying to load the directory: " << newPath);

  assert(!newPath.empty());

  if (newPath.empty()) {
    LOGE("No path provided");
    return {};
  }

  ImageRecordsSet images_found;

  fs::path current_path = newPath;

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

    images_found.insert(create_record(entry, newPath));
  }

  return images_found;
}

std::shared_ptr<ImagesDirLoader::ImageRecord> ImagesDirLoader::create_record(
    const fs::path& npath, const std::string& newAbs)
{
  auto pstr = npath.string();

  auto absI =
      std::search(pstr.begin(), pstr.end(), newAbs.begin(), newAbs.end());

  if (absI == pstr.begin() && pstr.size() > newAbs.size()) {
    LOGT("Erasing abs path from the string with slash");
    pstr.erase(pstr.begin(),
               pstr.begin() + static_cast<long>(newAbs.size() + 1U));
  }

  LOGT("Creating a new image record with rel: " << pstr);

  return std::make_shared<ImageRecord>(pstr, newAbs);
}

bool ImagesDirLoader::is_image(const fs::path& tpath)
{
  static const std::unordered_set<std::string> extsDB{
      ".png", ".jpg",  ".jpeg", ".webp", ".jpe", ".jp2", ".avif", ".bmp",
      ".dib", ".pbm",  ".pgm",  ".ppm",  ".pxm", ".pnm", ".pfm",  ".sr",
      ".ras", ".tiff", ".tif",  ".exr",  ".hdr", ".pic", ".gif"};

  const auto& pathE = tpath.extension().string();

  return extsDB.find(pathE) != extsDB.end();
}

}  // namespace iannotator::dbs::images
