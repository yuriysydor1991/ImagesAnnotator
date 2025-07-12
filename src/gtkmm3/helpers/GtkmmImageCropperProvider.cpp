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

#include "src/gtkmm3/helpers/GtkmmImageCropperProvider.h"

#include <cassert>
#include <functional>
#include <map>
#include <memory>

#include "src/annotator-events/events/IImageCropperFacilityProvider.h"
#include "src/gtkmm3/gtkmm_includes.h"
#include "src/log/log.h"

namespace templateGtkmm3::window::helpers
{

bool GtkmmImageCropperProvider::crop_out_2_fs(ImageRecordPtr ir,
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

  Glib::RefPtr<Gdk::Pixbuf> pixbuf;

  auto crop = load_and_crop(pixbuf, ir, irr);

  if (!crop) {
    LOGE("Fail to load and/or crop the original image");
    return false;
  }

  if (!save_crop(crop, tofpath)) {
    LOGE("Fail to save new crop: " << tofpath);
    return false;
  }

  return true;
}

Glib::RefPtr<Gdk::Pixbuf> GtkmmImageCropperProvider::load_and_crop(
    Glib::RefPtr<Gdk::Pixbuf>& pixbuf, ImageRecordPtr ir,
    ImageRecordRectPtr irr)
{
  assert(ir != nullptr);
  assert(irr != nullptr);

  const std::string filepath = ir->get_full_path();

  if (filepath.empty()) {
    LOGE("Invalid filepath obtained from the image record");
    return {};
  }

  try {
    pixbuf = Gdk::Pixbuf::create_from_file(filepath);
  }
  catch (const Glib::Error& ex) {
    LOGE("Failed to load the image: " << ex.what() << ":" << filepath);
    return {};
  };

  if (!pixbuf) {
    LOGE("Failure to obtain the image buffer");
    return {};
  }

  const auto nx = std::max(0, irr->x);
  const auto ny = std::max(0, irr->y);
  const auto nwidth = std::min(irr->width, pixbuf->get_width() - irr->x);
  const auto nheight = std::min(irr->height, pixbuf->get_height() - irr->y);

  Glib::RefPtr<Gdk::Pixbuf> crop =
      Gdk::Pixbuf::create_subpixbuf(pixbuf, nx, ny, nwidth, nheight);

  return crop;
}

bool GtkmmImageCropperProvider::save_crop(Glib::RefPtr<Gdk::Pixbuf> crop,
                                          std::string& tofpath)
{
  static const std::string extPNG = "png";

  assert(crop);
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

  try {
    crop->save(tofpath, extUsed);
  }
  catch (const Glib::Error& ex) {
    LOGE("Failed to save the image: " << ex.what() << " : " << tofpath);
    return false;
  };

  return true;
}

std::string GtkmmImageCropperProvider::get_ext_only(
    const std::filesystem::path& fspath)
{
  if (fspath.empty()) {
    LOGE("Empty path privided");
    return {};
  }

  std::string dotext = fspath.extension().string();

  if (dotext.empty()) {
    LOGT("Empty string extention retrieved");
    return {};
  }

  if (dotext[0] == '.') {
    dotext.erase(dotext.begin());
  }

  return dotext;
}

GtkmmImageCropperProvider::IImageCropperFacilityProviderPtr
GtkmmImageCropperProvider::clone()
{
  return std::make_shared<GtkmmImageCropperProvider>(*this);
}

}  // namespace templateGtkmm3::window::helpers
