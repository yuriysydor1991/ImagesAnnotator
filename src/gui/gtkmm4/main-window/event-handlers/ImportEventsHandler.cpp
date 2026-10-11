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

#include "src/gui/gtkmm4/main-window/event-handlers/ImportEventsHandler.h"

#include <cassert>
#include <functional>
#include <memory>
#include <string>

#include "src/gui/gtkmm-common/GtkmmImageSizeProvider.h"
#include "src/gui/gtkmm4/MainWindowContext.h"
#include "src/gui/gtkmm4/main-window/event-handlers/WindowOps.h"
#include "src/log/log.h"

namespace imagesAnnotatorGtkmm4::window::event_handlers
{

ImportEventsHandler::ImportEventsHandler(std::shared_ptr<WindowOps> nops)
    : AbstractWindowEventHandler(nops)
{
}

bool ImportEventsHandler::subscribe()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);

  // GTK4 menu entries -> Gio::Action activations.
  mwctx->wloader->get_import_txt_action()->signal_activate().connect(
      [this]([[maybe_unused]] const Glib::VariantBase& param) {
        handle_import_txt();
      });

  mwctx->wloader->get_import_yolo4_action()->signal_activate().connect(
      [this]([[maybe_unused]] const Glib::VariantBase& param) {
        handle_import_yolo4();
      });

  mwctx->wloader->get_import_ultralytics_detect_action()
      ->signal_activate()
      .connect([this]([[maybe_unused]] const Glib::VariantBase& param) {
        handle_import_ultralytics_detect();
      });

  mwctx->wloader->get_import_ultralytics_obb_action()
      ->signal_activate()
      .connect([this]([[maybe_unused]] const Glib::VariantBase& param) {
        handle_import_ultralytics_obb();
      });

  mwctx->wloader->get_import_ultralytics_segment_action()
      ->signal_activate()
      .connect([this]([[maybe_unused]] const Glib::VariantBase& param) {
        handle_import_ultralytics_segment();
      });

  mwctx->wloader->get_import_coco_action()->signal_activate().connect(
      [this]([[maybe_unused]] const Glib::VariantBase& param) {
        handle_import_coco();
      });

  mwctx->wloader->get_import_pascal_voc_action()->signal_activate().connect(
      [this]([[maybe_unused]] const Glib::VariantBase& param) {
        handle_import_pascal_voc();
      });

  mwctx->wloader->get_import_createml_action()->signal_activate().connect(
      [this]([[maybe_unused]] const Glib::VariantBase& param) {
        handle_import_createml();
      });

  mwctx->wloader->get_import_pytorchvision_action()->signal_activate().connect(
      [this]([[maybe_unused]] const Glib::VariantBase& param) {
        handle_import_pytorch();
      });

  mwctx->wloader->get_import_dxxwjz3p_action()->signal_activate().connect(
      [this]([[maybe_unused]] const Glib::VariantBase& param) {
        handle_import_dxxwjz3p();
      });

  mwctx->wloader->get_import_dxxwjz2p_action()->signal_activate().connect(
      [this]([[maybe_unused]] const Glib::VariantBase& param) {
        handle_import_dxxwjz2p();
      });

  mwctx->wloader->get_import_dxxwjz3_action()->signal_activate().connect(
      [this]([[maybe_unused]] const Glib::VariantBase& param) {
        handle_import_dxxwjz3();
      });

  mwctx->wloader->get_import_dxxwjz2_action()->signal_activate().connect(
      [this]([[maybe_unused]] const Glib::VariantBase& param) {
        handle_import_dxxwjz2();
      });

  return true;
}

events::events::IImageSizeFacilityProviderPtr
ImportEventsHandler::create_image_sizer()
{
  return std::make_shared<imagesAnnotatorGtkmmCommon::GtkmmImageSizeProvider>();
}

void ImportEventsHandler::handle_folder_import(
    Gtk::FileChooserDialog* dialog,
    const std::function<void(const std::string&)>& submitImport)
{
  assert(dialog != nullptr);

  // No has_to_export() counterpart here on purpose: an import is what gives
  // the application something to show, so it is offered over an empty project
  // just as much as over a loaded one.
  ops->show_spinner();

  const int result = WindowOps::run_dialog(*dialog);

  if (result != Gtk::ResponseType::OK) {
    LOGD("Dialog is closed");
    ops->hide_spinner();
    return;
  }

  const std::string newFileName = dialog->get_file()->get_path();

  LOGD("Selected import folder or project file: " << newFileName);

  submitImport(newFileName);

  ops->update_statuses();

  ops->hide_spinner();
}

void ImportEventsHandler::handle_import_txt()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);

  handle_folder_import(
      mwctx->wloader->get_txt_import_folder_dialog(),
      [this](const std::string& folder) {
        auto efactory = mwctx->actx->eventer->get_events_factory();

        assert(efactory != nullptr);

        auto importEvent = efactory->create_plain_txt_import_request(folder);

        importEvent->sizer = create_image_sizer();

        mwctx->actx->eventer->submit(importEvent);
      });
}

void ImportEventsHandler::handle_import_yolo4()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);

  handle_folder_import(
      mwctx->wloader->get_yolo4_import_folder_dialog(),
      [this](const std::string& folder) {
        auto efactory = mwctx->actx->eventer->get_events_factory();

        assert(efactory != nullptr);

        auto importEvent = efactory->create_yolo4_import_request(folder);

        importEvent->sizer = create_image_sizer();

        mwctx->actx->eventer->submit(importEvent);
      });
}

void ImportEventsHandler::handle_import_ultralytics_detect()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);

  handle_folder_import(
      mwctx->wloader->get_ultralytics_detect_import_folder_dialog(),
      [this](const std::string& folder) {
        auto efactory = mwctx->actx->eventer->get_events_factory();

        assert(efactory != nullptr);

        auto importEvent =
            efactory->create_ultralytics_detect_import_request(folder);

        importEvent->sizer = create_image_sizer();

        mwctx->actx->eventer->submit(importEvent);
      });
}

void ImportEventsHandler::handle_import_ultralytics_obb()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);

  handle_folder_import(
      mwctx->wloader->get_ultralytics_obb_import_folder_dialog(),
      [this](const std::string& folder) {
        auto efactory = mwctx->actx->eventer->get_events_factory();

        assert(efactory != nullptr);

        auto importEvent =
            efactory->create_ultralytics_obb_import_request(folder);

        importEvent->sizer = create_image_sizer();

        mwctx->actx->eventer->submit(importEvent);
      });
}

void ImportEventsHandler::handle_import_ultralytics_segment()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);

  handle_folder_import(
      mwctx->wloader->get_ultralytics_segment_import_folder_dialog(),
      [this](const std::string& folder) {
        auto efactory = mwctx->actx->eventer->get_events_factory();

        assert(efactory != nullptr);

        auto importEvent =
            efactory->create_ultralytics_segment_import_request(folder);

        importEvent->sizer = create_image_sizer();

        mwctx->actx->eventer->submit(importEvent);
      });
}

void ImportEventsHandler::handle_import_coco()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);

  handle_folder_import(
      mwctx->wloader->get_coco_import_folder_dialog(),
      [this](const std::string& folder) {
        auto efactory = mwctx->actx->eventer->get_events_factory();

        assert(efactory != nullptr);

        auto importEvent = efactory->create_coco_import_request(folder);

        importEvent->sizer = create_image_sizer();

        mwctx->actx->eventer->submit(importEvent);
      });
}

void ImportEventsHandler::handle_import_pascal_voc()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);

  handle_folder_import(
      mwctx->wloader->get_pascal_voc_import_folder_dialog(),
      [this](const std::string& folder) {
        auto efactory = mwctx->actx->eventer->get_events_factory();

        assert(efactory != nullptr);

        auto importEvent = efactory->create_pascal_voc_import_request(folder);

        importEvent->sizer = create_image_sizer();

        mwctx->actx->eventer->submit(importEvent);
      });
}

void ImportEventsHandler::handle_import_createml()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);

  handle_folder_import(
      mwctx->wloader->get_createml_import_folder_dialog(),
      [this](const std::string& folder) {
        auto efactory = mwctx->actx->eventer->get_events_factory();

        assert(efactory != nullptr);

        auto importEvent = efactory->create_createml_import_request(folder);

        importEvent->sizer = create_image_sizer();

        mwctx->actx->eventer->submit(importEvent);
      });
}

void ImportEventsHandler::handle_import_pytorch()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);

  handle_folder_import(
      mwctx->wloader->get_pytorch_import_folder_dialog(),
      [this](const std::string& folder) {
        auto efactory = mwctx->actx->eventer->get_events_factory();

        assert(efactory != nullptr);

        auto importEvent =
            efactory->create_pytorch_vision_import_request(folder);

        importEvent->sizer = create_image_sizer();

        mwctx->actx->eventer->submit(importEvent);
      });
}

void ImportEventsHandler::handle_import_dxxwjz3p()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);

  handle_folder_import(
      mwctx->wloader->get_dxxwjz3p_import_file_dialog(),
      [this](const std::string& projectFile) {
        auto efactory = mwctx->actx->eventer->get_events_factory();

        assert(efactory != nullptr);

        auto importEvent =
            efactory->create_dxxwjz3p_import_request(projectFile);

        importEvent->sizer = create_image_sizer();

        mwctx->actx->eventer->submit(importEvent);
      });
}

void ImportEventsHandler::handle_import_dxxwjz2p()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);

  handle_folder_import(
      mwctx->wloader->get_dxxwjz2p_import_file_dialog(),
      [this](const std::string& projectFile) {
        auto efactory = mwctx->actx->eventer->get_events_factory();

        assert(efactory != nullptr);

        auto importEvent =
            efactory->create_dxxwjz2p_import_request(projectFile);

        importEvent->sizer = create_image_sizer();

        mwctx->actx->eventer->submit(importEvent);
      });
}

void ImportEventsHandler::handle_import_dxxwjz3()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);

  handle_folder_import(
      mwctx->wloader->get_dxxwjz3_import_folder_dialog(),
      [this](const std::string& folder) {
        auto efactory = mwctx->actx->eventer->get_events_factory();

        assert(efactory != nullptr);

        auto importEvent = efactory->create_dxxwjz3_import_request(folder);

        importEvent->sizer = create_image_sizer();

        mwctx->actx->eventer->submit(importEvent);
      });
}

void ImportEventsHandler::handle_import_dxxwjz2()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);

  handle_folder_import(
      mwctx->wloader->get_dxxwjz2_import_folder_dialog(),
      [this](const std::string& folder) {
        auto efactory = mwctx->actx->eventer->get_events_factory();

        assert(efactory != nullptr);

        auto importEvent = efactory->create_dxxwjz2_import_request(folder);

        importEvent->sizer = create_image_sizer();

        mwctx->actx->eventer->submit(importEvent);
      });
}

}  // namespace imagesAnnotatorGtkmm4::window::event_handlers
