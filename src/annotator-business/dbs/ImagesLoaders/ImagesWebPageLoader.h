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

#ifndef IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_IMAGESWEBPAGELOADER_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_IMAGESWEBPAGELOADER_CLASS_H

#include <memory>
#include <regex>
#include <string>

#include "src/annotator-business/dbs/ImagesLoaders/IImagesLoader.h"
#include "src/annotator-events/events/ImageRecord.h"
#include "src/helpers/SortHelper.h"

namespace iannotator::dbs::images
{

/**
 * @brief The annotator images list loader from a web page controller.
 */
class ImagesWebPageLoader : virtual public helpers::SortHelper,
                            virtual public IImagesLoader
{
 public:
  using ImagesWebPageLoaderPtr = std::shared_ptr<ImagesWebPageLoader>;
  using ImageRecordPtr = IImagesLoader::ImageRecordPtr;

  virtual ~ImagesWebPageLoader() = default;
  ImagesWebPageLoader() = default;

  virtual ImageRecordsSet load(const std::string& newPath) override;

  static ImagesWebPageLoaderPtr create();

 private:
  using download_buffer = std::vector<char>;

  static ImageRecordsSet fetch_images_urls(const std::string& origPage,
                                           const download_buffer& webpage);

  static ImageRecordPtr create_ir(const std::string& origPage,
                                  const std::string& suburl);

  inline static constexpr const ImageRecordsSet::size_type DEFAULT_RESERVE =
      1024U;
  inline static const std::regex imgre{
      R"(<\s*img[^>]*\s+src\s*=\s*(['"])(.*?)\1)",
      std::regex_constants::ECMAScript | std::regex_constants::icase};
  inline static const download_buffer::size_type URL_INDEX = 2U;
};

using ImagesWebPageLoaderPtr = ImagesWebPageLoader::ImagesWebPageLoaderPtr;

}  // namespace iannotator::dbs::images

#endif  // IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_IMAGESWEBPAGELOADER_CLASS_H
