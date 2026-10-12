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

#ifndef IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_EVENTS_DATA_DRIVERS_ALIASES_H
#define IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_EVENTS_DATA_DRIVERS_ALIASES_H

// The one and only place in the whole project that names the
// ImagesAnnotator-DataDrivers library. Everything else - the record value
// objects, the annotations database interface, the records factory and the
// merger - is reached through the iadd alias declared below, so bumping the
// library to a namespace of the next version stays a one line change here.
//
// The header is included through the versioned
// <ImagesAnnotatorDataDrivers-0.14/...> directory the library installs, and not
// through the plain <IADataDriversFacade.h> the unprefixed include root would
// also resolve: the prefix pins the library version this translation unit talks
// to, and the generic names sitting on that same root - IRecord.h,
// ImageRecord.h - are the ones a busy include path collides with.
//
// IADataDriversFacade.h transitively pulls in every other public header of the
// library (the annotations database, the merger, the records factory, the
// library context and both record value objects), so a single include is
// enough.
#include <ImagesAnnotatorDataDrivers-0.14/IADataDriversFacade.h>

/// @brief The ImagesAnnotator-DataDrivers library namespace, whose name carries
/// the library major and minor version numbers.
namespace iadd = ImagesAnnotatorDataDrivers014;

#endif  // IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_EVENTS_DATA_DRIVERS_ALIASES_H
