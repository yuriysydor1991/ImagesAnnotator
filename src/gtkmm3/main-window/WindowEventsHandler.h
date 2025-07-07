#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM_WINDOWEVENTSHANDLER_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM_WINDOWEVENTSHANDLER_CLASS_H

#include <memory>

#include "src/annotator-events/events/ImagesDirProviderChangedHandler.h"
#include "src/gtkmm3/ComponentTypesAliases.h"
#include "src/gtkmm3/MainWindowContext.h"
#include "src/gtkmm3/gtkmm_includes.h"
#include "src/gtkmm3/main-window/WindowDataContext.h"
#include "src/helpers/TypeHelper.h"

namespace templateGtkmm3
{
class MainWindowContext;
}  // namespace templateGtkmm3

namespace templateGtkmm3::window
{

/**
 * @brief Class to hold the window loader routines from the UI xml files.
 */
class WindowEventsHandler
    : virtual public ComponentTypesAliases,
      virtual public events::events::ImagesDirProviderChangedHandler,
      public std::enable_shared_from_this<WindowEventsHandler>,
      virtual public helpers::TypeHelper
{
 public:
  using ImagesDirProviderChanged = events::events::ImagesDirProviderChanged;

  virtual ~WindowEventsHandler() = default;
  WindowEventsHandler() = default;

  virtual bool init(std::shared_ptr<MainWindowContext> nmwctx);

 protected:
  virtual void on_images_row_selected(Gtk::ListBoxRow* row);
  virtual void on_zoom_in_clicked();
  virtual void on_zoom_out_clicked();
  virtual bool on_image_scroll(GdkEventScroll* scroll_event);
  virtual void on_annotations_db_open_click();
  virtual void on_images_dir_open_click();
  virtual void on_menu_images_folder_open_activate();
  virtual void on_menu_annotations_db_open_activate();
  virtual void on_menu_annotations_db_save_activate();
  virtual void on_menu_annotations_db_saveas_activate();
  virtual void on_menu_annotations_project_close_activate();
  virtual void on_current_image_rect_row_selected(Gtk::ListBoxRow* row);
  virtual void on_current_rectangle_delete_click();
  virtual void on_rect_edit_entry_changed();
  virtual void on_all_annotations_selected(Gtk::ListBoxRow* row);
  virtual void on_annotations_name_copy_click();
  virtual void on_next_file_button_click();
  virtual void on_prev_file_button_click();
  virtual void on_ci_annotation_copy_click();
  virtual void on_annotations_search_text_changed();
  virtual void on_menu_about_activate();
  virtual void on_export_txt_2_folder_activate();

  virtual bool on_mouse_motion_start(GdkEventButton* event);
  virtual bool on_mouse_motion_end(GdkEventButton* event);
  virtual bool on_mouse_motion_event(GdkEventMotion* event);
  virtual bool on_rectangle_draw_start(GdkEventButton* event);
  virtual bool on_rectangle_draw_end(GdkEventButton* event);
  virtual bool on_rectangle_size_change(GdkEventMotion* event);
  virtual bool on_mouse_resize_motion_start(GdkEventButton* event);
  virtual bool on_mouse_resize_motion_event(GdkEventMotion* event);
  virtual bool update_current_resize(GdkEventMotion* event);

  virtual void update_image_zoom();

  virtual void subscribe_4_visual_events();
  virtual void subscribe_4_system_events();

  virtual void handle(std::shared_ptr<ImagesDirProviderChanged> event) override;

  void clean_list_box(Gtk::ListBox* listBox);
  void select_list_box_child(Gtk::ListBox* listBox, Gtk::Widget* child);

  void update_images_list();
  void update_current_rects_list();
  void update_rect_edit_entry();
  void update_annotations_list();
  void update_current_annotations_selection();
  void update_status_bar();

  void normalize_current_rect();
  void normilize_initial_image_load_scale();

  void set_resize_cursor();
  void reset_cursor();

  void show_spinner();
  void hide_spinner();

  void show_error_dialog(const std::string& emsg);

  virtual bool load_image(const std::string& filepath);

  virtual bool has_to_export();

  inline static constexpr const char* const overlay_class =
      "almost_non_transparent";
  inline static constexpr const double load_image_scale_helper = 1.015;

 private:
  std::shared_ptr<MainWindowContext> mwctx;

  bool dragging{false};
  bool isOverResize{false};
};

}  // namespace templateGtkmm3::window

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM_WINDOWEVENTSHANDLER_CLASS_H
