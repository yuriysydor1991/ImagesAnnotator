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

#ifndef IMAGES_ANNOTATOR_PROJECT_GTKMM_EVENTHANDLERS_PROJECTFILEEVENTSHANDLER_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_GTKMM_EVENTHANDLERS_PROJECTFILEEVENTSHANDLER_CLASS_H

#include <memory>

#include "src/gtkmm3/gtkmm_includes.h"
#include "src/gtkmm3/main-window/event-handlers/AbstractWindowEventHandler.h"

namespace templateGtkmm3::window::event_handlers
{

/**
 * @brief Handles the project file menu/buttons: opening images dir and
 * annotations db, saving, "save as" and closing the project.
 */
class ProjectFileEventsHandler : public AbstractWindowEventHandler
{
 public:
  explicit ProjectFileEventsHandler(std::shared_ptr<WindowOps> nops);

  bool subscribe() override;

 protected:
  void handle_annotations_db_open();
  void handle_images_dir_open();
  void handle_menu_images_folder_open();
  void handle_menu_annotations_db_open();
  void handle_menu_save();
  void handle_menu_saveas();
  void handle_menu_project_close();
};

}  // namespace templateGtkmm3::window::event_handlers

#endif  // IMAGES_ANNOTATOR_PROJECT_GTKMM_EVENTHANDLERS_PROJECTFILEEVENTSHANDLER_CLASS_H
