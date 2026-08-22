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

#ifndef IMAGES_ANNOTATOR_PROJECT_APP_CONVERTER_APPLICATIONFORMATSCONVERTER_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_APP_CONVERTER_APPLICATIONFORMATSCONVERTER_CLASS_H

#include <memory>
#include <string>

#include "src/annotator-events/events/DisplayErrorEvent.h"
#include "src/annotator-events/events/DisplayErrorEventHandler.h"
#include "src/annotator-events/events/EventsFactory.h"
#include "src/app/ApplicationContext.h"
#include "src/app/converter/ConversionFormats.h"
#include "src/app/converter/IFormatsConverter.h"

namespace app::converter
{

/**
 * @brief Converts an annotations dataset of one format into another one.
 *
 * The conversion is the import pipeline of the application followed by its
 * export one: the source is read into the very same annotations database the
 * graphical interface edits - the ImagesAnnotator-DataDrivers project database
 * the iannotator::AnnotatorController holds - and that database is then written
 * out in the destination format. Both halves are asked for through the request
 * events the menu entries submit, so this class carries no import or export
 * logic of its own and a layout the application gains is converted at once.
 *
 * A failing half reports itself through the DisplayErrorEvent the controller
 * submits for the interface to show, which is what this class subscribes for.
 */
class ApplicationFormatsConverter
    : public IFormatsConverter,
      public std::enable_shared_from_this<ApplicationFormatsConverter>,
      virtual public events::events::DisplayErrorEventHandler
{
 public:
  using DisplayErrorEventPtr = events::events::DisplayErrorEventPtr;

  virtual ~ApplicationFormatsConverter() = default;
  ApplicationFormatsConverter() = default;

  /**
   * @brief Implemented conversion. See the IFormatsConverter interface.
   *
   * @param ctx A parsed context naming both formats and both paths.
   *
   * @return Returns true when both halves of the conversion succeeded and
   * false otherwise.
   */
  virtual bool convert(std::shared_ptr<ApplicationContext> ctx) override;

  /**
   * @brief Notes the error the running half of the conversion reported. Its
   * description is not logged again here: whoever submits such an event logs
   * it already.
   *
   * @param event The error the annotator controller submitted.
   */
  virtual void handle(DisplayErrorEventPtr event) override;

 protected:
  /**
   * @brief Submits the import request event of the given source format.
   *
   * @param format The format to read.
   * @param srcPath The path to read it from.
   *
   * @return Returns true when the format is readable and its request was
   * submitted, and false otherwise.
   */
  virtual bool import_from(const ConversionFormat& format,
                           const std::string& srcPath);

  /**
   * @brief Submits the export request event of the given destination format.
   *
   * @param format The format to write.
   * @param dstPath The path to write it into.
   *
   * @return Returns true when the format is writable and its request was
   * submitted, and false otherwise.
   */
  virtual bool export_into(const ConversionFormat& format,
                           const std::string& dstPath);

 private:
  /**
   * @brief Stores the context and subscribes for the errors of both halves.
   *
   * @param ctx The context to run the conversion over.
   *
   * @return Returns true when the context carries everything a conversion
   * needs and false otherwise.
   */
  bool accept_context(std::shared_ptr<ApplicationContext> ctx);

  /**
   * @brief Resolves the command line spellings of both conversion sides.
   *
   * @param from Out: the resolved source format.
   * @param into Out: the resolved destination format.
   *
   * @return Returns true when both spellings name a format and false
   * otherwise.
   */
  bool parse_formats(ConversionFormat& from, ConversionFormat& into);

  /// @brief The events factory both halves build their request with, or a
  /// nullptr when the event controller hands out none.
  std::shared_ptr<events::events::EventsFactory> get_events_factory();

  /// @brief Runs and reports the reading half of the conversion.
  bool import_step(const ConversionFormat& format);

  /// @brief Runs and reports the writing half of the conversion.
  bool export_step(const ConversionFormat& format);

  /**
   * @brief Creates the destination the writing half is about to fill.
   *
   * A command line names a path which need not exist yet, while the graphical
   * interface only ever hands over a directory the user has picked out of a
   * chooser. The layouts differ on this - all of them but the plain text one
   * create the directory they were given - so the destination is made here
   * once for every format instead.
   *
   * @param format The format about to be written.
   * @param dstPath The path it is to be written into.
   *
   * @return Returns true when the destination is there and false otherwise.
   */
  bool prepare_destination(const ConversionFormat& format,
                           const std::string& dstPath);

  std::shared_ptr<ApplicationContext> actx;

  /// @brief Set by the error event of the half being run, since the request
  /// events themselves are submitted without a status.
  bool failed{false};
};

}  // namespace app::converter

#endif  // IMAGES_ANNOTATOR_PROJECT_APP_CONVERTER_APPLICATIONFORMATSCONVERTER_CLASS_H
