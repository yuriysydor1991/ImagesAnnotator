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

#ifndef IMAGES_ANNOTATOR_PROJECT_QT6_QTIMAGECROPPERPROVIDER_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_QT6_QTIMAGECROPPERPROVIDER_CLASS_H

#include <filesystem>
#include <memory>
#include <string>

#include "src/annotator-events/events/IImageCropperFacilityProvider.h"
#include "src/annotator-events/events/ImageRecord.h"
#include "src/annotator-events/events/ImageRecordRect.h"

class QImage;

namespace templateQt6::window::custom_widgets
{

/**
 * @brief QImage based IImageCropperFacilityProvider implementation.
 *
 * The Qt analogue of templateGtkmmCommon::GtkmmImageCropperProvider and of the
 * wxWidgets WxImageCropperProvider: the PyTorch Vision exporter injects it so
 * the toolkit-agnostic export code can crop the annotated rectangle regions out
 * of the source images without knowing which GUI toolkit is active.
 */
class QtImageCropperProvider
    : virtual public events::events::IImageCropperFacilityProvider,
      public std::enable_shared_from_this<QtImageCropperProvider>
{
 public:
  using ImageRecordPtr = events::events::ImageRecordPtr;
  using ImageRecordRectPtr = events::events::ImageRecordRectPtr;
  /// @brief The base class names it IImageCropperFacilityPtr - it is the
  /// ImagesAnnotator-DataExporters interface now. Kept under the project name.
  using IImageCropperFacilityProviderPtr =
      events::events::IImageCropperFacilityProviderPtr;

  virtual ~QtImageCropperProvider() = default;
  QtImageCropperProvider() = default;

  virtual bool crop_out_2_fs(ImageRecordPtr ir, ImageRecordRectPtr irr,
                             std::string& tofpath) override;

  virtual IImageCropperFacilityProviderPtr clone() override;

 private:
  bool load_and_crop(QImage& crop, ImageRecordPtr ir, ImageRecordRectPtr irr);
  bool save_crop(QImage& crop, std::string& tofpath);

  std::string get_ext_only(const std::filesystem::path& fspath);
};

}  // namespace templateQt6::window::custom_widgets

#endif  // IMAGES_ANNOTATOR_PROJECT_QT6_QTIMAGECROPPERPROVIDER_CLASS_H
