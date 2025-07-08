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

#ifndef IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_EVENTS_IMAGESPATHSDBPROVIDER_ABSTRACT_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_EVENTS_IMAGESPATHSDBPROVIDER_ABSTRACT_CLASS_H

#include <memory>
#include <set>
#include <string>

#include "src/annotator-events/events/IProvider.h"
#include "src/annotator-events/events/ImageRecord.h"

namespace events::events
{

/**
 * @brief The images dir changed event.
 */
class ImagesPathsDBProvider : virtual public IProvider
{
 public:
  using AnnotationsList = std::set<std::string>;

  virtual ~ImagesPathsDBProvider() = default;
  ImagesPathsDBProvider() = default;

  virtual ImageRecordsSet& get_images_db() = 0;

  virtual std::string get_db_path() = 0;

  virtual AnnotationsList get_available_annotations() = 0;

  virtual bool changed() = 0;
};

using ImagesPathsDBProviderPtr = std::shared_ptr<ImagesPathsDBProvider>;

}  // namespace events::events

#endif  // IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_EVENTS_IMAGESPATHSDBPROVIDER_ABSTRACT_CLASS_H
