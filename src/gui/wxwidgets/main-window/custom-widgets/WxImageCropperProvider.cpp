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

#include "src/gui/wxwidgets/main-window/custom-widgets/WxImageCropperProvider.h"

#include <algorithm>
#include <cassert>

#include "src/gui/wxwidgets/wx_includes.h"
#include "src/log/log.h"

namespace imagesAnnotatorWx::window::custom_widgets
{

bool WxImageCropperProvider::crop_out_2_fs(ImageRecordPtr ir,
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

  wxImage crop;

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

bool WxImageCropperProvider::load_and_crop(wxImage& crop, ImageRecordPtr ir,
                                           ImageRecordRectPtr irr)
{
  assert(ir != nullptr);
  assert(irr != nullptr);

  const std::string filepath = ir->get_full_path();

  if (filepath.empty()) {
    LOGE("Invalid filepath obtained from the image record");
    return false;
  }

  wxImage pixbuf;

  if (!pixbuf.LoadFile(wxString::FromUTF8(filepath))) {
    LOGE("Failed to load the image: " << filepath);
    return false;
  }

  const int nx = std::max(0, irr->x);
  const int ny = std::max(0, irr->y);
  const int nwidth = std::min(irr->width, pixbuf.GetWidth() - irr->x);
  const int nheight = std::min(irr->height, pixbuf.GetHeight() - irr->y);

  if (nwidth <= 0 || nheight <= 0) {
    LOGE("Degenerate crop region for the image: " << filepath);
    return false;
  }

  crop = pixbuf.GetSubImage(wxRect(nx, ny, nwidth, nheight));

  return crop.IsOk();
}

bool WxImageCropperProvider::save_crop(wxImage& crop, std::string& tofpath)
{
  static const std::string extPNG = "png";

  assert(crop.IsOk());
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

  if (!crop.SaveFile(wxString::FromUTF8(tofpath), wxBITMAP_TYPE_PNG)) {
    LOGE("Failed to save the image: " << tofpath);
    return false;
  }

  return true;
}

std::string WxImageCropperProvider::get_ext_only(
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

WxImageCropperProvider::IImageCropperFacilityProviderPtr
WxImageCropperProvider::clone()
{
  return std::make_shared<WxImageCropperProvider>(*this);
}

}  // namespace imagesAnnotatorWx::window::custom_widgets
