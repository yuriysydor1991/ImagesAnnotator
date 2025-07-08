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

#include "src/annotator-events/events/ImageRecord.h"

#include <algorithm>
#include <cassert>
#include <string>

#include "src/annotator-events/events/IRecord.h"
#include "src/log/log.h"

namespace events::events
{

ImageRecord::ImageRecord(const std::string& npath, const std::string& nabspath)
    : abs_dir_path{nabspath}, path{npath}
{
}

void ImageRecord::scaleStepIn() { imageScale += defaultScaleStep; }

void ImageRecord::scaleStepOut() { imageScale -= defaultScaleStep; }

std::string ImageRecord::get_full_path() const
{
  return abs_dir_path.empty() ? path : abs_dir_path + "/" + path;
}

bool ImageRecord::erase_current_rect()
{
  if (current_rect == nullptr) {
    LOGT("No current rect avaiable");
    return false;
  }

  auto rIter = rects.find(current_rect);

  if (rIter == rects.end()) {
    LOGE("Current rect is not in the rects queue");
    return false;
  }

  rects.erase(rIter);
  current_rect.reset();

  return true;
}

bool ImageRecord::equal(const ImageRecordsSet& l, const ImageRecordsSet& r)
{
  if (&l == &r) {
    return true;
  }

  if (l.size() != r.size()) {
    return false;
  }

  return std::equal(l.begin(), l.end(), r.begin(), r.end(),
                    [](const ImageRecordPtr& lir, const ImageRecordPtr& rir) {
                      return equal(lir, rir);
                    });
}

bool ImageRecord::equal(const ImageRecordPtr& l, const ImageRecordPtr& r)
{
  return l.get() == r.get() ||
         (l->abs_dir_path == r->abs_dir_path && l->path != r->path &&
          l->imageScale == r->imageScale &&
          ImageRecordRect::equal(l->rects, r->rects));
}

ImageRecordPtr ImageRecord::duplicate_shared() const
{
  auto irdup = std::make_shared<ImageRecord>(path, abs_dir_path);

  for (const auto& rect : rects) {
    irdup->rects.insert(rect->duplicate_shared());
  }

  return irdup;
}

ImageRecordsSet ImageRecord::duplicate(const ImageRecordsSet& orig)
{
  ImageRecordsSet dup;

  dup.reserve(orig.size());

  for (const auto& o : orig) {
    assert(o != nullptr);
    dup.emplace_back(o->duplicate_shared());
  }

  return dup;
}

}  // namespace events::events
