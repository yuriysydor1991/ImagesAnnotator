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

}  // namespace imagesAnnotatorQt6::window::event_handlers
