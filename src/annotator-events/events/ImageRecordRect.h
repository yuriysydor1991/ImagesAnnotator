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

#ifndef IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_EVENTS_IMAGERECORDRECT_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_EVENTS_IMAGERECORDRECT_CLASS_H

#include <memory>
#include <string>
#include <unordered_set>

#include "src/annotator-events/events/IRecord.h"

namespace events::events
{

/**
 * @brief The particular image rectangular record class.
 */
class ImageRecordRect : virtual public IRecord,
                        std::enable_shared_from_this<ImageRecordRect>
{
 public:
  using ImageRecordRectPtr = std::shared_ptr<ImageRecordRect>;
  using ImageRecordRectSet = std::unordered_set<ImageRecordRectPtr>;

  virtual ~ImageRecordRect() = default;
  ImageRecordRect();
  ImageRecordRect(const std::string& nname, const int& nx, const int& ny,
                  const int& nwidth, const int& nheight);

  std::string name;

  int x{0};
  int y{0};
  int width{0};
  int height{0};

  ImageRecordRectPtr duplicate_shared() const;

  static bool equal(const ImageRecordRectSet& l, const ImageRecordRectSet& r);
  static bool equal(const ImageRecordRectPtr& l, const ImageRecordRectPtr& r);
};

using ImageRecordRectPtr = ImageRecordRect::ImageRecordRectPtr;
using ImageRecordRectSet = ImageRecordRect::ImageRecordRectSet;

}  // namespace events::events

#endif  // IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_EVENTS_IMAGERECORDRECT_CLASS_H
