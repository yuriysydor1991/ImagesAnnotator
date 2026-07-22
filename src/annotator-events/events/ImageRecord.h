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

#ifndef IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_EVENTS_IMAGERECORD_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_EVENTS_IMAGERECORD_CLASS_H

#include <filesystem>
#include <string>
#include <vector>

#include "src/annotator-events/events/IRecord.h"
#include "src/annotator-events/events/ImageRecordRect.h"

namespace events::events
{

/**
 * @brief The particular image record class.
 */
class ImageRecord : virtual public IRecord
{
 public:
  using ImageRecordPtr = std::shared_ptr<ImageRecord>;
  using ImageRecordsSet = std::vector<ImageRecordPtr>;

  inline static constexpr const double defaultScaleStep = 0.05;

  /// @brief Bounds for the user-controlled zoom. scaleStepOut() never drives
  /// the scale to zero or below (which would produce a degenerate scaled pixbuf
  /// and crash gdk_pixbuf_scale_simple()), and scaleStepIn() never magnifies
  /// past a sane maximum. minScale stays strictly positive so it never collides
  /// with the "0 == no user-selected scale yet" sentinel.
  inline static constexpr const double minScale = 0.05;
  inline static constexpr const double maxScale = 20.0;

  virtual ~ImageRecord() = default;
  ImageRecord(const std::string& npath = "", const std::string& nabspath = "");

  /// @brief The root directory from which this record was taken
  std::string abs_dir_path;

  /// @brief rel path to the image starting from abs_dir_path
  std::string path;

  /**
   * @brief If the image is loaded from the network - this is it's temporary
   * cache filepath. Not included into the projects JSON files.
   */
  std::filesystem::path tmppath;

  ImageRecordRectSet rects;
  ImageRecordRectPtr current_rect;

  /// @brief Image width and height respectively
  int iwidth{0};
  int iheight{0};

  /// @brief The per-image preview scale. A value of 0 means "no user-selected
  /// scale yet": the GUI computes the initial fit-to-window scale on first
  /// display, and any value set afterwards (that fit or a subsequent user zoom)
  /// is preserved across image switches and serialized to the project JSON.
  double imageScale{0.0};

  void scaleStepIn();
  void scaleStepOut();

  std::string get_full_path() const;

  bool erase_current_rect();

  static bool equal(const ImageRecordsSet& l, const ImageRecordsSet& r);
  static bool equal(const ImageRecordPtr& l, const ImageRecordPtr& r);

  ImageRecordPtr duplicate_shared() const;

  static ImageRecordsSet duplicate(const ImageRecordsSet& orig);

  static ImageRecordPtr create();
  static ImageRecordPtr create(const std::string& npath,
                               const std::string& nabspath);
};

using ImageRecordPtr = ImageRecord::ImageRecordPtr;
using ImageRecordsSet = ImageRecord::ImageRecordsSet;

}  // namespace events::events

#endif  // IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_EVENTS_IMAGERECORD_CLASS_H
