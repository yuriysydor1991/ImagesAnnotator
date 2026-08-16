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

#ifndef IMAGES_ANNOTATOR_TEMPLATE_PROJECT_WINDOWDATACONTEXT_CLASS_H
#define IMAGES_ANNOTATOR_TEMPLATE_PROJECT_WINDOWDATACONTEXT_CLASS_H

#include <functional>
#include <map>
#include <memory>

#include "src/gui/gtkmm4/gtkmm_includes.h"

namespace imagesAnnotatorGtkmm4::window
{

#ifndef RESOURCES_PREFIX
#define RESOURCES_PREFIX "/ua/org/kytok/template/gtkmm4/glade/ImagesAnnotator"
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

  inline static constexpr const char* const spinner_id = "spinner";
  inline static constexpr const char* const main_window_overlay_id =
      "main_window_overlay";
  inline static constexpr const char* const central_canvas_scrolled_window_id =
      "central_canvas_scrolled_window";
  inline static constexpr const char* const about_dialog_id = "about_dialog";
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

  inline static constexpr const char* const images_search_entry_id =
      "images_search_entry";

  inline static constexpr const char* const delete_image_record_id =
      "delete_image_record";

  inline static constexpr const char* const web_page_images_url_asker_id =
      "web_page_images_url_asker";

  inline static constexpr const char* const images_web_page_url_entry_id =
      "images_web_page_url_entry";

  inline static constexpr const char* const images_web_page_open_menu_item_id =
      "images_web_page_open_menu_item";

  inline static constexpr const char* const images_web_page_cache_folder_id =
      "images_web_page_cache_folder";

  // Dialog shells defined in GtkmmWindow.ui (formerly built in C++ by
  // CustomWidgetsFactory). WindowLoader obtains each by id and hands the
  // file-chooser filters / localized action buttons to CustomWidgetsFactory to
  // apply once at load.
  inline static constexpr const char* const json_db_open_dialog_id =
      "json_db_open_dialog";
  inline static constexpr const char* const json_db_save_dialog_id =
      "json_db_save_dialog";
  inline static constexpr const char* const images_folder_choose_dialog_id =
      "images_folder_choose_dialog";
  inline static constexpr const char* const txt_export_folder_dialog_id =
      "txt_export_folder_dialog";
  inline static constexpr const char* const yolo4_export_folder_dialog_id =
      "yolo4_export_folder_dialog";
  inline static constexpr const char* const pytorch_export_folder_dialog_id =
      "pytorch_export_folder_dialog";
  inline static constexpr const char* const
      ultralytics_detect_export_folder_dialog_id =
          "ultralytics_detect_export_folder_dialog";
  inline static constexpr const char* const
      ultralytics_obb_export_folder_dialog_id =
          "ultralytics_obb_export_folder_dialog";
  inline static constexpr const char* const
      ultralytics_segment_export_folder_dialog_id =
          "ultralytics_segment_export_folder_dialog";
  inline static constexpr const char* const coco_export_folder_dialog_id =
      "coco_export_folder_dialog";
  inline static constexpr const char* const pascal_voc_export_folder_dialog_id =
      "pascal_voc_export_folder_dialog";
  inline static constexpr const char* const error_dialog_id = "error_dialog";
  inline static constexpr const char* const save_changes_ask_dialog_id =
      "save_changes_ask_dialog";
  inline static constexpr const char* const lost_rects_ask_dialog_id =
      "lost_rects_ask_dialog";

  inline static constexpr const char* const
      auto_insert_current_annotation_to_whole_id =
          "auto_insert_current_annotation_to_whole";

  // The two GtkPaned dividers that split the window into the left (images),
  // middle (image review) and right (annotations) columns. WindowLoader gives
  // them an equal-thirds initial split once the window is laid out.
  inline static constexpr const char* const main_columns_paned_id =
      "main_columns_paned";
  inline static constexpr const char* const left_columns_paned_id =
      "left_columns_paned";

  // GTK4 menu actions. Unlike the GTK3 GtkImageMenuItem widgets, the menu
  // entries are Gio::SimpleActions installed on the window under the "win"
  // action group. The menu model (see GtkmmWindow.ui) references them as
  // "win.<name>", and WindowLoader exposes each action through a getter.
  inline static constexpr const char* const action_group_prefix = "win";

  inline static constexpr const char* const action_images_open = "images-open";
  inline static constexpr const char* const action_annotations_db_open =
      "annotations-db-open";
  inline static constexpr const char* const action_annotations_db_save =
      "annotations-db-save";
  inline static constexpr const char* const action_annotations_db_saveas =
      "annotations-db-saveas";
  inline static constexpr const char* const action_annotations_project_close =
      "annotations-project-close";
  inline static constexpr const char* const action_export_txt = "export-txt";
  inline static constexpr const char* const action_export_yolo4 =
      "export-yolo4";
  inline static constexpr const char* const action_export_pytorchvision =
      "export-pytorchvision";
  inline static constexpr const char* const action_export_ultralytics_detect =
      "export-ultralytics-detect";
  inline static constexpr const char* const action_export_ultralytics_obb =
      "export-ultralytics-obb";
  inline static constexpr const char* const action_export_ultralytics_segment =
      "export-ultralytics-segment";
  inline static constexpr const char* const action_export_coco = "export-coco";
  inline static constexpr const char* const action_export_pascal_voc =
      "export-pascal-voc";
  inline static constexpr const char* const action_images_web_page_open =
      "images-web-page-open";
  inline static constexpr const char* const action_auto_insert_annotation =
      "auto-insert-annotation";
  inline static constexpr const char* const action_about = "about";
};

}  // namespace imagesAnnotatorGtkmm4::window

#endif  // IMAGES_ANNOTATOR_TEMPLATE_PROJECT_WINDOWDATACONTEXT_CLASS_H
