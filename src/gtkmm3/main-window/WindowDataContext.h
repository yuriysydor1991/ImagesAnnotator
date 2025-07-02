#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_WINDOWDATACONTEXT_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_WINDOWDATACONTEXT_CLASS_H

#include <functional>
#include <map>
#include <memory>

#include "src/gtkmm3/gtkmm_includes.h"

namespace templateGtkmm3::window
{

#ifndef RESOURCES_PREFIX
#define RESOURCES_PREFIX "/ua/org/kytok/template/gtkmm3/glade/ImagesAnnotator"
#endif  // RESOURCES_PREFIX

/**
 * @brief Class holds all the pointers and window element IDs for the
 * GtkmmIniter class usage.
 */
class WindowDataContext
{
 public:
  virtual ~WindowDataContext() = default;
  WindowDataContext() = default;

  static const std::set<std::string>& get_ids();

  inline static constexpr const char* const UI_res_path =
      RESOURCES_PREFIX "/GtkmmWindow.ui";
  inline static constexpr const char* const logo_res_path = RESOURCES_PREFIX
      "/resources/images/"
      "kytok.org.ua-logo.png";
  inline static constexpr const char* const main_window_css =
      RESOURCES_PREFIX "/resources/css/main-window.css";

  inline static constexpr const char* const window_id = "main_window";
  inline static constexpr const char* const images_folder_open_button_id =
      "images_folder_open_button";
  inline static constexpr const char* const annotations_db_open_button_id =
      "annotations_db_open_button";
  inline static constexpr const char* const accept_annotation_button_id =
      "accept_annotation_button";
  inline static constexpr const char* const
      delete_current_image_selected_annotation_id =
          "delete_current_image_selected_annotation";
  inline static constexpr const char* const annotation_search_entry_id =
      "annotation_search_entry";
  inline static constexpr const char* const annotations_db_list_id =
      "annotations_db_list";
  inline static constexpr const char* const images_list_id = "images_list";
  inline static constexpr const char* const current_image_zoom_in_id =
      "current_image_zoom_in";
  inline static constexpr const char* const current_image_zoom_out_id =
      "current_image_zoom_out";
  inline static constexpr const char* const place_4_working_widget_id =
      "place_4_working_widget";
  inline static constexpr const char* const images_open_menu_item_id =
      "images_open_menu_item";
  inline static constexpr const char* const annotations_db_open_menu =
      "annotations_db_open_menu";
  inline static constexpr const char* const annotations_db_save_menu_id =
      "annotations_db_save_menu";
  inline static constexpr const char* const annotations_db_save_as_menu_id =
      "annotations_db_save_as_menu";
};

}  // namespace templateGtkmm3::window

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_WINDOWDATACONTEXT_CLASS_H
