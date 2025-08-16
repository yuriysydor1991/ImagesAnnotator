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

#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM3_CUSTOM_WIDGETS_IMAGEPATHLABEL_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM3_CUSTOM_WIDGETS_IMAGEPATHLABEL_CLASS_H

#include <memory>

#include "src/annotator-events/events/ImageRecord.h"
#include "src/gtkmm3/gtkmm_includes.h"
#include "src/gtkmm3/main-window/WindowDataContext.h"
#include "src/helpers/ImageRecordUrlAndPathHelper.h"

namespace templateGtkmm3::window::custom_widgets
{

/**
 * @brief Subclass for images db path display in the LibBox.
 */
class ImagePathLabel : public Gtk::Label,
                       public std::enable_shared_from_this<ImagePathLabel>,
                       virtual public helpers::ImageRecordUrlAndPathHelper
{
 public:
  using ImagePathLabelPtr = std::shared_ptr<ImagePathLabel>;
  using ImageRecord = events::events::ImageRecord;

  inline static constexpr const char* const has_records_css_class =
      "image_row_has_records";

  virtual ~ImagePathLabel() = default;
  explicit ImagePathLabel(std::shared_ptr<ImageRecord> nr);

  virtual std::shared_ptr<ImageRecord> get_image_rec();

  virtual bool mark_as_has_records();
  virtual bool remove_has_records_mark();

 private:
  void update_text();

  std::shared_ptr<ImageRecord> myrec;
};

using ImagePathLabelPtr = ImagePathLabel::ImagePathLabelPtr;

}  // namespace templateGtkmm3::window::custom_widgets

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM3_CUSTOM_WIDGETS_IMAGEPATHLABEL_CLASS_H
