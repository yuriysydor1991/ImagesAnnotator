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

#include "src/gui/qt6/main-window/custom-widgets/QtImageCropperProvider.h"

#include <algorithm>
#include <cassert>

#include "src/gui/qt6/qt_includes.h"
#include "src/log/log.h"

namespace imagesAnnotatorQt6::window::custom_widgets
{

bool QtImageCropperProvider::crop_out_2_fs(ImageRecordPtr ir,
                                           ImageRecordRectPtr irr,
                                           std::string& tofpath)
{
  assert(ir != nullptr);
  assert(irr != nullptr);
  assert(!tofpath.empty());

  if (ir == nullptr) {
    LOGE("Image record pointer is invalid");
    return false;
  }

  if (irr == nullptr) {
    LOGE("Image rect record pointer is invalid");
    return false;
  }

  if (tofpath.empty()) {
    LOGE("New image destination path is empty");
    return false;
  }

  QImage crop;

  if (!load_and_crop(crop, ir, irr)) {
    LOGE("Fail to load and/or crop the original image");
    return false;
  }

  if (!save_crop(crop, tofpath)) {
    LOGE("Fail to save new crop: " << tofpath);
    return false;
  }

  return true;
}

bool QtImageCropperProvider::load_and_crop(QImage& crop, ImageRecordPtr ir,
                                           ImageRecordRectPtr irr)
{
  assert(ir != nullptr);
  assert(irr != nullptr);

  const std::string filepath = ir->get_full_path();

  if (filepath.empty()) {
    LOGE("Invalid filepath obtained from the image record");
    return false;
  }

  QImage source;

  if (!source.load(QString::fromUtf8(
          filepath.c_str(), static_cast<qsizetype>(filepath.size())))) {
    LOGE("Failed to load the image: " << filepath);
    return false;
  }

  const int nx = std::max(0, irr->x);
  const int ny = std::max(0, irr->y);
  const int nwidth = std::min(irr->width, source.width() - irr->x);
  const int nheight = std::min(irr->height, source.height() - irr->y);

  if (nwidth <= 0 || nheight <= 0) {
    LOGE("Degenerate crop region for the image: " << filepath);
    return false;
  }

  crop = source.copy(QRect{nx, ny, nwidth, nheight});

  return !crop.isNull();
}

bool QtImageCropperProvider::save_crop(QImage& crop, std::string& tofpath)
{
  static const std::string extPNG = "png";

  assert(!crop.isNull());
  assert(!tofpath.empty());

  const std::filesystem::path fspath{tofpath};

  const std::string ext = get_ext_only(fspath);
  const std::string extUsed = extPNG;

  if (ext != extUsed) {
    auto newfs =
        fspath.parent_path() / (fspath.stem().string() + "." + extUsed);
    tofpath = newfs.string();
  }

  if (std::filesystem::is_regular_file(tofpath)) {
    LOGT("Path already exists: " << tofpath << " creating new name");

    unsigned long long fiter{0U};
    do {
      auto newfs =
          fspath.parent_path() / (fspath.stem().string() + "-" +
                                  std::to_string(fiter++) + "." + extUsed);

      tofpath = newfs.string();

    } while (std::filesystem::is_regular_file(tofpath));

    LOGT("Created a new filename: " << tofpath);
  }

  if (!crop.save(QString::fromUtf8(tofpath.c_str(),
                                   static_cast<qsizetype>(tofpath.size())),
                 "PNG")) {
    LOGE("Failed to save the image: " << tofpath);
    return false;
  }

  return true;
}

std::string QtImageCropperProvider::get_ext_only(
    const std::filesystem::path& fspath)
{
  if (fspath.empty()) {
    LOGE("Empty path privided");
    return {};
  }

  std::string dotext = fspath.extension().string();

  if (dotext.empty()) {
    LOGT("Empty string extension retrieved");
    return {};
  }

  if (dotext[0] == '.') {
    dotext.erase(dotext.begin());
  }

  return dotext;
}

QtImageCropperProvider::IImageCropperFacilityProviderPtr
QtImageCropperProvider::clone()
{
  return std::make_shared<QtImageCropperProvider>(*this);
}

}  // namespace imagesAnnotatorQt6::window::custom_widgets
