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

#include "src/gui/wxwidgets/main-window/custom-widgets/WxImageSizeProvider.h"

#include <wx/image.h>
#include <wx/log.h>
#include <wx/string.h>

#include <memory>
#include <string>

#include "src/log/log.h"

namespace imagesAnnotatorWx::window::custom_widgets
{

bool WxImageSizeProvider::read_image_size(const std::string& imagePath,
                                          int& width, int& height)
{
  if (imagePath.empty()) {
    LOGE("Empty image path provided");
    return false;
  }

  // wxWidgets pops a message box of its own for a file it cannot read, which
  // an import walking a whole dataset directory must not do: an unreadable
  // entry costs a log line here and nothing else.
  wxLogNull noPopups;

  wxImage image;

  if (!image.LoadFile(wxString::FromUTF8(imagePath))) {
    LOGE("Fail to read the image info: " << imagePath);
    return false;
  }

  if (image.GetWidth() <= 0 || image.GetHeight() <= 0) {
    LOGE("The image carries no area: " << imagePath);
    return false;
  }

  width = image.GetWidth();
  height = image.GetHeight();

  LOGT("Measured " << imagePath << " as " << width << "x" << height);

  return true;
}

WxImageSizeProvider::IImageSizeFacilityProviderPtr WxImageSizeProvider::clone()
{
  return std::make_shared<WxImageSizeProvider>();
}

}  // namespace imagesAnnotatorWx::window::custom_widgets
