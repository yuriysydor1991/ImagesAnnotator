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

#include "src/gui/gtkmm4/main-window/event-handlers/ExportEventsHandler.h"

#include <cassert>
#include <functional>
#include <memory>
#include <string>

#include "src/gui/gtkmm-common/GtkmmImageCropperProvider.h"
#include "src/gui/gtkmm4/MainWindowContext.h"
#include "src/gui/gtkmm4/main-window/event-handlers/WindowOps.h"
#include "src/log/log.h"

namespace imagesAnnotatorGtkmm4::window::event_handlers
{

ExportEventsHandler::ExportEventsHandler(std::shared_ptr<WindowOps> nops)
    : AbstractWindowEventHandler(nops)
{
}

bool ExportEventsHandler::subscribe()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);

  // GTK4 menu entries -> Gio::Action activations.
  mwctx->wloader->get_export_txt_action()->signal_activate().connect(
      [this]([[maybe_unused]] const Glib::VariantBase& param) {
        handle_export_txt();
      });

  mwctx->wloader->get_export_yolo4_action()->signal_activate().connect(
      [this]([[maybe_unused]] const Glib::VariantBase& param) {
        handle_export_yolo4();
      });

  mwctx->wloader->get_export_pytorchvision_action()->signal_activate().connect(
      [this]([[maybe_unused]] const Glib::VariantBase& param) {
        handle_export_pytorch();
      });

  mwctx->wloader->get_export_ultralytics_detect_action()
      ->signal_activate()
      .connect([this]([[maybe_unused]] const Glib::VariantBase& param) {
        handle_export_ultralytics_detect();
      });
  mwctx->wloader->get_export_ultralytics_obb_action()
      ->signal_activate()
      .connect([this]([[maybe_unused]] const Glib::VariantBase& param) {
        handle_export_ultralytics_obb();
      });
  mwctx->wloader->get_export_ultralytics_segment_action()
      ->signal_activate()
      .connect([this]([[maybe_unused]] const Glib::VariantBase& param) {
        handle_export_ultralytics_segment();
      });
  mwctx->wloader->get_export_coco_action()->signal_activate().connect(
      [this]([[maybe_unused]] const Glib::VariantBase& param) {
        handle_export_coco();
      });
  mwctx->wloader->get_export_pascal_voc_action()->signal_activate().connect(
      [this]([[maybe_unused]] const Glib::VariantBase& param) {
        handle_export_pascal_voc();
      });
  mwctx->wloader->get_export_createml_action()->signal_activate().connect(
      [this]([[maybe_unused]] const Glib::VariantBase& param) {
        handle_export_createml();
      });

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

  auto* dialog = mwctx->wloader->get_txt_export_folder_dialog();

  const int result = WindowOps::run_dialog(*dialog);

  if (result != Gtk::ResponseType::OK) {
    LOGD("Dialog is closed");
    ops->hide_spinner();
    return;
  }

  const std::string newFileName = dialog->get_file()->get_path();

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

  auto* dialog = mwctx->wloader->get_yolo4_export_folder_dialog();

  const int result = WindowOps::run_dialog(*dialog);

  if (result != Gtk::ResponseType::OK) {
    LOGD("Dialog is closed");
    ops->hide_spinner();
    return;
  }

  const std::string newFileName = dialog->get_file()->get_path();

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

  auto* dialog = mwctx->wloader->get_pytorch_export_folder_dialog();

  const int result = WindowOps::run_dialog(*dialog);

  if (result != Gtk::ResponseType::OK) {
    LOGD("Dialog is closed");
    ops->hide_spinner();
    return;
  }

  const std::string newFileName = dialog->get_file()->get_path();

  LOGD("Selected yolo4 export folder: " << newFileName);

  auto efactory = mwctx->actx->eventer->get_events_factory();

  assert(efactory != nullptr);

  auto exportEvent =
      efactory->create_pytorch_vision_export_request(newFileName);

  assert(exportEvent != nullptr);

  exportEvent->cropper =
      std::make_shared<imagesAnnotatorGtkmmCommon::GtkmmImageCropperProvider>();

  mwctx->actx->eventer->submit(exportEvent);

  ops->update_statuses();

  ops->hide_spinner();
}

void ExportEventsHandler::handle_folder_export(
    Gtk::FileChooserDialog* dialog,
    const std::function<void(const std::string&)>& submitExport)
{
  assert(dialog != nullptr);

  if (!has_to_export()) {
    const std::string errmsg = "No images found or db opened to export!";
    ops->show_error_dialog(errmsg);
    return;
  }

  ops->show_spinner();

  const int result = WindowOps::run_dialog(*dialog);

  if (result != Gtk::ResponseType::OK) {
    LOGD("Dialog is closed");
    ops->hide_spinner();
    return;
  }

  const std::string newFileName = dialog->get_file()->get_path();

  LOGD("Selected export folder: " << newFileName);

  submitExport(newFileName);

  ops->update_statuses();

  ops->hide_spinner();
}

void ExportEventsHandler::handle_export_ultralytics_detect()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);

  handle_folder_export(
      mwctx->wloader->get_ultralytics_detect_export_folder_dialog(),
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
  assert(mwctx->wloader != nullptr);

  handle_folder_export(
      mwctx->wloader->get_ultralytics_obb_export_folder_dialog(),
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
  assert(mwctx->wloader != nullptr);

  handle_folder_export(
      mwctx->wloader->get_ultralytics_segment_export_folder_dialog(),
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
  assert(mwctx->wloader != nullptr);

  handle_folder_export(mwctx->wloader->get_coco_export_folder_dialog(),
                       [this](const std::string& folder) {
                         auto efactory =
                             mwctx->actx->eventer->get_events_factory();

                         assert(efactory != nullptr);

                         mwctx->actx->eventer->submit(
                             efactory->create_coco_export_request(folder));
                       });
}

void ExportEventsHandler::handle_export_pascal_voc()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);

  handle_folder_export(
      mwctx->wloader->get_pascal_voc_export_folder_dialog(),
      [this](const std::string& folder) {
        auto efactory = mwctx->actx->eventer->get_events_factory();

        assert(efactory != nullptr);

        mwctx->actx->eventer->submit(
            efactory->create_pascal_voc_export_request(folder));
      });
}

void ExportEventsHandler::handle_export_createml()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);

  handle_folder_export(mwctx->wloader->get_createml_export_folder_dialog(),
                       [this](const std::string& folder) {
                         auto efactory =
                             mwctx->actx->eventer->get_events_factory();

                         assert(efactory != nullptr);

                         mwctx->actx->eventer->submit(
                             efactory->create_createml_export_request(folder));
                       });
}

}  // namespace imagesAnnotatorGtkmm4::window::event_handlers
