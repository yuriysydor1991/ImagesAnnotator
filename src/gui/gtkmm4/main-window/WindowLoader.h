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

#ifndef IMAGES_ANNOTATOR_PROJECT_GTKMM4_WINDOWLOADER_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_GTKMM4_WINDOWLOADER_CLASS_H

#include <memory>

#include "src/gui/gtkmm4/ComponentTypesAliases.h"
#include "src/gui/gtkmm4/gtkmm_includes.h"
#include "src/gui/gtkmm4/main-window/WindowDataContext.h"
#include "src/gui/gtkmm4/main-window/custom-widgets/CustomWidgetsFactory.h"

namespace imagesAnnotatorGtkmm4
{
class MainWindowContext;
}

namespace imagesAnnotatorGtkmm4::window
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
  virtual Gtk::SearchEntry* get_images_search_entry();
  virtual Gtk::Button* get_delete_image_record();
  virtual Gtk::Dialog* get_web_page_url_asker();
  virtual Gtk::Entry* get_images_web_page_url_entry();
  virtual Gtk::Label* get_images_web_page_cache_folder();

  /// @brief Dialogs whose shells are defined in GtkmmWindow.ui. Each is a
  /// single reused instance; the event handlers just obtain the ready dialog
  /// and run it. The one-time filter / action-button decoration is applied at
  /// load by prepare_dialogs().
  virtual Gtk::FileChooserDialog* get_json_db_open_dialog();
  virtual Gtk::FileChooserDialog* get_json_db_save_dialog();
  virtual Gtk::FileChooserDialog* get_images_folder_choose_dialog();
  virtual Gtk::FileChooserDialog* get_txt_export_folder_dialog();
  virtual Gtk::FileChooserDialog* get_yolo4_export_folder_dialog();
  virtual Gtk::FileChooserDialog* get_pytorch_export_folder_dialog();
  virtual Gtk::FileChooserDialog* get_ultralytics_detect_export_folder_dialog();
  virtual Gtk::FileChooserDialog* get_ultralytics_obb_export_folder_dialog();
  virtual Gtk::FileChooserDialog*
  get_ultralytics_segment_export_folder_dialog();
  virtual Gtk::FileChooserDialog* get_coco_export_folder_dialog();
  virtual Gtk::FileChooserDialog* get_pascal_voc_export_folder_dialog();
  virtual Gtk::FileChooserDialog* get_createml_export_folder_dialog();
  virtual Gtk::FileChooserDialog* get_dxxwjz2_export_folder_dialog();
  virtual Gtk::FileChooserDialog* get_dxxwjz2p_export_folder_dialog();
  virtual Gtk::FileChooserDialog* get_dxxwjz3_export_folder_dialog();
  virtual Gtk::FileChooserDialog* get_dxxwjz3p_export_folder_dialog();
  virtual Gtk::FileChooserDialog* get_txt_import_folder_dialog();
  virtual Gtk::FileChooserDialog* get_yolo4_import_folder_dialog();
  virtual Gtk::FileChooserDialog* get_ultralytics_detect_import_folder_dialog();
  virtual Gtk::FileChooserDialog* get_ultralytics_obb_import_folder_dialog();
  virtual Gtk::FileChooserDialog*
  get_ultralytics_segment_import_folder_dialog();
  virtual Gtk::FileChooserDialog* get_coco_import_folder_dialog();
  virtual Gtk::FileChooserDialog* get_pascal_voc_import_folder_dialog();
  virtual Gtk::FileChooserDialog* get_createml_import_folder_dialog();
  virtual Gtk::FileChooserDialog* get_dxxwjz2_import_folder_dialog();
  virtual Gtk::FileChooserDialog* get_dxxwjz2p_import_file_dialog();
  virtual Gtk::FileChooserDialog* get_dxxwjz3_import_folder_dialog();
  virtual Gtk::FileChooserDialog* get_dxxwjz3p_import_file_dialog();
  virtual Gtk::FileChooserDialog* get_pytorch_import_folder_dialog();
  virtual Gtk::MessageDialog* get_error_dialog();
  virtual Gtk::MessageDialog* get_save_changes_ask_dialog();
  virtual Gtk::MessageDialog* get_lost_rects_ask_dialog();

  /// @brief menu entries. In GTK4 the former GtkImageMenuItem widgets are
  /// Gio::SimpleActions referenced by the menu model. The event handlers
  /// connect to each action's signal_activate() instead of a menu-item's
  /// signal_activate().

  virtual Glib::RefPtr<Gio::SimpleAction> get_images_open_action();
  virtual Glib::RefPtr<Gio::SimpleAction> get_annotations_db_open_action();
  virtual Glib::RefPtr<Gio::SimpleAction> get_annotations_db_save_action();
  virtual Glib::RefPtr<Gio::SimpleAction> get_annotations_db_saveas_action();
  virtual Glib::RefPtr<Gio::SimpleAction>
  get_annotations_project_close_action();
  virtual Glib::RefPtr<Gio::SimpleAction> get_about_action();
  virtual Glib::RefPtr<Gio::SimpleAction> get_export_txt_action();
  virtual Glib::RefPtr<Gio::SimpleAction> get_export_yolo4_action();
  virtual Glib::RefPtr<Gio::SimpleAction> get_export_pytorchvision_action();
  virtual Glib::RefPtr<Gio::SimpleAction>
  get_export_ultralytics_detect_action();
  virtual Glib::RefPtr<Gio::SimpleAction> get_export_ultralytics_obb_action();
  virtual Glib::RefPtr<Gio::SimpleAction>
  get_export_ultralytics_segment_action();
  virtual Glib::RefPtr<Gio::SimpleAction> get_export_coco_action();
  virtual Glib::RefPtr<Gio::SimpleAction> get_export_pascal_voc_action();
  virtual Glib::RefPtr<Gio::SimpleAction> get_export_createml_action();
  virtual Glib::RefPtr<Gio::SimpleAction> get_export_dxxwjz2_action();
  virtual Glib::RefPtr<Gio::SimpleAction> get_export_dxxwjz2p_action();
  virtual Glib::RefPtr<Gio::SimpleAction> get_export_dxxwjz3_action();
  virtual Glib::RefPtr<Gio::SimpleAction> get_export_dxxwjz3p_action();
  virtual Glib::RefPtr<Gio::SimpleAction> get_import_txt_action();
  virtual Glib::RefPtr<Gio::SimpleAction> get_import_yolo4_action();
  virtual Glib::RefPtr<Gio::SimpleAction>
  get_import_ultralytics_detect_action();
  virtual Glib::RefPtr<Gio::SimpleAction> get_import_ultralytics_obb_action();
  virtual Glib::RefPtr<Gio::SimpleAction>
  get_import_ultralytics_segment_action();
  virtual Glib::RefPtr<Gio::SimpleAction> get_import_coco_action();
  virtual Glib::RefPtr<Gio::SimpleAction> get_import_pascal_voc_action();
  virtual Glib::RefPtr<Gio::SimpleAction> get_import_createml_action();
  virtual Glib::RefPtr<Gio::SimpleAction> get_import_dxxwjz2_action();
  virtual Glib::RefPtr<Gio::SimpleAction> get_import_dxxwjz2p_action();
  virtual Glib::RefPtr<Gio::SimpleAction> get_import_dxxwjz3_action();
  virtual Glib::RefPtr<Gio::SimpleAction> get_import_dxxwjz3p_action();
  virtual Glib::RefPtr<Gio::SimpleAction> get_import_pytorchvision_action();
  virtual Glib::RefPtr<Gio::SimpleAction> get_images_web_page_open_action();
  virtual Glib::RefPtr<Gio::SimpleAction> get_auto_insert_annotation_action();

  virtual Gtk::ListBox* get_current_image_annotations();

  virtual void update_window_title(const std::string& openedFile = "");

 protected:
  virtual bool propagate_params();
  virtual bool propagate_css_params();
  virtual void set_window_icon();
  virtual void decorate_button_icons();
  virtual bool create_actions();
  virtual bool attach_spinner();

  /// @brief Reparents every builder-defined dialog onto the main window and
  /// applies the one-time decoration GtkBuilder cannot express from the .ui:
  /// the JSON and darknetxx project file filters and the localized,
  /// icon-decorated action buttons.
  virtual bool prepare_dialogs();

  /// @brief Sizes the two column dividers so the central image working area is
  /// the dominant widget while the left (images) and right (annotations)
  /// management panels keep a comfortable fixed width. The paned widths are
  /// only known after the maximized window is mapped, so the split is applied
  /// once from an idle callback scheduled on the first map.
  virtual bool init_paned_layout();

 private:
  template <class WidgetType>
  WidgetType* get_widget(const std::string& id);

  inline static constexpr const int spinner_size = 100;

  /// @brief Initial comfortable width (in pixels) of each side management
  /// panel; the central image working area takes the remaining space.
  inline static constexpr const int side_panel_width = 300;

  std::shared_ptr<MainWindowContext> mwctx;
  Glib::RefPtr<Gtk::Builder> builder;

  bool paned_positions_done{false};

  Glib::RefPtr<Gio::SimpleActionGroup> actionGroup;
  Glib::RefPtr<Gio::SimpleAction> action_images_open;
  Glib::RefPtr<Gio::SimpleAction> action_annotations_db_open;
  Glib::RefPtr<Gio::SimpleAction> action_annotations_db_save;
  Glib::RefPtr<Gio::SimpleAction> action_annotations_db_saveas;
  Glib::RefPtr<Gio::SimpleAction> action_annotations_project_close;
  Glib::RefPtr<Gio::SimpleAction> action_about;
  Glib::RefPtr<Gio::SimpleAction> action_export_txt;
  Glib::RefPtr<Gio::SimpleAction> action_export_yolo4;
  Glib::RefPtr<Gio::SimpleAction> action_export_pytorchvision;
  Glib::RefPtr<Gio::SimpleAction> action_export_ultralytics_detect;
  Glib::RefPtr<Gio::SimpleAction> action_export_ultralytics_obb;
  Glib::RefPtr<Gio::SimpleAction> action_export_ultralytics_segment;
  Glib::RefPtr<Gio::SimpleAction> action_export_coco;
  Glib::RefPtr<Gio::SimpleAction> action_export_pascal_voc;
  Glib::RefPtr<Gio::SimpleAction> action_export_createml;
  Glib::RefPtr<Gio::SimpleAction> action_export_dxxwjz2;
  Glib::RefPtr<Gio::SimpleAction> action_export_dxxwjz2p;
  Glib::RefPtr<Gio::SimpleAction> action_export_dxxwjz3;
  Glib::RefPtr<Gio::SimpleAction> action_export_dxxwjz3p;
  Glib::RefPtr<Gio::SimpleAction> action_import_txt;
  Glib::RefPtr<Gio::SimpleAction> action_import_yolo4;
  Glib::RefPtr<Gio::SimpleAction> action_import_ultralytics_detect;
  Glib::RefPtr<Gio::SimpleAction> action_import_ultralytics_obb;
  Glib::RefPtr<Gio::SimpleAction> action_import_ultralytics_segment;
  Glib::RefPtr<Gio::SimpleAction> action_import_coco;
  Glib::RefPtr<Gio::SimpleAction> action_import_pascal_voc;
  Glib::RefPtr<Gio::SimpleAction> action_import_createml;
  Glib::RefPtr<Gio::SimpleAction> action_import_dxxwjz2;
  Glib::RefPtr<Gio::SimpleAction> action_import_dxxwjz2p;
  Glib::RefPtr<Gio::SimpleAction> action_import_dxxwjz3;
  Glib::RefPtr<Gio::SimpleAction> action_import_dxxwjz3p;
  Glib::RefPtr<Gio::SimpleAction> action_import_pytorchvision;
  Glib::RefPtr<Gio::SimpleAction> action_images_web_page_open;
  Glib::RefPtr<Gio::SimpleAction> action_auto_insert_annotation;
};

}  // namespace imagesAnnotatorGtkmm4::window

#endif  // IMAGES_ANNOTATOR_PROJECT_GTKMM4_WINDOWLOADER_CLASS_H
