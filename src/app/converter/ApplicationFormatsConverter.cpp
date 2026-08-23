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

#include "src/app/converter/ApplicationFormatsConverter.h"

#include <cassert>
#include <filesystem>
#include <memory>
#include <string>
#include <system_error>

#include "src/annotator-events/events/DisplayErrorEvent.h"
#include "src/annotator-events/events/DisplayErrorEventHandler.h"
#include "src/annotator-events/events/EventsFactory.h"
#include "src/app/ApplicationContext.h"
#include "src/app/converter/ConversionFormats.h"
#include "src/log/log.h"

namespace app::converter
{

bool ApplicationFormatsConverter::convert(
    std::shared_ptr<ApplicationContext> ctx)
{
  if (!accept_context(ctx)) {
    return false;
  }

  const bool converted = run_conversion();

  // The event controller of the context holds this converter as a subscriber
  // while this converter holds that very context, so the reference cycle is
  // broken here the way the annotator controller breaks its own one in deinit.
  actx.reset();

  return converted;
}

bool ApplicationFormatsConverter::run_conversion()
{
  ConversionFormat from{};
  ConversionFormat into{};

  return parse_formats(from, into) && import_step(from) && export_step(into);
}

bool ApplicationFormatsConverter::accept_context(
    std::shared_ptr<ApplicationContext> ctx)
{
  assert(ctx != nullptr);

  if (ctx == nullptr) {
    LOGE("No valid application context provided");
    return false;
  }

  assert(ctx->eventer != nullptr);

  if (ctx->eventer == nullptr) {
    LOGE("No valid application context' eventer found");
    return false;
  }

  actx = ctx;

  // The request events are submitted without a status, so the very error event
  // the graphical interface would have shown is what tells a failing half from
  // a successful one.
  actx->eventer->subscribe(
      events::events::DisplayErrorEventHandlerPtr{shared_from_this()});

  return true;
}

bool ApplicationFormatsConverter::parse_formats(ConversionFormat& from,
                                                ConversionFormat& into)
{
  if (!ConversionFormats::parse(actx->convert_from, from)) {
    LOGE("Unknown source annotations format: " << actx->convert_from);
    return false;
  }

  if (!ConversionFormats::parse(actx->convert_to, into)) {
    LOGE("Unknown destination annotations format: " << actx->convert_to);
    return false;
  }

  return true;
}

bool ApplicationFormatsConverter::import_step(const ConversionFormat& format)
{
  LOGI("Reading the " << actx->convert_from << " annotations from "
                      << actx->convert_src);

  failed = false;

  if (!import_from(format, actx->convert_src) || failed) {
    LOGE("Fail to read the " << actx->convert_from << " annotations from "
                             << actx->convert_src);
    return false;
  }

  return true;
}

bool ApplicationFormatsConverter::export_step(const ConversionFormat& format)
{
  LOGI("Writing the " << actx->convert_to << " annotations into "
                      << actx->convert_dst);

  if (!prepare_destination(format, actx->convert_dst)) {
    return false;
  }

  failed = false;

  if (!export_into(format, actx->convert_dst) || failed) {
    LOGE("Fail to write the " << actx->convert_to << " annotations into "
                              << actx->convert_dst);
    return false;
  }

  LOGI("The conversion into " << actx->convert_dst << " is complete");

  return true;
}

bool ApplicationFormatsConverter::prepare_destination(
    const ConversionFormat& format, const std::string& dstPath)
{
  // The native project is a single file, so what has to be there is the
  // directory holding it; every other format is given a directory of its own.
  const std::filesystem::path wanted =
      format == ConversionFormat::imagesannotator
          ? std::filesystem::path{dstPath}.parent_path()
          : std::filesystem::path{dstPath};

  if (wanted.empty() || std::filesystem::exists(wanted)) {
    return true;
  }

  std::error_code ecode;

  std::filesystem::create_directories(wanted, ecode);

  if (ecode) {
    LOGE("Fail to create the destination " << wanted.string() << ": "
                                           << ecode.message());
    return false;
  }

  return true;
}

std::shared_ptr<events::events::EventsFactory>
ApplicationFormatsConverter::get_events_factory()
{
  auto efactory = actx->eventer->get_events_factory();

  assert(efactory != nullptr);

  if (efactory == nullptr) {
    LOGE("No valid events factory found");
  }

  return efactory;
}

bool ApplicationFormatsConverter::import_from(const ConversionFormat& format,
                                              const std::string& srcPath)
{
  auto efactory = get_events_factory();

  if (efactory == nullptr) {
    return false;
  }

  switch (format) {
    case ConversionFormat::imagesannotator:
      actx->eventer->submit(efactory->create_annotations_dir_changed(srcPath));
      break;
    case ConversionFormat::plain_txt:
      actx->eventer->submit(efactory->create_plain_txt_import_request(srcPath));
      break;
    case ConversionFormat::yolo4:
      actx->eventer->submit(efactory->create_yolo4_import_request(srcPath));
      break;
    case ConversionFormat::ultralytics_detect:
      actx->eventer->submit(
          efactory->create_ultralytics_detect_import_request(srcPath));
      break;
    case ConversionFormat::ultralytics_obb:
      actx->eventer->submit(
          efactory->create_ultralytics_obb_import_request(srcPath));
      break;
    case ConversionFormat::ultralytics_segment:
      actx->eventer->submit(
          efactory->create_ultralytics_segment_import_request(srcPath));
      break;
    case ConversionFormat::coco:
      actx->eventer->submit(efactory->create_coco_import_request(srcPath));
      break;
    case ConversionFormat::pascal_voc:
      actx->eventer->submit(
          efactory->create_pascal_voc_import_request(srcPath));
      break;
    case ConversionFormat::createml:
      actx->eventer->submit(efactory->create_createml_import_request(srcPath));
      break;
    case ConversionFormat::pytorch:
      actx->eventer->submit(
          efactory->create_pytorch_vision_import_request(srcPath));
      break;
  }

  return true;
}

bool ApplicationFormatsConverter::export_into(const ConversionFormat& format,
                                              const std::string& dstPath)
{
  auto efactory = get_events_factory();

  if (efactory == nullptr) {
    return false;
  }

  switch (format) {
    case ConversionFormat::imagesannotator:
      actx->eventer->submit(efactory->create_store_request(dstPath));
      break;
    case ConversionFormat::plain_txt:
      actx->eventer->submit(
          efactory->create_plain_txt_2_folder_export(dstPath));
      break;
    case ConversionFormat::yolo4:
      actx->eventer->submit(efactory->create_yolo4_export_request(dstPath));
      break;
    case ConversionFormat::ultralytics_detect:
      actx->eventer->submit(
          efactory->create_ultralytics_detect_export_request(dstPath));
      break;
    case ConversionFormat::ultralytics_obb:
      actx->eventer->submit(
          efactory->create_ultralytics_obb_export_request(dstPath));
      break;
    case ConversionFormat::ultralytics_segment:
      actx->eventer->submit(
          efactory->create_ultralytics_segment_export_request(dstPath));
      break;
    case ConversionFormat::coco:
      actx->eventer->submit(efactory->create_coco_export_request(dstPath));
      break;
    case ConversionFormat::pascal_voc:
      actx->eventer->submit(
          efactory->create_pascal_voc_export_request(dstPath));
      break;
    case ConversionFormat::createml:
      actx->eventer->submit(efactory->create_createml_export_request(dstPath));
      break;
    case ConversionFormat::pytorch:
      // The only layout of the export side which is written by cutting the
      // rectangles out of the pictures. The cutters the graphical toolkits
      // implement are not there for a conversion, which starts no interface at
      // all, so the request goes out with an empty cropper slot and the
      // ImagesAnnotator-DataExporters library fills it with the OpenCV cropper
      // it ships itself - the very way the reading half leaves the image size
      // reader to the importers library.
      actx->eventer->submit(
          efactory->create_pytorch_vision_export_request(dstPath));
      break;
  }

  return true;
}

void ApplicationFormatsConverter::handle(DisplayErrorEventPtr event)
{
  assert(event != nullptr);

  if (event == nullptr) {
    LOGE("Invalid event pointer provided");
    return;
  }

  failed = true;
}

}  // namespace app::converter
