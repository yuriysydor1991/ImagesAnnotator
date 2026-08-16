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

#include "src/gui/qt6/main-window/event-handlers/ExportEventsHandler.h"

#include <cassert>
#include <functional>
#include <string>

#include "src/gui/qt6/MainWindowContext.h"
#include "src/gui/qt6/main-window/custom-widgets/QtImageCropperProvider.h"
#include "src/gui/qt6/main-window/event-handlers/WindowOps.h"
#include "src/gui/qt6/qt_includes.h"
#include "src/log/log.h"

namespace imagesAnnotatorQt6::window::event_handlers
{

ExportEventsHandler::ExportEventsHandler(std::shared_ptr<WindowOps> nops)
    : AbstractWindowEventHandler(nops)
{
}

bool ExportEventsHandler::subscribe()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);

  auto* window = mwctx->wloader->get_window();

  QObject::connect(mwctx->wloader->get_export_txt2_folder_mi(),
                   &QAction::triggered, window,
                   [this]() { handle_export_txt(); });
  QObject::connect(mwctx->wloader->get_export_yolo4_folder_mi(),
                   &QAction::triggered, window,
                   [this]() { handle_export_yolo4(); });
  QObject::connect(mwctx->wloader->get_export_pytorchvision_folder_mi(),
                   &QAction::triggered, window,
                   [this]() { handle_export_pytorch(); });
  QObject::connect(mwctx->wloader->get_export_ultralytics_detect_folder_mi(),
                   &QAction::triggered, window,
                   [this]() { handle_export_ultralytics_detect(); });
  QObject::connect(mwctx->wloader->get_export_ultralytics_obb_folder_mi(),
                   &QAction::triggered, window,
                   [this]() { handle_export_ultralytics_obb(); });
  QObject::connect(mwctx->wloader->get_export_ultralytics_segment_folder_mi(),
                   &QAction::triggered, window,
                   [this]() { handle_export_ultralytics_segment(); });
  QObject::connect(mwctx->wloader->get_export_coco_folder_mi(),
                   &QAction::triggered, window,
                   [this]() { handle_export_coco(); });
  QObject::connect(mwctx->wloader->get_export_pascal_voc_folder_mi(),
                   &QAction::triggered, window,
                   [this]() { handle_export_pascal_voc(); });

  return true;
}

bool ExportEventsHandler::has_to_export()
{
  assert(mwctx != nullptr);

  return !mwctx->imagesVDB.empty() ||
         (mwctx->images_provider != nullptr &&
          !mwctx->images_provider->get_db_path().empty());
}

void ExportEventsHandler::handle_export_txt()
{
  assert(mwctx != nullptr);
  assert(mwctx->cwFactory != nullptr);

  if (!has_to_export()) {
    ops->show_error_dialog("No images found or db opened to export!");
    return;
  }

  std::string newFileName;

  if (!mwctx->cwFactory->ask_txt_export_folder(mwctx->wloader->get_window(),
                                               newFileName)) {
    LOGD("Dialog is closed");
    return;
  }

  LOGD("Selected txt export folder: " << newFileName);

  ops->show_spinner();

  auto efactory = mwctx->actx->eventer->get_events_factory();

  assert(efactory != nullptr);

  auto exportEvent = efactory->create_plain_txt_2_folder_export(newFileName);

  mwctx->actx->eventer->submit(exportEvent);

  ops->update_statuses();

  ops->hide_spinner();
}

void ExportEventsHandler::handle_export_yolo4()
{
  assert(mwctx != nullptr);
  assert(mwctx->cwFactory != nullptr);

  if (!has_to_export()) {
    ops->show_error_dialog("No images found or db opened to export!");
    return;
  }

  std::string newFileName;

  if (!mwctx->cwFactory->ask_yolo4_export_folder(mwctx->wloader->get_window(),
                                                 newFileName)) {
    LOGD("Dialog is closed");
    return;
  }

  LOGD("Selected yolo4 export folder: " << newFileName);

  ops->show_spinner();

  auto efactory = mwctx->actx->eventer->get_events_factory();

  assert(efactory != nullptr);

  auto exportEvent = efactory->create_yolo4_export_request(newFileName);

  mwctx->actx->eventer->submit(exportEvent);

  ops->update_statuses();

  ops->hide_spinner();
}

void ExportEventsHandler::handle_export_pytorch()
{
  assert(mwctx != nullptr);
  assert(mwctx->cwFactory != nullptr);

  if (!has_to_export()) {
    ops->show_error_dialog("No images found or db opened to export!");
    return;
  }

  std::string newFileName;

  if (!mwctx->cwFactory->ask_pytorch_export_folder(mwctx->wloader->get_window(),
                                                   newFileName)) {
    LOGD("Dialog is closed");
    return;
  }

  LOGD("Selected PyTorch Vision export folder: " << newFileName);

  ops->show_spinner();

  auto efactory = mwctx->actx->eventer->get_events_factory();

  assert(efactory != nullptr);

  auto exportEvent =
      efactory->create_pytorch_vision_export_request(newFileName);

  assert(exportEvent != nullptr);

  exportEvent->cropper =
      std::make_shared<custom_widgets::QtImageCropperProvider>();

  mwctx->actx->eventer->submit(exportEvent);

  ops->update_statuses();

  ops->hide_spinner();
}

void ExportEventsHandler::handle_folder_export(
    const std::function<bool(std::string&)>& askFolder,
    const std::function<void(const std::string&)>& submitExport)
{
  if (!has_to_export()) {
    ops->show_error_dialog("No images found or db opened to export!");
    return;
  }

  std::string newFileName;

  if (!askFolder(newFileName)) {
    LOGD("Dialog is closed");
    return;
  }

  LOGD("Selected export folder: " << newFileName);

  ops->show_spinner();

  submitExport(newFileName);

  ops->update_statuses();

  ops->hide_spinner();
}

void ExportEventsHandler::handle_export_ultralytics_detect()
{
  assert(mwctx != nullptr);
  assert(mwctx->cwFactory != nullptr);

  handle_folder_export(
      [this](std::string& out) {
        return mwctx->cwFactory->ask_ultralytics_detect_export_folder(
            mwctx->wloader->get_window(), out);
      },
      [this](const std::string& folder) {
        auto efactory = mwctx->actx->eventer->get_events_factory();

        assert(efactory != nullptr);

        mwctx->actx->eventer->submit(
            efactory->create_ultralytics_detect_export_request(folder));
      });
}

void ExportEventsHandler::handle_export_ultralytics_obb()
{
  assert(mwctx != nullptr);
  assert(mwctx->cwFactory != nullptr);

  handle_folder_export(
      [this](std::string& out) {
        return mwctx->cwFactory->ask_ultralytics_obb_export_folder(
            mwctx->wloader->get_window(), out);
      },
      [this](const std::string& folder) {
        auto efactory = mwctx->actx->eventer->get_events_factory();

        assert(efactory != nullptr);

        mwctx->actx->eventer->submit(
            efactory->create_ultralytics_obb_export_request(folder));
      });
}

void ExportEventsHandler::handle_export_ultralytics_segment()
{
  assert(mwctx != nullptr);
  assert(mwctx->cwFactory != nullptr);

  handle_folder_export(
      [this](std::string& out) {
        return mwctx->cwFactory->ask_ultralytics_segment_export_folder(
            mwctx->wloader->get_window(), out);
      },
      [this](const std::string& folder) {
        auto efactory = mwctx->actx->eventer->get_events_factory();

        assert(efactory != nullptr);

        mwctx->actx->eventer->submit(
            efactory->create_ultralytics_segment_export_request(folder));
      });
}

void ExportEventsHandler::handle_export_coco()
{
  assert(mwctx != nullptr);
  assert(mwctx->cwFactory != nullptr);

  handle_folder_export(
      [this](std::string& out) {
        return mwctx->cwFactory->ask_coco_export_folder(
            mwctx->wloader->get_window(), out);
      },
      [this](const std::string& folder) {
        auto efactory = mwctx->actx->eventer->get_events_factory();

        assert(efactory != nullptr);

        mwctx->actx->eventer->submit(
            efactory->create_coco_export_request(folder));
      });
}

void ExportEventsHandler::handle_export_pascal_voc()
{
  assert(mwctx != nullptr);
  assert(mwctx->cwFactory != nullptr);

  handle_folder_export(
      [this](std::string& out) {
        return mwctx->cwFactory->ask_pascal_voc_export_folder(
            mwctx->wloader->get_window(), out);
      },
      [this](const std::string& folder) {
        auto efactory = mwctx->actx->eventer->get_events_factory();

        assert(efactory != nullptr);

        mwctx->actx->eventer->submit(
            efactory->create_pascal_voc_export_request(folder));
      });
}

}  // namespace imagesAnnotatorQt6::window::event_handlers
