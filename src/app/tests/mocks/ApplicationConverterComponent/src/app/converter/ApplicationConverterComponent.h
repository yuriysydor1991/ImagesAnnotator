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

#ifndef IMAGES_ANNOTATOR_PROJECT_APP_CONVERTER_APPLICATIONCONVERTERCOMPONENT_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_APP_CONVERTER_APPLICATIONCONVERTERCOMPONENT_CLASS_H

#include <gmock/gmock.h>

#include <functional>
#include <memory>

#include "src/app/ApplicationContext.h"
#include "src/app/IApplication.h"
#include "src/app/converter/IFormatsConverter.h"

namespace app::converter
{

class ApplicationConverterComponent : public IApplication
{
 public:
  virtual ~ApplicationConverterComponent() = default;

  explicit ApplicationConverterComponent(const IFormatsConverterPtr& nconverter)
      : converter{nconverter}
  {
    if (onMockCreate) {
      onMockCreate(*this);
    }
  }

  inline static std::function<void(ApplicationConverterComponent&)>
      onMockCreate;

  /// @brief The converter handed over at the construction, which is what makes
  /// the both instances the factory creates checkable.
  IFormatsConverterPtr converter;

  MOCK_METHOD(int, run, (std::shared_ptr<ApplicationContext> ctx), (override));
};

}  // namespace app::converter

#endif  // IMAGES_ANNOTATOR_PROJECT_APP_CONVERTER_APPLICATIONCONVERTERCOMPONENT_CLASS_H
