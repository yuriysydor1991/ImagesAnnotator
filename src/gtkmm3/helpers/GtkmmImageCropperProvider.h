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

#ifndef IMAGES_ANNOTATOR_TEMPLATE_PROJECT_GTKMMIMAGECROPPERPROVIDER_CLASS_H
#define IMAGES_ANNOTATOR_TEMPLATE_PROJECT_GTKMMIMAGECROPPERPROVIDER_CLASS_H

#include <functional>
#include <map>
#include <memory>

#include "src/annotator-events/events/IImageCropperFacilityProvider.h"
#include "src/annotator-events/events/ImageRecord.h"
#include "src/annotator-events/events/ImageRecordRect.h"
#include "src/gtkmm3/gtkmm_includes.h"

namespace templateGtkmm3::window::helpers
{

/**
 * @brief The IImageCropperFacilityProvider facility interface implementation.
 */
class GtkmmImageCropperProvider
    : virtual public events::events::IImageCropperFacilityProvider,
      public std::enable_shared_from_this<GtkmmImageCropperProvider>
{
 public:
  using ImageRecordPtr = events::events::ImageRecordPtr;
  using ImageRecordRectPtr = events::events::ImageRecordRectPtr;

  virtual ~GtkmmImageCropperProvider() = default;
  GtkmmImageCropperProvider() = default;

  virtual bool crop_out_2_fs(ImageRecordPtr ir, ImageRecordRectPtr irr,
                             const std::string& tofpath) override;

  virtual IImageCropperFacilityProviderPtr clone() override;

 private:
  Glib::RefPtr<Gdk::Pixbuf> load_and_crop(Glib::RefPtr<Gdk::Pixbuf>& pixbuf,
                                          ImageRecordPtr ir,
                                          ImageRecordRectPtr irr);
  bool save_crop(Glib::RefPtr<Gdk::Pixbuf> crop, const std::string& tofpath);
};

}  // namespace templateGtkmm3::window::helpers

#endif  // IMAGES_ANNOTATOR_TEMPLATE_PROJECT_GTKMMIMAGECROPPERPROVIDER_CLASS_H
