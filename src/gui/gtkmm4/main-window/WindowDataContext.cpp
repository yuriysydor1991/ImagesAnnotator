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

#include "src/gui/gtkmm4/main-window/WindowDataContext.h"

#include <cassert>
#include <memory>

#include "src/gui/gtkmm4/gtkmm_includes.h"
#include "src/log/log.h"

namespace templateGtkmm4::window
{

const std::set<std::string>& WindowDataContext::get_ids()
{
  // Only real Gtk::Builder widgets are validated here. In GTK4 the former menu
  // items became Gio::Actions referenced by the menu model rather than builder
  // widgets, so they are intentionally excluded from this set.
  static const std::set<std::string> ids{
      window_id,
      images_folder_open_button_id,
      annotations_db_open_button_id,
      delete_current_image_selected_annotation_id,
      annotation_search_entry_id,
      annotations_db_list_id,
      images_list_id,
      current_image_zoom_in_id,
      current_image_zoom_out_id,
      place_4_working_widget_id,
      current_image_annotations_id,
      current_image_selected_annotation_edit_id,
      copy_annotation_name_to_current_rect_id,
      previous_image_button_id,
      next_image_button_id,
      main_window_status_bar_id,
      duplicate_current_annotation_id,
      spinner_id,
      main_window_overlay_id,
      central_canvas_scrolled_window_id,
      about_dialog_id,
      images_search_entry_id,
      delete_image_record_id,
      web_page_images_url_asker_id,
      images_web_page_url_entry_id,
      images_web_page_cache_folder_id,
      json_db_open_dialog_id,
      json_db_save_dialog_id,
      images_folder_choose_dialog_id,
      txt_export_folder_dialog_id,
      yolo4_export_folder_dialog_id,
      pytorch_export_folder_dialog_id,
      error_dialog_id,
      save_changes_ask_dialog_id,
      lost_rects_ask_dialog_id};

  return ids;
}

}  // namespace templateGtkmm4::window
