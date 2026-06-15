#ifndef IMAGES_ANNOTATOR_PROJECT_GTKMM_TESTS_EVENTHANDLERS_FAKE_WINDOW_LOADER_H
#define IMAGES_ANNOTATOR_PROJECT_GTKMM_TESTS_EVENTHANDLERS_FAKE_WINDOW_LOADER_H

#include <gtkmm.h>

#include "src/gtkmm3/main-window/WindowLoader.h"

/**
 * @brief Test double for the WindowLoader used by the event handler component
 * tests.
 *
 * The real WindowLoader populates its widgets from the embedded GResource UI
 * file (Gtk::Builder::create_from_resource). The event handlers only reach the
 * widgets through the WindowLoader's virtual getters, so this double overrides
 * those getters to return real, owned GTK widgets constructed directly. That
 * lets the handlers' subscribe() wiring be exercised without loading the UI
 * resource, while still connecting against genuine GTK signals.
 *
 * Must only be instantiated once GTK has been initialized (see GtkInitFixture).
 */
class FakeWindowLoader : public templateGtkmm3::window::WindowLoader
{
 public:
  Gtk::Window* get_window() override { return &window_; }

  Gtk::Button* get_current_image_zoom_in() override { return &zoom_in_; }
  Gtk::Button* get_current_image_zoom_out() override { return &zoom_out_; }

  Gtk::Button* get_prev_file_button() override { return &prev_; }
  Gtk::Button* get_next_file_button() override { return &next_; }

  Gtk::ListBox* get_images_list() override { return &images_list_; }
  Gtk::SearchEntry* get_images_search_entry() override
  {
    return &images_search_;
  }
  Gtk::Button* get_delete_image_record() override { return &del_image_; }

  Gtk::ListBox* get_current_image_annotations() override
  {
    return &current_image_annotations_;
  }
  Gtk::Button* get_delete_current_image_selected_annotation() override
  {
    return &del_ann_;
  }
  Gtk::Entry* get_edit_current_rect_entry() override { return &edit_rect_; }
  Gtk::ListBox* get_annotations_db_list() override
  {
    return &annotations_db_list_;
  }
  Gtk::Button* get_copy_annotation_name_button() override
  {
    return &copy_name_;
  }
  Gtk::Button* get_copy_current_annotation_button() override
  {
    return &copy_curr_;
  }
  Gtk::SearchEntry* get_annotation_search_entry() override
  {
    return &annotation_search_;
  }
  Gtk::ImageMenuItem* auto_insert_current_annotation_to_whole_mi() override
  {
    return &auto_insert_whole_;
  }

  Gtk::Button* get_annotations_db_open_button() override
  {
    return &ann_db_open_;
  }
  Gtk::Button* get_images_folder_open_button() override
  {
    return &images_folder_open_;
  }
  Gtk::ImageMenuItem* get_images_open_mi() override { return &images_open_mi_; }
  Gtk::ImageMenuItem* get_annotations_db_open_mi() override
  {
    return &ann_db_open_mi_;
  }
  Gtk::ImageMenuItem* get_annotations_db_save_mi() override
  {
    return &ann_save_mi_;
  }
  Gtk::ImageMenuItem* get_annotations_db_saveas_mi() override
  {
    return &ann_saveas_mi_;
  }
  Gtk::ImageMenuItem* get_annotations_project_close_mi() override
  {
    return &ann_close_mi_;
  }

  Gtk::ImageMenuItem* get_export_txt2_folder_mi() override
  {
    return &export_txt_mi_;
  }
  Gtk::ImageMenuItem* get_export_yolo4_folder_mi() override
  {
    return &export_yolo4_mi_;
  }
  Gtk::ImageMenuItem* get_export_pytorchvision_folder_mi() override
  {
    return &export_pytorch_mi_;
  }

  Gtk::ImageMenuItem* get_images_web_page_open_menu_item_mi() override
  {
    return &webpage_open_mi_;
  }

  Gtk::ImageMenuItem* get_about_mi() override { return &about_mi_; }

 private:
  Gtk::Window window_;

  Gtk::Button zoom_in_;
  Gtk::Button zoom_out_;
  Gtk::Button prev_;
  Gtk::Button next_;
  Gtk::Button del_image_;
  Gtk::Button del_ann_;
  Gtk::Button copy_name_;
  Gtk::Button copy_curr_;
  Gtk::Button ann_db_open_;
  Gtk::Button images_folder_open_;

  Gtk::ListBox images_list_;
  Gtk::ListBox current_image_annotations_;
  Gtk::ListBox annotations_db_list_;

  Gtk::SearchEntry images_search_;
  Gtk::SearchEntry annotation_search_;

  Gtk::Entry edit_rect_;

  Gtk::ImageMenuItem auto_insert_whole_;
  Gtk::ImageMenuItem images_open_mi_;
  Gtk::ImageMenuItem ann_db_open_mi_;
  Gtk::ImageMenuItem ann_save_mi_;
  Gtk::ImageMenuItem ann_saveas_mi_;
  Gtk::ImageMenuItem ann_close_mi_;
  Gtk::ImageMenuItem export_txt_mi_;
  Gtk::ImageMenuItem export_yolo4_mi_;
  Gtk::ImageMenuItem export_pytorch_mi_;
  Gtk::ImageMenuItem webpage_open_mi_;
  Gtk::ImageMenuItem about_mi_;
};

#endif  // IMAGES_ANNOTATOR_PROJECT_GTKMM_TESTS_EVENTHANDLERS_FAKE_WINDOW_LOADER_H
