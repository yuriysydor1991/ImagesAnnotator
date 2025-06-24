#include "src/gtkmm3/window/WindowDataContext.h"

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
      central_working_image_id};
  return ids;
}

}  // namespace templateGtkmm3::window
