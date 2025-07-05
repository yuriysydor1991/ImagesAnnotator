#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM_WINDOWEVENTSHANDLER_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM_WINDOWEVENTSHANDLER_CLASS_H

#include <memory>

#include "src/annotator-events/events/ImagesDirProviderChangedHandler.h"
#include "src/gtkmm3/ComponentTypesAliases.h"
#include "src/gtkmm3/MainWindowContext.h"
#include "src/gtkmm3/gtkmm_includes.h"
#include "src/gtkmm3/main-window/WindowDataContext.h"

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
      public std::enable_shared_from_this<WindowEventsHandler>
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
  virtual bool on_rectangle_draw_start(GdkEventButton* event);
  virtual bool on_rectangle_draw_end(GdkEventButton* event);
  virtual bool on_rectangle_size_change(GdkEventMotion* event);
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

  virtual void update_image_zoom();

  virtual void subscribe_4_visual_events();
  virtual void subscribe_4_system_events();

  virtual void handle(std::shared_ptr<ImagesDirProviderChanged> event) override;

  void clean_list_box(Gtk::ListBox* listBox);

  void update_images_list();
  void update_current_rects_list();
  void update_rect_edit_entry();
  void update_annotations_list();

  template <class Ntype>
  static int ceilInt(const Ntype& val);

  template <class Ntype>
  static double toD(const Ntype& val);

  template <class Ntype>
  static int toI(const Ntype& val);

 private:
  std::shared_ptr<MainWindowContext> mwctx;

  bool dragging{false};
};

}  // namespace templateGtkmm3::window

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM_WINDOWEVENTSHANDLER_CLASS_H
