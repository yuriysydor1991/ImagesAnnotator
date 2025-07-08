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

#ifndef IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_IMAGES_DB_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_IMAGES_DB_CLASS_H

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

#endif  // IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_IMAGES_DB_CLASS_H
