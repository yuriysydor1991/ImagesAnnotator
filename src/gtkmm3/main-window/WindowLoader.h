/**
 * The "Simplified BSD License"
 *
 * Copyright (c) 2025, Yurii Sydor (yuriysydor1991@gmail.com)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   - Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef IMAGES_ANNOTATOR_PROJECT_GTKMM_WINDOWLOADER_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_GTKMM_WINDOWLOADER_CLASS_H

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
  virtual Gtk::Overlay* get_main_overlay();
  virtual Gtk::Spinner* get_spinner();
  virtual Gtk::ScrolledWindow* get_central_scrolled_window();
  virtual Gtk::AboutDialog* get_about();

  /// @brief menus

  virtual Gtk::ImageMenuItem* get_images_open_mi();
  virtual Gtk::ImageMenuItem* get_annotations_db_open_mi();
  virtual Gtk::ImageMenuItem* get_annotations_db_save_mi();
  virtual Gtk::ImageMenuItem* get_annotations_db_saveas_mi();
  virtual Gtk::ImageMenuItem* get_annotations_project_close_mi();
  virtual Gtk::ImageMenuItem* get_about_mi();
  virtual Gtk::ImageMenuItem* get_export_txt2_folder_mi();

  virtual Gtk::ListBox* get_current_image_annotations();

  virtual void update_window_title(const std::string& openedFile = "");

 protected:
  virtual bool propagate_params();
  virtual bool propagate_css_params();

 private:
  template <class WidgetType>
  WidgetType* get_widget(const std::string& id);

  inline static constexpr const int spinner_size = 100;

  std::shared_ptr<MainWindowContext> mwctx;
  Glib::RefPtr<Gtk::Builder> builder;
};

}  // namespace templateGtkmm3::window

#endif  // IMAGES_ANNOTATOR_PROJECT_GTKMM_WINDOWLOADER_CLASS_H
