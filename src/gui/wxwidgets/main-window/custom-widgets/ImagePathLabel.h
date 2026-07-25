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

#ifndef IMAGES_ANNOTATOR_PROJECT_WX_CUSTOM_WIDGETS_IMAGEPATHLABEL_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_WX_CUSTOM_WIDGETS_IMAGEPATHLABEL_CLASS_H

#include <memory>
#include <string>

#include "src/annotator-events/events/ImageRecord.h"

namespace templateWx::window::custom_widgets
{

/**
 * @brief Row data holder for one image in the images list.
 *
 * The wxWidgets analogue of templateGtkmm3's ImagePathLabel. In GTKmm it was a
 * Gtk::Label living inside a Gtk::ListBox row; the wx images list is a
 * wxListCtrl, so this class carries only the data a row needs - the
 * ImageRecord, the display text and the "has annotations" flag the list uses to
 * paint the row background - instead of being a widget itself.
 */
class ImagePathLabel
{
 public:
  using ImagePathLabelPtr = std::shared_ptr<ImagePathLabel>;
  using ImageRecord = events::events::ImageRecord;

  virtual ~ImagePathLabel() = default;
  explicit ImagePathLabel(std::shared_ptr<ImageRecord> nr);

  virtual std::shared_ptr<ImageRecord> get_image_rec();

  /// @brief The path/URL shown for this image (computed once at construction).
  const std::string& get_text() const;

  /// @brief Whether the image already carries annotation rectangles - drives
  /// the green row background, the wx analogue of the image_row_has_records CSS
  /// class.
  bool has_records() const;

  virtual bool mark_as_has_records();
  virtual bool remove_has_records_mark();

 private:
  void update_text();

  std::shared_ptr<ImageRecord> myrec;
  std::string text;
  bool hasRecords{false};
};

using ImagePathLabelPtr = ImagePathLabel::ImagePathLabelPtr;

}  // namespace templateWx::window::custom_widgets

#endif  // IMAGES_ANNOTATOR_PROJECT_WX_CUSTOM_WIDGETS_IMAGEPATHLABEL_CLASS_H
