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

#include <memory>

#include "src/annotator-business/AnnotatorController.h"
#include "src/annotator-events/ImagesAnnotatorEventController.h"
#include "src/app/ApplicationContext.h"
#include "src/app/applications/IApplication.h"
#include "src/app/applications/converter/IFormatsConverter.h"

namespace app::converter
{

/**
 * @brief The application component which runs a requested format conversion.
 *
 * It is the app::Application of the conversion command line flags: the very
 * same annotations pipeline is brought up - the event controller of the
 * context and the annotator business controller holding the project database -
 * and the converter is run over it in place of the graphical interface, which
 * this component never starts. The user is told what is being prepared and
 * converted through the log messages alone.
 */
class ApplicationConverterComponent : public IApplication
{
 public:
  virtual ~ApplicationConverterComponent() = default;

  /**
   * @brief An ApplicationConverterComponent constructor.
   *
   * @param nconverter The converter to run once the pipeline is up.
   */
  explicit ApplicationConverterComponent(
      const IFormatsConverterPtr& nconverter);

  /**
   * @brief Implemented conversion application. See IApplication interface.
   *
   * @param ctx Application's run context with the parsed conversion flags.
   *
   * @return Returns the application return status. A zero value on the success
   * and other value otherwise.
   */
  virtual int run(std::shared_ptr<ApplicationContext> ctx) override;

 protected:
  virtual std::shared_ptr<iannotator::AnnotatorController>
  create_annotator_b_controller();
  virtual std::shared_ptr<events::ImagesAnnotatorEventController>
  create_event_controller(std::shared_ptr<ApplicationContext> ctx);

  std::shared_ptr<iannotator::AnnotatorController> abcontroller;

 private:
  IFormatsConverterPtr converter;
};

}  // namespace app::converter

#endif  // IMAGES_ANNOTATOR_PROJECT_APP_CONVERTER_APPLICATIONCONVERTERCOMPONENT_CLASS_H
