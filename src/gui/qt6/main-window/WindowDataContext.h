/**
 * The "Simplified BSD License"
 *
 * Copyright (c) 2026, Yurii Sydor (yuriysydor1991@gmail.com)
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

#ifndef IMAGES_ANNOTATOR_PROJECT_QT6_WINDOWDATACONTEXT_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_QT6_WINDOWDATACONTEXT_CLASS_H

#include <set>
#include <string>

namespace imagesAnnotatorQt6::window
{

#ifndef QT6_RESOURCES_PREFIX
/// @brief Root of the compiled-in Qt resource tree (see
/// src/gui/qt6/ui/template.qrc). It is the Qt analogue of the GResource prefix
/// the GTKmm components use and follows the very same reverse-DNS layout, with
/// the toolkit name as its last element.
#define QT6_RESOURCES_PREFIX ":/ua/org/kytok/template/qt6"
#endif  // QT6_RESOURCES_PREFIX

/**
 * @brief Holds the stable object names of every main window element plus the
 * embedded resource paths - the Qt analogue of imagesAnnotatorGtkmm3's
 * WindowDataContext.
 *
 * The GTKmm window is inflated from a Glade .ui file and every widget is looked
 * up in the Gtk::Builder by a string id. The Qt window is built in code, but
 * the same ids are kept and assigned as QObject::objectName() values, so they
 * still identify the widgets: WindowLoader::all_widget_are_valid() resolves
 * every id below through QObject::findChild() exactly as the GTKmm loader
 * resolves them through the builder, and Qt Style Sheets can address the same
 * names. Keeping the id strings identical to the gtkmm3 ones also keeps the two
 * components directly comparable.
 *
 * The ids of the GTKmm-only elements are absent: the spinner and its overlay
 * (Qt shows a wait cursor instead), the About dialog and the web page URL asker
 * with its entry and cache-folder label (built on demand as modal Qt dialogs)
 * and the working area viewport (a single QScrollArea covers what GTK splits
 * between a Gtk::ScrolledWindow and a Gtk::Viewport).
 */
class WindowDataContext
{
 public:
  virtual ~WindowDataContext() = default;
  WindowDataContext() = default;

  /// @brief The full set of object names the window installs.
  static const std::set<std::string>& get_ids();

  inline static constexpr const char* const logo_res_path = QT6_RESOURCES_PREFIX
      "/resources/images/"
      "kytok.org.ua-logo.png";
  inline static constexpr const char* const main_window_qss =
      QT6_RESOURCES_PREFIX "/resources/css/main-window.qss";

  inline static constexpr const char* const window_id = "main_window";
  inline static constexpr const char* const images_folder_open_button_id =
      "images_folder_open_button";
  inline static constexpr const char* const annotations_db_open_button_id =
      "annotations_db_open_button";
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
  inline static constexpr const char* const images_open_menu_item_id =
      "images_open_menu_item";
  inline static constexpr const char* const annotations_db_open_menu =
      "annotations_db_open_menu";
  inline static constexpr const char* const annotations_db_save_menu_id =
      "annotations_db_save_menu";
  inline static constexpr const char* const annotations_db_save_as_menu_id =
      "annotations_db_save_as_menu";
  inline static constexpr const char* const annotations_project_close_id =
      "annotations_project_close";

  inline static constexpr const char* const current_image_annotations_id =
      "current_image_annotations";
  inline static constexpr const char* const
      current_image_selected_annotation_edit_id =
          "current_image_selected_annotation_edit";
  inline static constexpr const char* const
      copy_annotation_name_to_current_rect_id =
          "copy_annotation_name_to_current_rect";
  inline static constexpr const char* const previous_image_button_id =
      "previous_image_button";
  inline static constexpr const char* const next_image_button_id =
      "next_image_button";
  inline static constexpr const char* const main_window_status_bar_id =
      "main_window_status_bar";
  inline static constexpr const char* const duplicate_current_annotation_id =
      "duplicate_current_annotation";

  inline static constexpr const char* const central_canvas_scrolled_window_id =
      "central_canvas_scrolled_window";
  inline static constexpr const char* const central_working_canvas_id =
      "central_working_canvas";
  inline static constexpr const char* const about_menu_item_id =
      "about_menu_item";

  inline static constexpr const char* const annotations_export_txt_2_folder_id =
      "annotations_export_txt_2_folder";

  inline static constexpr const char* const
      annotations_export_yolo4_2_folder2_id =
          "annotations_export_yolo4_2_folder2";

  inline static constexpr const char* const
      annotations_export_pytorchvision_2_folder_id =
          "annotations_export_pytorchvision_2_folder";
  inline static constexpr const char* const
      annotations_export_ultralytics_detect_2_folder_id =
          "annotations_export_ultralytics_detect_2_folder";
  inline static constexpr const char* const
      annotations_export_ultralytics_obb_2_folder_id =
          "annotations_export_ultralytics_obb_2_folder";
  inline static constexpr const char* const
      annotations_export_ultralytics_segment_2_folder_id =
          "annotations_export_ultralytics_segment_2_folder";
  inline static constexpr const char* const
      annotations_export_coco_2_folder_id = "annotations_export_coco_2_folder";
  inline static constexpr const char* const
      annotations_export_pascal_voc_2_folder_id =
          "annotations_export_pascal_voc_2_folder";
  inline static constexpr const char* const
      annotations_export_createml_2_folder_id =
          "annotations_export_createml_2_folder";

  inline static constexpr const char* const images_search_entry_id =
      "images_search_entry";

  inline static constexpr const char* const delete_image_record_id =
      "delete_image_record";

  inline static constexpr const char* const images_web_page_open_menu_item_id =
      "images_web_page_open_menu_item";

  inline static constexpr const char* const
      auto_insert_current_annotation_to_whole_id =
          "auto_insert_current_annotation_to_whole";
};

}  // namespace imagesAnnotatorQt6::window

#endif  // IMAGES_ANNOTATOR_PROJECT_QT6_WINDOWDATACONTEXT_CLASS_H
