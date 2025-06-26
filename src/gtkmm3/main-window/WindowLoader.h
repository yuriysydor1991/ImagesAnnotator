#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM_WINDOWLOADER_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM_WINDOWLOADER_CLASS_H

#include <memory>

#include "src/gtkmm3/gtkmm_includes.h"
#include "src/gtkmm3/main-window/WindowDataContext.h"

namespace templateGtkmm3::window
{

/**
 * @brief Class to hold the window loader routines from the UI xml files.
 */
class WindowLoader
{
 public:
  virtual ~WindowLoader() = default;
  WindowLoader() = default;

  virtual bool load_window(std::shared_ptr<WindowDataContext> nwctx);

  virtual bool all_widget_are_valid();

  virtual Gtk::Window* get_window();
  virtual Gtk::Button* get_images_folder_open_button();
  virtual Gtk::Button* get_annotations_db_open_button();
  virtual Gtk::Button* get_accept_annotation_button();
  virtual Gtk::Button* get_delete_current_image_selected_annotation();
  virtual Gtk::SearchEntry* get_annotation_search_entry();
  virtual Gtk::ListBox* get_annotations_db_list();
  virtual Gtk::ListBox* get_images_list();
  virtual Gtk::Image* get_image();
  virtual Gtk::Button* get_current_image_zoom_in();
  virtual Gtk::Button* get_current_image_zoom_out();

 protected:
  virtual bool propagate_params();

 private:
  template <class WidgetType>
  WidgetType* get_widget(const std::string& id);

  std::shared_ptr<WindowDataContext> wctx;
  Glib::RefPtr<Gtk::Builder> builder;
};

}  // namespace templateGtkmm3::window

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM_WINDOWLOADER_CLASS_H
