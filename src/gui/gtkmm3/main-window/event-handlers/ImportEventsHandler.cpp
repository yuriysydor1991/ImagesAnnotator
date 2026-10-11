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

#include "src/gui/gtkmm3/main-window/event-handlers/ImportEventsHandler.h"

#include <cassert>
#include <functional>
#include <memory>
#include <string>

#include "src/gui/gtkmm-common/GtkmmImageSizeProvider.h"
#include "src/gui/gtkmm3/MainWindowContext.h"
#include "src/gui/gtkmm3/main-window/event-handlers/WindowOps.h"
#include "src/log/log.h"

namespace imagesAnnotatorGtkmm3::window::event_handlers
{

ImportEventsHandler::ImportEventsHandler(std::shared_ptr<WindowOps> nops)
    : AbstractWindowEventHandler(nops)
{
}

bool ImportEventsHandler::subscribe()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);

  mwctx->wloader->get_import_txt_folder_mi()->signal_activate().connect(
      sigc::mem_fun(*this, &ImportEventsHandler::handle_import_txt));
  mwctx->wloader->get_import_yolo4_folder_mi()->signal_activate().connect(
      sigc::mem_fun(*this, &ImportEventsHandler::handle_import_yolo4));
  mwctx->wloader->get_import_ultralytics_detect_folder_mi()
      ->signal_activate()
      .connect(sigc::mem_fun(
          *this, &ImportEventsHandler::handle_import_ultralytics_detect));
  mwctx->wloader->get_import_ultralytics_obb_folder_mi()
      ->signal_activate()
      .connect(sigc::mem_fun(
          *this, &ImportEventsHandler::handle_import_ultralytics_obb));
  mwctx->wloader->get_import_ultralytics_segment_folder_mi()
      ->signal_activate()
      .connect(sigc::mem_fun(
          *this, &ImportEventsHandler::handle_import_ultralytics_segment));
  mwctx->wloader->get_import_coco_folder_mi()->signal_activate().connect(
      sigc::mem_fun(*this, &ImportEventsHandler::handle_import_coco));
  mwctx->wloader->get_import_pascal_voc_folder_mi()->signal_activate().connect(
      sigc::mem_fun(*this, &ImportEventsHandler::handle_import_pascal_voc));
  mwctx->wloader->get_import_createml_folder_mi()->signal_activate().connect(
      sigc::mem_fun(*this, &ImportEventsHandler::handle_import_createml));
  mwctx->wloader->get_import_pytorchvision_folder_mi()
      ->signal_activate()
      .connect(
          sigc::mem_fun(*this, &ImportEventsHandler::handle_import_pytorch));
  mwctx->wloader->get_import_dxxwjz2_folder_mi()->signal_activate().connect(
      sigc::mem_fun(*this, &ImportEventsHandler::handle_import_dxxwjz2));
  mwctx->wloader->get_import_dxxwjz2p_project_mi()->signal_activate().connect(
      sigc::mem_fun(*this, &ImportEventsHandler::handle_import_dxxwjz2p));
  mwctx->wloader->get_import_dxxwjz3_folder_mi()->signal_activate().connect(
      sigc::mem_fun(*this, &ImportEventsHandler::handle_import_dxxwjz3));
  mwctx->wloader->get_import_dxxwjz3p_project_mi()->signal_activate().connect(
      sigc::mem_fun(*this, &ImportEventsHandler::handle_import_dxxwjz3p));

  return true;
}

events::events::IImageSizeFacilityProviderPtr
ImportEventsHandler::create_image_sizer()
{
  return std::make_shared<imagesAnnotatorGtkmmCommon::GtkmmImageSizeProvider>();
}

void ImportEventsHandler::handle_folder_import(
    std::shared_ptr<Gtk::FileChooserDialog> dialog,
    const std::function<void(const std::string&)>& submitImport)
{
  assert(dialog != nullptr);

  // No has_to_export() counterpart here on purpose: an import is what gives
  // the application something to show, so it is offered over an empty project
  // just as much as over a loaded one.
  ops->show_spinner();

  const int result = dialog->run();

  if (result != Gtk::RESPONSE_OK) {
    LOGD("Dialog is closed");
    ops->hide_spinner();
    return;
  }

  const std::string newFileName = dialog->get_filename();

  LOGD("Selected import folder or project file: " << newFileName);

  submitImport(newFileName);

  ops->update_statuses();

  ops->hide_spinner();
}

void ImportEventsHandler::handle_import_txt()
{
  assert(mwctx != nullptr);
  assert(mwctx->cwFactory != nullptr);

  handle_folder_import(
      mwctx->cwFactory->create_txt_import_folder_choose_dialog(
          mwctx->wloader->get_window()),
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
  assert(mwctx->cwFactory != nullptr);

  handle_folder_import(
      mwctx->cwFactory->create_yolo4_import_folder_choose_dialog(
          mwctx->wloader->get_window()),
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
  assert(mwctx->cwFactory != nullptr);

  handle_folder_import(
      mwctx->cwFactory->create_ultralytics_detect_import_folder_choose_dialog(
          mwctx->wloader->get_window()),
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
  assert(mwctx->cwFactory != nullptr);

  handle_folder_import(
      mwctx->cwFactory->create_ultralytics_obb_import_folder_choose_dialog(
          mwctx->wloader->get_window()),
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
  assert(mwctx->cwFactory != nullptr);

  handle_folder_import(
      mwctx->cwFactory->create_ultralytics_segment_import_folder_choose_dialog(
          mwctx->wloader->get_window()),
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
  assert(mwctx->cwFactory != nullptr);

  handle_folder_import(
      mwctx->cwFactory->create_coco_import_folder_choose_dialog(
          mwctx->wloader->get_window()),
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
  assert(mwctx->cwFactory != nullptr);

  handle_folder_import(
      mwctx->cwFactory->create_pascal_voc_import_folder_choose_dialog(
          mwctx->wloader->get_window()),
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
  assert(mwctx->cwFactory != nullptr);

  handle_folder_import(
      mwctx->cwFactory->create_createml_import_folder_choose_dialog(
          mwctx->wloader->get_window()),
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
  assert(mwctx->cwFactory != nullptr);

  handle_folder_import(
      mwctx->cwFactory->create_pytorch_import_folder_choose_dialog(
          mwctx->wloader->get_window()),
      [this](const std::string& folder) {
        auto efactory = mwctx->actx->eventer->get_events_factory();

        assert(efactory != nullptr);

        auto importEvent =
            efactory->create_pytorch_vision_import_request(folder);

        importEvent->sizer = create_image_sizer();

        mwctx->actx->eventer->submit(importEvent);
      });
}

void ImportEventsHandler::handle_import_dxxwjz2()
{
  assert(mwctx != nullptr);
  assert(mwctx->cwFactory != nullptr);

  handle_folder_import(
      mwctx->cwFactory->create_dxxwjz2_import_folder_choose_dialog(
          mwctx->wloader->get_window()),
      [this](const std::string& folder) {
        auto efactory = mwctx->actx->eventer->get_events_factory();

        assert(efactory != nullptr);

        auto importEvent = efactory->create_dxxwjz2_import_request(folder);

        importEvent->sizer = create_image_sizer();

        mwctx->actx->eventer->submit(importEvent);
      });
}

void ImportEventsHandler::handle_import_dxxwjz2p()
{
  assert(mwctx != nullptr);
  assert(mwctx->cwFactory != nullptr);

  handle_folder_import(
      mwctx->cwFactory->create_dxxwjz2p_import_file_choose_dialog(
          mwctx->wloader->get_window()),
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
  assert(mwctx->cwFactory != nullptr);

  handle_folder_import(
      mwctx->cwFactory->create_dxxwjz3_import_folder_choose_dialog(
          mwctx->wloader->get_window()),
      [this](const std::string& folder) {
        auto efactory = mwctx->actx->eventer->get_events_factory();

        assert(efactory != nullptr);

        auto importEvent = efactory->create_dxxwjz3_import_request(folder);

        importEvent->sizer = create_image_sizer();

        mwctx->actx->eventer->submit(importEvent);
      });
}

void ImportEventsHandler::handle_import_dxxwjz3p()
{
  assert(mwctx != nullptr);
  assert(mwctx->cwFactory != nullptr);

  handle_folder_import(
      mwctx->cwFactory->create_dxxwjz3p_import_file_choose_dialog(
          mwctx->wloader->get_window()),
      [this](const std::string& projectFile) {
        auto efactory = mwctx->actx->eventer->get_events_factory();

        assert(efactory != nullptr);

        auto importEvent =
            efactory->create_dxxwjz3p_import_request(projectFile);

        importEvent->sizer = create_image_sizer();

        mwctx->actx->eventer->submit(importEvent);
      });
}

}  // namespace imagesAnnotatorGtkmm3::window::event_handlers
