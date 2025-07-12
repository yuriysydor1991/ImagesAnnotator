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

#include "src/annotator-business/exporters/PyTorchVisionFolderExporter.h"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <memory>

#include "src/annotator-business/exporters/IExporter.h"
#include "src/annotator-events/events/ImageRecordRect.h"
#include "src/annotator-events/events/ImagesPathsDBProvider.h"
#include "src/log/log.h"

namespace iannotator::exporters
{

namespace
{
namespace fs = std::filesystem;
}

bool PyTorchVisionFolderExporter::export_db(ExportContextPtr ectx)
{
  assert(ectx != nullptr);
  assert(ectx->cropper != nullptr);
  assert(!ectx->export_path.empty());

  if (ectx == nullptr) {
    LOGE("Invalid export context pointer provided");
    return false;
  }

  if (ectx->export_path.empty()) {
    LOGE("No dst folder export path given");
    return false;
  }

  if (ectx->cropper == nullptr) {
    LOGE("No image cropper provided");
    return false;
  }

  auto irs = ectx->dbProvider->get_images_db();

  for (auto& ir : irs) {
    assert(ir != nullptr);

    if (ir == nullptr) {
      LOGE("Invalid image record pointer provided");
      continue;
    }

    const fs::path origfs = ir->get_full_path();

    assert(!origfs.empty());

    if (origfs.empty()) {
      LOGE("Image with an empty path");
      continue;
    }

    if (ir->rects.empty()) {
      LOGT("No rects to crop out");
      continue;
    }

    if (!export_rects(ir, ectx->cropper, ectx->export_path, origfs)) {
      LOGE("Fail to crop out rects for image: " << ir->get_full_path());
      continue;
    }
  }

  return true;
}

bool PyTorchVisionFolderExporter::export_rects(
    ImageRecordPtr& ir, IImageCropperFacilityProvider cropper,
    const std::filesystem::path& exportPath,
    const std::filesystem::path& imageOrigPath)
{
  assert(ir != nullptr);
  assert(cropper != nullptr);

  for (auto& irr : ir->rects) {
    assert(irr != nullptr);

    if (irr == nullptr) {
      LOGE("Invalid rect pointer in the queue");
      continue;
    }

    if (irr->name.empty()) {
      LOGT("Tag name is empty for " << imageOrigPath.string());
      continue;
    }

    fs::path cTagDir = exportPath / irr->name;

    if (!check_directory(cTagDir)) {
      LOGE("Failure while checking the directory: " << cTagDir.string());
      continue;
    }

    fs::path nipath = cTagDir / imageOrigPath.filename();

    // string path may be altered
    std::string spath = nipath.string();

    if (!cropper->crop_out_2_fs(ir, irr, spath)) {
      LOGE("Failure during image cropping out");
      continue;
    }
  }

  return true;
}

bool PyTorchVisionFolderExporter::check_directory(
    const std::filesystem::path dirPath)
{
  if (fs::is_directory(dirPath)) {
    LOGT("Directory already present " << dirPath.string());
    return true;
  }

  try {
    if (!fs::create_directory(dirPath)) {
      LOGE("Failure during the directory creation: " << dirPath.string());
      return false;
    }
  }
  catch (const std::exception& e) {
    LOGE("Failure during directory creation " << e.what());
    return false;
  }

  return true;
}

}  // namespace iannotator::exporters
