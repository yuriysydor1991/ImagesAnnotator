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

#include "src/helpers/ImageRecordUrlAndPathHelper.h"

#include <cassert>

#include "src/CURL/CURLController.h"
#include "src/log/log.h"

namespace helpers
{

std::string ImageRecordUrlAndPathHelper::get_ir_path(
    const ImageRecordPtr ir) const
{
  if (ir == nullptr) {
    LOGE("Invalid image record pointer provided");
    return {};
  }

  const bool absUrl = curli::CURLController::is_url(ir->abs_dir_path);
  const bool pathUrl = curli::CURLController::is_url(ir->path);

  if (absUrl && !pathUrl) {
    const std::string computedURL =
        curli::CURLController::get_absolute_url(ir->abs_dir_path, ir->path);

    LOGT("Computed URL: " << computedURL);
    return computedURL;
  } else if (absUrl && pathUrl) {
    LOGT("Two URLs provided");

    return ir->path;
  }

  LOGT("regular fs path provided");

  return ir->get_full_path();
}

bool ImageRecordUrlAndPathHelper::has_urls(const ImageRecordPtr ir) const
{
  assert(ir != nullptr);

  if (ir == nullptr) {
    LOGE("Invalid pointer provided");
    return false;
  }

  const bool absUrl = curli::CURLController::is_url(ir->abs_dir_path);
  const bool pathUrl = curli::CURLController::is_url(ir->path);

  return absUrl || pathUrl;
}

}  // namespace helpers
