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

#ifndef IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_EVENTS_IMPORTULTRALYTICSSEGMENTREQUEST_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_EVENTS_IMPORTULTRALYTICSSEGMENTREQUEST_CLASS_H

#include <memory>
#include <string>

#include "src/annotator-events/events/IImageSizeFacilityProvider.h"
#include "src/annotator-events/events/IRequestEvent.h"

namespace events::events
{

/**
 * @brief Event for the Ultralytics YOLO instance segmentation dataset import
 * from some folder into the currently opened project.
 *
 * The layout is the one of the two imports above, with the polygon outlining
 * the object on every label file line. The annotations database holds no mask,
 * so what comes back is the rectangle holding that polygon - which for a
 * dataset this application exported is the very rectangle it was written out
 * of.
 */
class ImportUltralyticsSegmentRequest : virtual public IRequestEvent
{
 public:
  virtual ~ImportUltralyticsSegmentRequest() = default;
  explicit ImportUltralyticsSegmentRequest(const std::string& newFolderPath);

  /// @brief The directory holding the dataset to read.
  std::string src_folder_path;

  /// @brief The image measuring service of the GUI toolkit in use.
  IImageSizeFacilityProviderPtr sizer;
};

using ImportUltralyticsSegmentRequestPtr =
    std::shared_ptr<ImportUltralyticsSegmentRequest>;

}  // namespace events::events

#endif  // IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_EVENTS_IMPORTULTRALYTICSSEGMENTREQUEST_CLASS_H
