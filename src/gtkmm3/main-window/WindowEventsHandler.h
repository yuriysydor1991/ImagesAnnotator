#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM_WINDOWEVENTSHANDLER_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM_WINDOWEVENTSHANDLER_CLASS_H

#include <memory>

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
class WindowEventsHandler : virtual public ComponentTypesAliases
{
 public:
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

  virtual void update_image_zoom();

  template <class Ntype>
  static int ceilInt(const Ntype& val);

  template <class Ntype>
  static double toD(const Ntype& val);

  template <class Ntype>
  static int toI(const Ntype& val);

 private:
  std::shared_ptr<MainWindowContext> mwctx;
};

}  // namespace templateGtkmm3::window

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM_WINDOWEVENTSHANDLER_CLASS_H
