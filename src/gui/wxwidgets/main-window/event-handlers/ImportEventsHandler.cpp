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

#include "src/gui/wxwidgets/main-window/event-handlers/ImportEventsHandler.h"

#include <cassert>
#include <functional>
#include <memory>
#include <string>

#include "src/gui/wxwidgets/MainWindowContext.h"
#include "src/gui/wxwidgets/main-window/WindowDataContext.h"
#include "src/gui/wxwidgets/main-window/custom-widgets/WxImageSizeProvider.h"
#include "src/gui/wxwidgets/main-window/event-handlers/WindowOps.h"
#include "src/log/log.h"

namespace imagesAnnotatorWx::window::event_handlers
{

ImportEventsHandler::ImportEventsHandler(std::shared_ptr<WindowOps> nops)
    : AbstractWindowEventHandler(nops)
{
}

bool ImportEventsHandler::subscribe()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);

  auto* frame = mwctx->wloader->get_window();

  frame->Bind(
      wxEVT_MENU, [this](wxCommandEvent&) { handle_import_txt(); },
      WindowDataContext::ID_IMPORT_TXT);
  frame->Bind(
      wxEVT_MENU, [this](wxCommandEvent&) { handle_import_yolo4(); },
      WindowDataContext::ID_IMPORT_YOLO4);
  frame->Bind(
      wxEVT_MENU,
      [this](wxCommandEvent&) { handle_import_ultralytics_detect(); },
      WindowDataContext::ID_IMPORT_ULTRALYTICS_DETECT);
  frame->Bind(
      wxEVT_MENU, [this](wxCommandEvent&) { handle_import_ultralytics_obb(); },
      WindowDataContext::ID_IMPORT_ULTRALYTICS_OBB);
  frame->Bind(
      wxEVT_MENU,
      [this](wxCommandEvent&) { handle_import_ultralytics_segment(); },
      WindowDataContext::ID_IMPORT_ULTRALYTICS_SEGMENT);
  frame->Bind(
      wxEVT_MENU, [this](wxCommandEvent&) { handle_import_coco(); },
      WindowDataContext::ID_IMPORT_COCO);
  frame->Bind(
      wxEVT_MENU, [this](wxCommandEvent&) { handle_import_pascal_voc(); },
      WindowDataContext::ID_IMPORT_PASCAL_VOC);
  frame->Bind(
      wxEVT_MENU, [this](wxCommandEvent&) { handle_import_createml(); },
      WindowDataContext::ID_IMPORT_CREATEML);
  frame->Bind(
      wxEVT_MENU, [this](wxCommandEvent&) { handle_import_pytorch(); },
      WindowDataContext::ID_IMPORT_PYTORCH);

  return true;
}

events::events::IImageSizeFacilityProviderPtr
ImportEventsHandler::create_image_sizer()
{
  return std::make_shared<
      imagesAnnotatorWx::window::custom_widgets::WxImageSizeProvider>();
}

void ImportEventsHandler::handle_folder_import(
    const std::function<bool(std::string&)>& askFolder,
    const std::function<void(const std::string&)>& submitImport)
{
  // No has_to_export() counterpart here on purpose: an import is what gives
  // the application something to show, so it is offered over an empty project
  // just as much as over a loaded one.
  std::string newFileName;

  if (!askFolder(newFileName)) {
    LOGD("Dialog is closed");
    return;
  }

  LOGD("Selected import folder: " << newFileName);

  ops->show_spinner();

  submitImport(newFileName);

  ops->update_statuses();

  ops->hide_spinner();
}

void ImportEventsHandler::handle_import_txt()
{
  assert(mwctx != nullptr);
  assert(mwctx->cwFactory != nullptr);

  handle_folder_import(
      [this](std::string& out) {
        return mwctx->cwFactory->ask_txt_import_folder(
            mwctx->wloader->get_window(), out);
      },
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
      [this](std::string& out) {
        return mwctx->cwFactory->ask_yolo4_import_folder(
            mwctx->wloader->get_window(), out);
      },
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
      [this](std::string& out) {
        return mwctx->cwFactory->ask_ultralytics_detect_import_folder(
            mwctx->wloader->get_window(), out);
      },
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
      [this](std::string& out) {
        return mwctx->cwFactory->ask_ultralytics_obb_import_folder(
            mwctx->wloader->get_window(), out);
      },
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
      [this](std::string& out) {
        return mwctx->cwFactory->ask_ultralytics_segment_import_folder(
            mwctx->wloader->get_window(), out);
      },
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
      [this](std::string& out) {
        return mwctx->cwFactory->ask_coco_import_folder(
            mwctx->wloader->get_window(), out);
      },
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
      [this](std::string& out) {
        return mwctx->cwFactory->ask_pascal_voc_import_folder(
            mwctx->wloader->get_window(), out);
      },
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
      [this](std::string& out) {
        return mwctx->cwFactory->ask_createml_import_folder(
            mwctx->wloader->get_window(), out);
      },
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
      [this](std::string& out) {
        return mwctx->cwFactory->ask_pytorch_import_folder(
            mwctx->wloader->get_window(), out);
      },
      [this](const std::string& folder) {
        auto efactory = mwctx->actx->eventer->get_events_factory();

        assert(efactory != nullptr);

        auto importEvent =
            efactory->create_pytorch_vision_import_request(folder);

        importEvent->sizer = create_image_sizer();

        mwctx->actx->eventer->submit(importEvent);
      });
}

}  // namespace imagesAnnotatorWx::window::event_handlers
