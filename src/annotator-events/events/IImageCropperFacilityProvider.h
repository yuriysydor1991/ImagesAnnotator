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

#ifndef IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_EVENTS_IIMAGECROPPERFACILITYPROVIDER_ABSTRACT_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_EVENTS_IIMAGECROPPERFACILITYPROVIDER_ABSTRACT_CLASS_H

#include "src/annotator-events/events/DataExportersAliases.h"
#include "src/annotator-events/events/ImageRecord.h"

namespace events::events
{

/// @brief The image cropping service interface, provided by the
/// ImagesAnnotator-DataExporters library. The library decodes no image format
/// of its own, so the PyTorch Vision export cuts the annotated rectangles out
/// through this interface, which the GUI toolkits implement over the imaging
/// stack they already link. See IRecord.h on why this is an alias.
using IImageCropperFacilityProvider = iade::IImageCropperFacility;

using IImageCropperFacilityProviderPtr = iade::IImageCropperFacilityPtr;

}  // namespace events::events

#endif  // IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_EVENTS_IIMAGECROPPERFACILITYPROVIDER_ABSTRACT_CLASS_H
