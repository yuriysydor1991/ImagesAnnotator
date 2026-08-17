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

#ifndef IMAGES_ANNOTATOR_TEMPLATE_PROJECT_GTKMMIMAGESIZEPROVIDER_CLASS_H
#define IMAGES_ANNOTATOR_TEMPLATE_PROJECT_GTKMMIMAGESIZEPROVIDER_CLASS_H

#include <memory>
#include <string>

#include "src/annotator-events/events/IImageSizeFacilityProvider.h"

// Toolkit-agnostic: probes the image header through the gdk-pixbuf C entry
// point, which both gdkmm-3.0 and gdkmm-4.0 build on, so it lives in
// gtkmm-common and is compiled once against whichever GTKmm toolkit is active.
namespace imagesAnnotatorGtkmmCommon
{

/**
 * @brief The IImageSizeFacilityProvider facility interface implementation.
 *
 * The counterpart of GtkmmImageCropperProvider on the import side: the
 * ImagesAnnotator-DataImporters library decodes no image format of its own, so
 * the imports whose annotations are not stored in the pixels of their image ask
 * the application to measure the pictures.
 */
class GtkmmImageSizeProvider
    : virtual public events::events::IImageSizeFacilityProvider,
      public std::enable_shared_from_this<GtkmmImageSizeProvider>
{
 public:
  /// @brief The base class names it IImageSizeFacilityPtr - it is the
  /// ImagesAnnotator-DataImporters interface now. Kept under the project name.
  using IImageSizeFacilityProviderPtr =
      events::events::IImageSizeFacilityProviderPtr;

  virtual ~GtkmmImageSizeProvider() = default;
  GtkmmImageSizeProvider() = default;

  /**
   * @brief Reads the intrinsic pixel size of the given image file.
   *
   * Only the image header is parsed - no pixel data is decoded - so measuring
   * a whole dataset directory costs no more than reading its first bytes.
   *
   * @param imagePath The filesystem path of the image to measure.
   * @param width Out: the image width in pixels, left alone on a failure.
   * @param height Out: the image height in pixels, left alone on a failure.
   *
   * @return Returns true when both dimensions were read.
   */
  virtual bool read_image_size(const std::string& imagePath, int& width,
                               int& height) override;

  virtual IImageSizeFacilityProviderPtr clone() override;
};

}  // namespace imagesAnnotatorGtkmmCommon

#endif  // IMAGES_ANNOTATOR_TEMPLATE_PROJECT_GTKMMIMAGESIZEPROVIDER_CLASS_H
