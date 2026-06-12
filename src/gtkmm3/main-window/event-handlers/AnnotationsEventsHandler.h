/**
 * The "Simplified BSD License"
 *
 * Copyright (c) 2025, Yurii Sydor (yuriysydor1991@gmail.com)
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

#ifndef IMAGES_ANNOTATOR_PROJECT_GTKMM_EVENTHANDLERS_ANNOTATIONSEVENTSHANDLER_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_GTKMM_EVENTHANDLERS_ANNOTATIONSEVENTSHANDLER_CLASS_H

#include <memory>

#include "src/gtkmm3/ComponentTypesAliases.h"
#include "src/gtkmm3/gtkmm_includes.h"
#include "src/gtkmm3/main-window/event-handlers/AbstractWindowEventHandler.h"

namespace templateGtkmm3::window::event_handlers
{

/**
 * @brief Handles the annotations related events: current image rectangles list,
 * the all annotations list, rectangle renaming, copying and duplication.
 */
class AnnotationsEventsHandler : public AbstractWindowEventHandler,
                                virtual public ComponentTypesAliases
{
 public:
  explicit AnnotationsEventsHandler(std::shared_ptr<WindowOps> nops);

  bool subscribe() override;

 protected:
  void handle_ci_rect_row_selected(Gtk::ListBoxRow* row);
  void handle_current_rectangle_delete();
  void handle_rect_edit_entry_changed();
  void handle_all_annotations_selected(Gtk::ListBoxRow* row);
  void handle_annotations_name_copy();
  void handle_ci_annotation_copy();
  void handle_annotations_search_text_changed();
  void handle_auto_insert_to_whole();
};

}  // namespace templateGtkmm3::window::event_handlers

#endif  // IMAGES_ANNOTATOR_PROJECT_GTKMM_EVENTHANDLERS_ANNOTATIONSEVENTSHANDLER_CLASS_H
