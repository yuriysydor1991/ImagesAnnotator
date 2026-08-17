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

#ifndef IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_EVENTS_IIMAGESIZEFACILITYPROVIDER_ABSTRACT_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_EVENTS_IIMAGESIZEFACILITYPROVIDER_ABSTRACT_CLASS_H

#include "src/annotator-events/events/DataImportersAliases.h"

namespace events::events
{

/// @brief The image measuring service interface, provided by the
/// ImagesAnnotator-DataImporters library. The library decodes no image format
/// of its own, and an annotation of the project format is a rectangle in the
/// pixels of its image, so the imports of the layouts which do not store those
/// pixels - the YOLO v4 one, whose boxes are normalised, and the PyTorch Vision
/// one, whose annotation is the cropped image itself - measure the pictures
/// through this interface, which the GUI toolkits implement over the imaging
/// stack they already link. It is the counterpart of the
/// IImageCropperFacilityProvider of the export side. See IRecord.h on why this
/// is an alias.
using IImageSizeFacilityProvider = iadi::IImageSizeFacility;

using IImageSizeFacilityProviderPtr = iadi::IImageSizeFacilityPtr;

}  // namespace events::events

#endif  // IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_EVENTS_IIMAGESIZEFACILITYPROVIDER_ABSTRACT_CLASS_H
