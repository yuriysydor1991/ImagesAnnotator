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

#ifndef IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_EVENTS_EXPORT2PYTORCHFOLDERREQUESTHANDLER_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_EVENTS_EXPORT2PYTORCHFOLDERREQUESTHANDLER_CLASS_H

#include <memory>

#include "src/annotator-events/events/EventIHandler.h"
#include "src/annotator-events/events/Export2PyTorchVisionRequest.h"
#include "src/annotator-events/events/ImagesDirChanged.h"

namespace events::events
{

/**
 * @brief The PyTorch Vision folder format export event handler abstract class.
 */
class Export2PyTorchVisionRequestHandler : virtual public EventIHandler
{
 public:
  virtual ~Export2PyTorchVisionRequestHandler() = default;
  Export2PyTorchVisionRequestHandler() = default;

  virtual void handle(Export2PyTorchVisionRequestPtr event) = 0;
};

using Export2PyTorchVisionRequestHandlerPtr =
    std::shared_ptr<Export2PyTorchVisionRequestHandler>;

}  // namespace events::events

#endif  // IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_EVENTS_EXPORT2PYTORCHFOLDERREQUESTHANDLER_CLASS_H
