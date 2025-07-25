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

#ifndef IMAGES_ANNOTATOR_TEMPLATE_PROJECT_IMAGELOADER_CLASS_H
#define IMAGES_ANNOTATOR_TEMPLATE_PROJECT_IMAGELOADER_CLASS_H

#include <filesystem>
#include <memory>
#include <string>

#include "project-global-decls.h"
#include "src/CURL/CURLController.h"
#include "src/annotator-events/events/ImageRecord.h"
#include "src/helpers/ImageRecordUrlAndPathHelper.h"

namespace helpers
{

/**
 * @brief The net image loader class. For images list from the webpages
 * etc.
 */
class ImageLoader : public std::enable_shared_from_this<ImageLoader>,
                    virtual public ::helpers::ImageRecordUrlAndPathHelper
{
 public:
  using ImageLoaderPtr = std::shared_ptr<ImageLoader>;
  using ImageRecordPtr = events::events::ImageRecordPtr;
  using download_buffer = curli::CURLController::download_buffer;
  using CURLControllerPtr = curli::CURLControllerPtr;

  virtual ~ImageLoader() = default;
  ImageLoader();

  bool load(ImageRecordPtr ir);

  static ImageLoaderPtr create();

 private:
  inline static const std::string app_folder_name =
      project_decls::PROJECT_NAME + "-" + project_decls::PROJECT_BUILD_VERSION;
  inline static const std::string preloads_folder_name = "preloads";
  inline static constexpr const char* const defaultFSDateFormat =
      "%Y.%m.%d_%H.%M.%S";

  std::filesystem::path get_tmp_path();
  std::string get_fs_timestamp();
  std::filesystem::path get_net_ir_filepath(ImageRecordPtr ir);
  bool write_data(ImageRecordPtr ir, const download_buffer& irdata);
  bool create_file(const std::filesystem::path& irfpath);
  std::filesystem::path get_directory_path(ImageRecordPtr ir);
  bool create_directories(const std::filesystem::path& irdpath);
  std::filesystem::path generate_new_filepath(
      const ImageRecordPtr ir, const std::filesystem::path& irdpath);

  CURLControllerPtr curl;
};

using ImageLoaderPtr = ImageLoader::ImageLoaderPtr;

}  // namespace helpers

#endif  // IMAGES_ANNOTATOR_TEMPLATE_PROJECT_IMAGELOADER_CLASS_H
