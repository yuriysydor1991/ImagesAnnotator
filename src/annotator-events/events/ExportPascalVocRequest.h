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

#ifndef IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_EVENTS_EXPORTPASCALVOCREQUEST_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_EVENTS_EXPORTPASCALVOCREQUEST_CLASS_H

#include <memory>
#include <string>

#include "src/annotator-events/events/IRequestEvent.h"

namespace events::events
{

/**
 * @brief Event for the available annotations data export as the Pascal VOC
 * dataset into some folder.
 *
 * The layout is the devkit directory shape: the copied images under
 * JPEGImages/, one Annotations XML descriptor per image over them and the
 * ImageSets/Main lists naming those. These are the very files LabelImg saves
 * its own work in, so an exported project opens for correction in that
 * annotator directly, and the torchvision VOCDetection and the MMDetection
 * XMLDataset take the same directories. A rectangle reaches the descriptor as
 * the two corner points it was drawn between, in the pixels of the image it
 * was drawn in.
 */
class ExportPascalVocRequest : virtual public IRequestEvent
{
 public:
  virtual ~ExportPascalVocRequest() = default;
  explicit ExportPascalVocRequest(const std::string& newFolderPath);

  std::string dst_folder_path;
};

using ExportPascalVocRequestPtr = std::shared_ptr<ExportPascalVocRequest>;

}  // namespace events::events

#endif  // IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_EVENTS_EXPORTPASCALVOCREQUEST_CLASS_H
