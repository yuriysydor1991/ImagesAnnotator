/**
 * The "Simplified BSD License"
 *
 * Copyright (c) 2026, Yurii Sydor (yuriysydor1991@gmail.com)
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

#include "src/gui/qt6/main-window/custom-widgets/ImagePathLabel.h"

#include <cassert>

#include "src/helpers/ImageRecordUrlAndPathHelper.h"
#include "src/log/log.h"

namespace templateQt6::window::custom_widgets
{

ImagePathLabel::ImagePathLabel(std::shared_ptr<ImageRecord> nr) : myrec{nr}
{
  update_text();
}

void ImagePathLabel::update_text()
{
  assert(myrec != nullptr);
  assert(!myrec->path.empty());

  if (myrec == nullptr) {
    LOGE("No valid image record provided");
    return;
  }

  if (myrec->path.empty()) {
    LOGE("Image db record contains no path");
    return;
  }

  text = helpers::ImageRecordUrlAndPathHelper::get_ir_path(myrec);

  LOGT("My path: " << text);
}

std::shared_ptr<ImagePathLabel::ImageRecord> ImagePathLabel::get_image_rec()
{
  return myrec;
}

const std::string& ImagePathLabel::get_text() const { return text; }

bool ImagePathLabel::has_records() const { return hasRecords; }

bool ImagePathLabel::mark_as_has_records()
{
  hasRecords = true;
  return true;
}

bool ImagePathLabel::remove_has_records_mark()
{
  hasRecords = false;
  return true;
}

}  // namespace templateQt6::window::custom_widgets
