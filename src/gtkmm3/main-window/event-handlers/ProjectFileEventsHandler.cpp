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

#include "src/gtkmm3/main-window/event-handlers/ProjectFileEventsHandler.h"

#include <cassert>
#include <memory>

#include "src/gtkmm3/MainWindowContext.h"
#include "src/gtkmm3/main-window/event-handlers/WindowOps.h"
#include "src/log/log.h"

namespace templateGtkmm3::window::event_handlers
{

ProjectFileEventsHandler::ProjectFileEventsHandler(
    std::shared_ptr<WindowOps> nops)
    : AbstractWindowEventHandler(nops)
{
}

bool ProjectFileEventsHandler::subscribe()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);

  auto* oAnnB = mwctx->wloader->get_annotations_db_open_button();
  auto* imagesFolderB = mwctx->wloader->get_images_folder_open_button();
  auto* miOpenImagesF = mwctx->wloader->get_images_open_mi();
  auto* miOpenAnnotationsF = mwctx->wloader->get_annotations_db_open_mi();
  auto* annSaveM = mwctx->wloader->get_annotations_db_save_mi();
  auto* annSaveAsM = mwctx->wloader->get_annotations_db_saveas_mi();
  auto* annClose = mwctx->wloader->get_annotations_project_close_mi();

  oAnnB->signal_clicked().connect(sigc::mem_fun(
      *this, &ProjectFileEventsHandler::handle_annotations_db_open));
  imagesFolderB->signal_clicked().connect(
      sigc::mem_fun(*this, &ProjectFileEventsHandler::handle_images_dir_open));

  miOpenImagesF->signal_activate().connect(sigc::mem_fun(
      *this, &ProjectFileEventsHandler::handle_menu_images_folder_open));
  miOpenAnnotationsF->signal_activate().connect(sigc::mem_fun(
      *this, &ProjectFileEventsHandler::handle_menu_annotations_db_open));
  annSaveM->signal_activate().connect(
      sigc::mem_fun(*this, &ProjectFileEventsHandler::handle_menu_save));
  annSaveAsM->signal_activate().connect(
      sigc::mem_fun(*this, &ProjectFileEventsHandler::handle_menu_saveas));
  annClose->signal_activate().connect(sigc::mem_fun(
      *this, &ProjectFileEventsHandler::handle_menu_project_close));

  return true;
}

void ProjectFileEventsHandler::handle_annotations_db_open()
{
  LOGT("Open new annotations dir");

  ops->show_spinner();

  if (ops->lastChangedStatus) {
    if (!ops->ask_about_unsaved_changes()) {
      LOGD("User interupted data changes discard");
      return;
    }
  }

  auto dialog =
      mwctx->cwFactory->create_json_db_dialog(mwctx->wloader->get_window());

  const int result = dialog->run();

  if (result != Gtk::RESPONSE_OK) {
    LOGD("Dialog is closed");
    ops->hide_spinner();
    return;
  }

  const auto newFileName = dialog->get_filename();

  LOGD("Selected json db file: " << newFileName);

  mwctx->actx->eventer->onAnnotationsDirChanged(newFileName);

  ops->update_statuses();
}

void ProjectFileEventsHandler::handle_images_dir_open()
{
  LOGT("Open new images dir");

  ops->show_spinner();

  auto dialog = mwctx->cwFactory->create_folder_choose_dialog(
      mwctx->wloader->get_window());

  const int result = dialog->run();

  if (result != Gtk::RESPONSE_OK) {
    LOGD("Dialog is closed");
    ops->hide_spinner();
    return;
  }

  LOGD("Selected new images directory: " << dialog->get_filename());

  mwctx->actx->eventer->onImagesDirChanged(dialog->get_filename());

  ops->update_statuses();
}

void ProjectFileEventsHandler::handle_menu_images_folder_open()
{
  handle_images_dir_open();
}

void ProjectFileEventsHandler::handle_menu_annotations_db_open()
{
  handle_annotations_db_open();
}

void ProjectFileEventsHandler::handle_menu_save()
{
  ops->save_project();
}

void ProjectFileEventsHandler::handle_menu_saveas()
{
  ops->save_project_as();
}

void ProjectFileEventsHandler::handle_menu_project_close()
{
  LOGT("Trying to close the project project");

  if (ops->lastChangedStatus) {
    if (!ops->ask_about_unsaved_changes()) {
      LOGT("User doesn't want to close project anymore");
      return;
    }
  }

  ops->dragging = false;
  ops->lastChangedStatus = false;

  auto ef = mwctx->actx->eventer->get_events_factory();

  auto pcloseE = ef->create_close_event();

  mwctx->actx->eventer->submit(pcloseE);

  ops->clean_list_box(mwctx->wloader->get_images_list());
  ops->clean_list_box(mwctx->wloader->get_annotations_db_list());
  ops->clean_list_box(mwctx->wloader->get_current_image_annotations());

  mwctx->wloader->get_annotation_search_entry()->set_text("");
  mwctx->wloader->get_edit_current_rect_entry()->set_text("");

  mwctx->annotationsList.clear();
  mwctx->currentVisualRects.clear();
  mwctx->imagesVDB.clear();
  mwctx->centralCanvas->clear();
  mwctx->annotationsList.clear();
  mwctx->current_annotation_name.reset();

  ops->update_statuses();
}

}  // namespace templateGtkmm3::window::event_handlers
