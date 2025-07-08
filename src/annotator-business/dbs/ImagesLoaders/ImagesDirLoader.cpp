/**
 * The "Simplified BSD License"
 *
 * Copyright (c) 2025, Yurii Sydor (yuriysydor1991@gmail.com)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   - Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

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

  images_found.reserve(1024U);

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

    images_found.emplace_back(create_record(entry, newPath));
  }

  std::sort(images_found.begin(), images_found.end(),
            get_image_records_sorter());

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
