#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_WINDOWDATACONTEXT_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_WINDOWDATACONTEXT_CLASS_H

#include <functional>
#include <map>
#include <memory>

#include "src/gtkmm3/gtkmm_includes.h"

namespace templateGtkmm3::window
{

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
      "/ua/org/kytok/template/gtkmm3/glade/GtkmmWindow.ui";
  inline static constexpr const char* const logo_res_path =
      "/ua/org/kytok/template/gtkmm3/glade/resources/images/"
      "kytok.org.ua-logo.png";

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
  inline static constexpr const char* const central_working_image_id =
      "central_working_image";
};

}  // namespace templateGtkmm3::window

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_WINDOWDATACONTEXT_CLASS_H
