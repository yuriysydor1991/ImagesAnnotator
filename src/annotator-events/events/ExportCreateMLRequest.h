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

#ifndef IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_EVENTS_EXPORTCREATEMLREQUEST_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_EVENTS_EXPORTCREATEMLREQUEST_CLASS_H

#include <memory>
#include <string>

#include "src/annotator-events/events/IRequestEvent.h"

namespace events::events
{

/**
 * @brief Event for the available annotations data export as the Create ML
 * object detection dataset into some folder.
 *
 * The layout is one flat directory: the copied images and the single
 * annotations.json descriptor beside them, which is the
 * directoryWithImagesAndJsonAnnotation data source of Apple's MLObjectDetector
 * and the very folder the Create ML application takes when one is dropped into
 * its training well. A run over it produces a Core ML detector, the one route
 * from a project of this application onto an iPhone. A rectangle reaches the
 * descriptor as the centre of the box next to its size, in the pixels of the
 * image it was drawn in.
 */
class ExportCreateMLRequest : virtual public IRequestEvent
{
 public:
  virtual ~ExportCreateMLRequest() = default;
  explicit ExportCreateMLRequest(const std::string& newFolderPath);

  std::string dst_folder_path;
};

using ExportCreateMLRequestPtr = std::shared_ptr<ExportCreateMLRequest>;

}  // namespace events::events

#endif  // IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_EVENTS_EXPORTCREATEMLREQUEST_CLASS_H
