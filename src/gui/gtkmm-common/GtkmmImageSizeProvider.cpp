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

#include "src/gui/gtkmm-common/GtkmmImageSizeProvider.h"

#include <gdk-pixbuf/gdk-pixbuf.h>

#include <memory>
#include <string>

#include "src/log/log.h"

namespace imagesAnnotatorGtkmmCommon
{

bool GtkmmImageSizeProvider::read_image_size(const std::string& imagePath,
                                             int& width, int& height)
{
  if (imagePath.empty()) {
    LOGE("Empty image path provided");
    return false;
  }

  int readWidth{0};
  int readHeight{0};

  // The gdk-pixbuf C entry point rather than a gdkmm one: gdkmm3 wraps it as
  // Gdk::Pixbuf::get_file_info while gdkmm4 dropped that wrapper, and this
  // component is compiled once against whichever of the two is active. It
  // parses the image header only and decodes no pixel at all, which is exactly
  // what an import needs: it is measuring, not displaying. The returned format
  // description is owned by gdk-pixbuf and must not be freed.
  const GdkPixbufFormat* const format =
      gdk_pixbuf_get_file_info(imagePath.c_str(), &readWidth, &readHeight);

  if (format == nullptr) {
    LOGE("Fail to read the image info: " << imagePath);
    return false;
  }

  if (readWidth <= 0 || readHeight <= 0) {
    LOGE("The image carries no area: " << imagePath);
    return false;
  }

  width = readWidth;
  height = readHeight;

  LOGT("Measured " << imagePath << " as " << width << "x" << height);

  return true;
}

GtkmmImageSizeProvider::IImageSizeFacilityProviderPtr
GtkmmImageSizeProvider::clone()
{
  return std::make_shared<GtkmmImageSizeProvider>();
}

}  // namespace imagesAnnotatorGtkmmCommon
