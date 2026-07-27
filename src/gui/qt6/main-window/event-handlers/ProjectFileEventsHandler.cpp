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

#include "src/gui/qt6/main-window/event-handlers/ProjectFileEventsHandler.h"

#include <cassert>
#include <string>

#include "src/gui/qt6/MainWindowContext.h"
#include "src/gui/qt6/main-window/WindowDataContext.h"
#include "src/gui/qt6/main-window/event-handlers/WindowOps.h"
#include "src/gui/qt6/qt_includes.h"
#include "src/log/log.h"

namespace templateQt6::window::event_handlers
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

  auto* window = mwctx->wloader->get_window();

  QObject::connect(mwctx->wloader->get_annotations_db_open_button(),
                   &QPushButton::clicked, window,
                   [this]() { handle_annotations_db_open(); });
  QObject::connect(mwctx->wloader->get_images_folder_open_button(),
                   &QPushButton::clicked, window,
                   [this]() { handle_images_dir_open(); });

  QObject::connect(mwctx->wloader->get_images_open_mi(), &QAction::triggered,
                   window, [this]() { handle_menu_images_folder_open(); });
  QObject::connect(mwctx->wloader->get_annotations_db_open_mi(),
                   &QAction::triggered, window,
                   [this]() { handle_menu_annotations_db_open(); });
  QObject::connect(mwctx->wloader->get_annotations_db_save_mi(),
                   &QAction::triggered, window,
                   [this]() { handle_menu_save(); });
  QObject::connect(mwctx->wloader->get_annotations_db_saveas_mi(),
                   &QAction::triggered, window,
                   [this]() { handle_menu_saveas(); });
  QObject::connect(mwctx->wloader->get_annotations_project_close_mi(),
                   &QAction::triggered, window,
                   [this]() { handle_menu_project_close(); });

  return true;
}

void ProjectFileEventsHandler::handle_annotations_db_open()
{
  LOGT("Open new annotations dir");

  if (ops->lastChangedStatus) {
    if (!ops->ask_about_unsaved_changes()) {
      LOGD("User interupted data changes discard");
      return;
    }
  }

  std::string newFileName;

  if (!mwctx->cwFactory->ask_open_json_db(mwctx->wloader->get_window(),
                                          newFileName)) {
    LOGD("Dialog is closed");
    return;
  }

  LOGD("Selected json db file: " << newFileName);

  ops->show_spinner();

  // The provider-changed event that follows synchronously hides the spinner.
  mwctx->actx->eventer->onAnnotationsDirChanged(newFileName);

  ops->update_statuses();
}

void ProjectFileEventsHandler::handle_images_dir_open()
{
  LOGT("Open new images dir");

  std::string newDir;

  if (!mwctx->cwFactory->ask_images_folder(mwctx->wloader->get_window(),
                                           newDir)) {
    LOGD("Dialog is closed");
    return;
  }

  LOGD("Selected new images directory: " << newDir);

  ops->show_spinner();

  mwctx->actx->eventer->onImagesDirChanged(newDir);

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

void ProjectFileEventsHandler::handle_menu_save() { ops->save_project(); }

void ProjectFileEventsHandler::handle_menu_saveas() { ops->save_project_as(); }

void ProjectFileEventsHandler::handle_menu_project_close()
{
  LOGT("Trying to close the project");

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

  ops->clear_all_lists();

  ops->update_statuses();

  ops->update_action_buttons_sensitivity();
}

}  // namespace templateQt6::window::event_handlers
