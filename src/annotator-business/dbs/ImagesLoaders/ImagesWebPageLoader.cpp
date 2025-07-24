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

#include "src/annotator-business/dbs/ImagesLoaders/ImagesWebPageLoader.h"

#include <cassert>
#include <regex>
#include <string>

#include "src/CURL/CURLController.h"
#include "src/log/log.h"

namespace iannotator::dbs::images
{

ImagesWebPageLoader::ImageRecordsSet ImagesWebPageLoader::load(
    const std::string& newPath)
{
  assert(!newPath.empty());

  if (newPath.empty()) {
    LOGE("Empty path provided");
    return {};
  }

  curli::CURLControllerPtr curl = curli::CURLController::create();

  assert(curl != nullptr);

  const auto& webpage = curl->download(newPath);

  if (webpage.empty()) {
    LOGW("Downloaded an empty page by URL: " << newPath);
    return {};
  }

  auto iurls = fetch_images_urls(newPath, webpage);

  if (iurls.empty()) {
    LOGD("No images found");
    return {};
  }

  return iurls;
}

ImagesWebPageLoader::ImageRecordsSet ImagesWebPageLoader::fetch_images_urls(
    const std::string& origPage, const download_buffer& webpage)
{
  if (webpage.empty()) {
    return {};
  }

  ImageRecordsSet iurls;

  iurls.reserve(DEFAULT_RESERVE);

  std::match_results<download_buffer::const_iterator> mr;

  auto siter = webpage.cbegin();

  while (siter < webpage.end() &&
         std::regex_search(siter, webpage.end(), mr, imgre)) {
    if (mr.size() <= URL_INDEX) {
      LOGT("No match for URL component of the regex");
      continue;
    }

    auto ir = create_ir(origPage, mr[URL_INDEX]);

    assert(ir != nullptr);

    LOGT("Found img URL: " << ir->path);

    iurls.emplace_back(ir);

    siter += mr.position(URL_INDEX);
    siter += static_cast<download_buffer::difference_type>(ir->path.size());
  }

  LOGT("Found " << iurls.size() << " img html tag matches");

  return iurls;
}

ImagesWebPageLoader::ImageRecordPtr ImagesWebPageLoader::create_ir(
    const std::string& origPage, const std::string& suburl)
{
  auto ir = ImageRecord::create(suburl, origPage);

  assert(ir != nullptr);

  return ir;
}

ImagesWebPageLoader::ImagesWebPageLoaderPtr ImagesWebPageLoader::create()
{
  return std::make_shared<ImagesWebPageLoader>();
}

}  // namespace iannotator::dbs::images
