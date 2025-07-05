#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM_WINDOWLOADER_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM_WINDOWLOADER_CLASS_H

#include <memory>

#include "src/gtkmm3/ComponentTypesAliases.h"
#include "src/gtkmm3/gtkmm_includes.h"
#include "src/gtkmm3/main-window/WindowDataContext.h"
#include "src/gtkmm3/main-window/custom-widgets/CustomWidgetsFactory.h"

namespace templateGtkmm3
{
class MainWindowContext;
}

namespace templateGtkmm3::window
{

/**
 * @brief Class to hold the window loader routines from the UI xml files.
 */
class WindowLoader : virtual public ComponentTypesAliases
{
 public:
  virtual ~WindowLoader() = default;
  WindowLoader() = default;

  virtual bool load_window(std::shared_ptr<MainWindowContext> nmwctx);

  virtual bool all_widget_are_valid();

  virtual Gtk::Window* get_window();
  virtual Gtk::Button* get_images_folder_open_button();
  virtual Gtk::Button* get_annotations_db_open_button();
  virtual Gtk::Button* get_delete_current_image_selected_annotation();
  virtual Gtk::SearchEntry* get_annotation_search_entry();
  virtual Gtk::ListBox* get_annotations_db_list();
  virtual Gtk::ListBox* get_images_list();
  virtual Gtk::Button* get_current_image_zoom_in();
  virtual Gtk::Button* get_current_image_zoom_out();
  virtual Gtk::Viewport* get_place_4_working_widget();
  virtual Gtk::Button* get_copy_annotation_name_button();
  virtual Gtk::Entry* get_edit_current_rect_entry();
  virtual Gtk::Button* get_prev_file_button();
  virtual Gtk::Button* get_next_file_button();
  virtual Gtk::Statusbar* get_window_status_bar();
  virtual Gtk::Button* get_copy_current_annotation_button();

  /// @brief menus

  virtual Gtk::ImageMenuItem* get_images_open_mi();
  virtual Gtk::ImageMenuItem* get_annotations_db_open_mi();
  virtual Gtk::ImageMenuItem* get_annotations_db_save_mi();
  virtual Gtk::ImageMenuItem* get_annotations_db_saveas_mi();
  virtual Gtk::ImageMenuItem* get_annotations_project_close_mi();

  virtual Gtk::ListBox* get_current_image_annotations();

 protected:
  virtual bool propagate_params();
  virtual bool propagate_css_params();

 private:
  template <class WidgetType>
  WidgetType* get_widget(const std::string& id);

  std::shared_ptr<MainWindowContext> mwctx;
  Glib::RefPtr<Gtk::Builder> builder;
};

}  // namespace templateGtkmm3::window

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM_WINDOWLOADER_CLASS_H
