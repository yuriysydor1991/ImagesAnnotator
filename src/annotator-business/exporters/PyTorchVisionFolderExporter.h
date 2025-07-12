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

#ifndef IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_PYTORCHVISIONFOLDEREXPORTER_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_PYTORCHVISIONFOLDEREXPORTER_CLASS_H

#include <filesystem>
#include <fstream>
#include <memory>
#include <unordered_map>

#include "src/annotator-business/exporters/IExporter.h"
#include "src/annotator-events/events/ImageRecordRect.h"
#include "src/annotator-events/events/ImagesPathsDBProvider.h"

namespace iannotator::exporters
{

/**
 * @brief The annotations exporter for the PyTorch Vision understandable format.
 * (Which is directories with cropped images, each in it's own category).
 */
class PyTorchVisionFolderExporter : virtual public IExporter
{
 public:
  virtual ~PyTorchVisionFolderExporter() = default;
  PyTorchVisionFolderExporter() = default;

  virtual bool export_db(ExportContextPtr ectx) override;

 private:
  using ImageRecordPtr = events::events::ImageRecordPtr;
  using ImageRecordRectPtr = events::events::ImageRecordRectPtr;
  using IImageCropperFacilityProvider =
      events::events::IImageCropperFacilityProviderPtr;

  bool check_directory(const std::filesystem::path dirPath);

  bool export_rects(ImageRecordPtr& ir, IImageCropperFacilityProvider cropper,
                    const std::filesystem::path& exportPath,
                    const std::filesystem::path& imageOrigPath);
};

}  // namespace iannotator::exporters

#endif  // IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_PYTORCHVISIONFOLDEREXPORTER_CLASS_H
