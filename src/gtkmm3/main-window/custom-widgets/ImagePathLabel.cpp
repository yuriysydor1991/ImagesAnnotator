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

#include "src/gtkmm3/main-window/custom-widgets/ImagePathLabel.h"

#include <cassert>
#include <memory>

#include "src/CURL/CURLController.h"
#include "src/annotator-events/events/ImageRecord.h"
#include "src/gtkmm3/gtkmm_includes.h"
#include "src/gtkmm3/main-window/WindowDataContext.h"
#include "src/log/log.h"

namespace templateGtkmm3::window::custom_widgets
{

ImagePathLabel::ImagePathLabel(const std::shared_ptr<ImageRecord> nr)
    : myrec{nr}
{
  update_text();

  set_ellipsize(Pango::ELLIPSIZE_START);
  set_single_line_mode(true);
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
    LOGW("Image db record contains no path");
    return;
  }

  LOGT("My path: " << myrec->get_full_path());

  const auto& abs = myrec->abs_dir_path;

  const bool absUrl = curli::CURLController::is_url(abs);
  const bool pathUrl = curli::CURLController::is_url(myrec->path);

  if (absUrl && !pathUrl) {
    const std::string computedURL =
        curli::CURLController::get_absolute_url(abs, myrec->path);

    LOGT("Computed URL: " << computedURL);

    set_text(computedURL);
  } else if (absUrl && pathUrl) {
    LOGT("Two URLs provided");

    set_text(abs + " -> " + myrec->path);
  } else {
    LOGT("regular fs path provided");
    set_text(myrec->get_full_path());
  }
}

std::shared_ptr<ImagePathLabel::ImageRecord> ImagePathLabel::get_image_rec()
{
  return myrec;
}

bool ImagePathLabel::mark_as_has_records()
{
  get_style_context()->add_class(has_records_css_class);

  return true;
}

bool ImagePathLabel::remove_has_records_mark()
{
  get_style_context()->remove_class(has_records_css_class);

  return true;
}

}  // namespace templateGtkmm3::window::custom_widgets
