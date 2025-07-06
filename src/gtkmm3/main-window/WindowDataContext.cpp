#include "src/gtkmm3/main-window/WindowDataContext.h"

#include <cassert>
#include <memory>

#include "src/gtkmm3/gtkmm_includes.h"
#include "src/log/log.h"

namespace templateGtkmm3::window
{

const std::set<std::string>& WindowDataContext::get_ids()
{
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
      images_open_menu_item_id,
      annotations_db_save_menu_id,
      annotations_db_save_as_menu_id,
      annotations_project_close_id,
      current_image_annotations_id,
      current_image_selected_annotation_edit_id,
      copy_annotation_name_to_current_rect_id,
      previous_image_button_id,
      next_image_button_id,
      main_window_status_bar_id,
      duplicate_current_annotation_id,
      spinner_id,
      main_window_overlay_id,
      central_canvas_scrolled_window_id};

  return ids;
}

}  // namespace templateGtkmm3::window
