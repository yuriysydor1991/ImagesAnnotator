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
      accept_annotation_button_id,
      delete_current_image_selected_annotation_id,
      annotation_search_entry_id,
      annotations_db_list_id,
      images_list_id,
      current_image_zoom_in_id,
      current_image_zoom_out_id,
      place_4_working_widget_id};

  return ids;
}

}  // namespace templateGtkmm3::window
