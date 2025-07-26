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

#ifndef IMAGES_ANNOTATOR_PROJECT_IMAGERECORDURLANDPATHHELPER_HELPER_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_IMAGERECORDURLANDPATHHELPER_HELPER_CLASS_H

#include "src/annotator-events/events/ImageRecord.h"
#include "src/helpers/IHelper.h"

namespace helpers
{

/**
 * @brief The helper class to compute the ImageRecord appropriate paths.
 */
class ImageRecordUrlAndPathHelper : virtual public IHelper
{
 public:
  using ImageRecordPtr = events::events::ImageRecordPtr;

  virtual ~ImageRecordUrlAndPathHelper() = default;
  ImageRecordUrlAndPathHelper() = default;

  /**
   * @brief May return fs path or URL to an image at Web.
   */
  static std::string get_ir_path(const ImageRecordPtr ir);

  static bool has_urls(const ImageRecordPtr ir);
};

}  // namespace helpers

#endif  // IMAGES_ANNOTATOR_PROJECT_IMAGERECORDURLANDPATHHELPER_HELPER_CLASS_H
