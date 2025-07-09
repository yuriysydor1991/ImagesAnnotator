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

  inline static constexpr const double defaultInitScaling = 1.0;
  inline static constexpr const double defaultScaleStep = 0.05;

  virtual ~ImageRecord() = default;
  ImageRecord(const std::string& npath = "", const std::string& nabspath = "");

  /// @brief The root directory from which this record was taken
  std::string abs_dir_path;

  /// @brief rel path to the image starting from abs_dir_path
  std::string path;

  ImageRecordRectSet rects;
  ImageRecordRectPtr current_rect;

  /// @brief Image width and height respectively
  int iwidth{0};
  int iheight{0};

  double imageScale{defaultInitScaling};

  void scaleStepIn();
  void scaleStepOut();

  std::string get_full_path() const;

  bool erase_current_rect();

  static bool equal(const ImageRecordsSet& l, const ImageRecordsSet& r);
  static bool equal(const ImageRecordPtr& l, const ImageRecordPtr& r);

  ImageRecordPtr duplicate_shared() const;

  static ImageRecordsSet duplicate(const ImageRecordsSet& orig);
};

using ImageRecordPtr = ImageRecord::ImageRecordPtr;
using ImageRecordsSet = ImageRecord::ImageRecordsSet;

}  // namespace events::events

#endif  // IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_EVENTS_IMAGERECORD_CLASS_H
