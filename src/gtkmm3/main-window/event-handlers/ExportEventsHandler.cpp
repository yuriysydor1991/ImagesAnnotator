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

#include "src/gtkmm3/main-window/event-handlers/ExportEventsHandler.h"

#include <cassert>
#include <memory>
#include <string>

#include "src/gtkmm3/MainWindowContext.h"
#include "src/gtkmm3/helpers/GtkmmImageCropperProvider.h"
#include "src/gtkmm3/main-window/event-handlers/WindowOps.h"
#include "src/log/log.h"

namespace templateGtkmm3::window::event_handlers
{

ExportEventsHandler::ExportEventsHandler(std::shared_ptr<WindowOps> nops)
    : AbstractWindowEventHandler(nops)
{
}

bool ExportEventsHandler::subscribe()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);

  auto* exportTxt2FolderM = mwctx->wloader->get_export_txt2_folder_mi();
  auto* exportYolo42FolderM = mwctx->wloader->get_export_yolo4_folder_mi();
  auto* export2PyTorchVM = mwctx->wloader->get_export_pytorchvision_folder_mi();

  exportTxt2FolderM->signal_activate().connect(
      sigc::mem_fun(*this, &ExportEventsHandler::handle_export_txt));

  exportYolo42FolderM->signal_activate().connect(
      sigc::mem_fun(*this, &ExportEventsHandler::handle_export_yolo4));

  export2PyTorchVM->signal_activate().connect(
      sigc::mem_fun(*this, &ExportEventsHandler::handle_export_pytorch));

  return true;
}

void ExportEventsHandler::handle_export_txt()
{
  assert(mwctx != nullptr);
  assert(mwctx->cwFactory != nullptr);

  if (!has_to_export()) {
    const std::string errmsg = "No images found or db opened to export!";
    ops->show_error_dialog(errmsg);
    return;
  }

  ops->show_spinner();

  auto dialog = mwctx->cwFactory->create_txt_export_folder_choose_dialog(
      mwctx->wloader->get_window());

  const int result = dialog->run();

  if (result != Gtk::RESPONSE_OK) {
    LOGD("Dialog is closed");
    ops->hide_spinner();
    return;
  }

  const auto newFileName = dialog->get_filename();

  LOGD("Selected txt export folder: " << newFileName);

  auto efactory = mwctx->actx->eventer->get_events_factory();

  assert(efactory != nullptr);

  auto exportEvent = efactory->create_plain_txt_2_folder_export(newFileName);

  mwctx->actx->eventer->submit(exportEvent);

  ops->update_statuses();

  ops->hide_spinner();
}

bool ExportEventsHandler::has_to_export()
{
  assert(mwctx != nullptr);

  return mwctx != nullptr && (!mwctx->imagesVDB.empty() ||
                              (mwctx->images_provider != nullptr &&
                               !mwctx->images_provider->get_db_path().empty()));
}

void ExportEventsHandler::handle_export_yolo4()
{
  assert(mwctx != nullptr);
  assert(mwctx->cwFactory != nullptr);

  if (!has_to_export()) {
    const std::string errmsg = "No images found or db opened to export!";
    ops->show_error_dialog(errmsg);
    return;
  }

  ops->show_spinner();

  auto dialog = mwctx->cwFactory->create_yolo4_export_folder_choose_dialog(
      mwctx->wloader->get_window());

  const int result = dialog->run();

  if (result != Gtk::RESPONSE_OK) {
    LOGD("Dialog is closed");
    ops->hide_spinner();
    return;
  }

  const auto newFileName = dialog->get_filename();

  LOGD("Selected yolo4 export folder: " << newFileName);

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
    const std::string errmsg = "No images found or db opened to export!";
    ops->show_error_dialog(errmsg);
    return;
  }

  ops->show_spinner();

  auto dialog = mwctx->cwFactory->create_pytorch_export_folder_choose_dialog(
      mwctx->wloader->get_window());

  const int result = dialog->run();

  if (result != Gtk::RESPONSE_OK) {
    LOGD("Dialog is closed");
    ops->hide_spinner();
    return;
  }

  const auto newFileName = dialog->get_filename();

  LOGD("Selected yolo4 export folder: " << newFileName);

  auto efactory = mwctx->actx->eventer->get_events_factory();

  assert(efactory != nullptr);

  auto exportEvent =
      efactory->create_pytorch_vision_export_request(newFileName);

  assert(exportEvent != nullptr);

  exportEvent->cropper = std::make_shared<helpers::GtkmmImageCropperProvider>();

  mwctx->actx->eventer->submit(exportEvent);

  ops->update_statuses();

  ops->hide_spinner();
}

}  // namespace templateGtkmm3::window::event_handlers
