#ifndef IMAGES_ANNOTATOR_PROJECT_GTKMM4_TESTS_EVENTHANDLERS_FAKE_WINDOW_LOADER_H
#define IMAGES_ANNOTATOR_PROJECT_GTKMM4_TESTS_EVENTHANDLERS_FAKE_WINDOW_LOADER_H

#include <gtkmm.h>

#include "src/gui/gtkmm4/main-window/WindowLoader.h"

/**
 * @brief Test double for the WindowLoader used by the event handler component
 * tests.
 *
 * The real WindowLoader populates its widgets from the embedded GResource UI
 * file (Gtk::Builder::create_from_resource) and installs the "win.*" menu
 * actions. The event handlers only reach the widgets and actions through the
 * WindowLoader's virtual getters, so this double overrides those getters to
 * return real, owned GTK widgets and Gio::SimpleActions constructed directly.
 * That lets the handlers' subscribe() wiring be exercised without loading the
 * UI resource, while still connecting against genuine GTK signals.
 *
 * Must only be instantiated once GTK has been initialized (see GtkInitFixture).
 */
class FakeWindowLoader : public imagesAnnotatorGtkmm4::window::WindowLoader
{
 public:
  FakeWindowLoader()
      : images_open_a_{Gio::SimpleAction::create("images-open")},
        ann_db_open_a_{Gio::SimpleAction::create("annotations-db-open")},
        ann_save_a_{Gio::SimpleAction::create("annotations-db-save")},
        ann_saveas_a_{Gio::SimpleAction::create("annotations-db-saveas")},
        ann_close_a_{Gio::SimpleAction::create("annotations-project-close")},
        about_a_{Gio::SimpleAction::create("about")},
        export_txt_a_{Gio::SimpleAction::create("export-txt")},
        export_yolo4_a_{Gio::SimpleAction::create("export-yolo4")},
        export_pytorch_a_{Gio::SimpleAction::create("export-pytorchvision")},
        export_ultralytics_detect_a_{
            Gio::SimpleAction::create("export-ultralytics-detect")},
        export_ultralytics_obb_a_{
            Gio::SimpleAction::create("export-ultralytics-obb")},
        export_ultralytics_segment_a_{
            Gio::SimpleAction::create("export-ultralytics-segment")},
        export_coco_a_{Gio::SimpleAction::create("export-coco")},
        webpage_open_a_{Gio::SimpleAction::create("images-web-page-open")},
        auto_insert_a_{Gio::SimpleAction::create("auto-insert-annotation")}
  {
  }

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

  Gtk::Button* get_annotations_db_open_button() override
  {
    return &ann_db_open_;
  }
  Gtk::Button* get_images_folder_open_button() override
  {
    return &images_folder_open_;
  }

  // GTK4 menu entries are Gio::Actions rather than GtkImageMenuItem widgets.
  Glib::RefPtr<Gio::SimpleAction> get_images_open_action() override
  {
    return images_open_a_;
  }
  Glib::RefPtr<Gio::SimpleAction> get_annotations_db_open_action() override
  {
    return ann_db_open_a_;
  }
  Glib::RefPtr<Gio::SimpleAction> get_annotations_db_save_action() override
  {
    return ann_save_a_;
  }
  Glib::RefPtr<Gio::SimpleAction> get_annotations_db_saveas_action() override
  {
    return ann_saveas_a_;
  }
  Glib::RefPtr<Gio::SimpleAction> get_annotations_project_close_action()
      override
  {
    return ann_close_a_;
  }
  Glib::RefPtr<Gio::SimpleAction> get_about_action() override
  {
    return about_a_;
  }
  Glib::RefPtr<Gio::SimpleAction> get_export_txt_action() override
  {
    return export_txt_a_;
  }
  Glib::RefPtr<Gio::SimpleAction> get_export_yolo4_action() override
  {
    return export_yolo4_a_;
  }
  Glib::RefPtr<Gio::SimpleAction> get_export_pytorchvision_action() override
  {
    return export_pytorch_a_;
  }
  Glib::RefPtr<Gio::SimpleAction> get_export_ultralytics_detect_action()
      override
  {
    return export_ultralytics_detect_a_;
  }
  Glib::RefPtr<Gio::SimpleAction> get_export_ultralytics_obb_action() override
  {
    return export_ultralytics_obb_a_;
  }
  Glib::RefPtr<Gio::SimpleAction> get_export_ultralytics_segment_action()
      override
  {
    return export_ultralytics_segment_a_;
  }
  Glib::RefPtr<Gio::SimpleAction> get_export_coco_action() override
  {
    return export_coco_a_;
  }
  Glib::RefPtr<Gio::SimpleAction> get_images_web_page_open_action() override
  {
    return webpage_open_a_;
  }
  Glib::RefPtr<Gio::SimpleAction> get_auto_insert_annotation_action() override
  {
    return auto_insert_a_;
  }

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

  Glib::RefPtr<Gio::SimpleAction> images_open_a_;
  Glib::RefPtr<Gio::SimpleAction> ann_db_open_a_;
  Glib::RefPtr<Gio::SimpleAction> ann_save_a_;
  Glib::RefPtr<Gio::SimpleAction> ann_saveas_a_;
  Glib::RefPtr<Gio::SimpleAction> ann_close_a_;
  Glib::RefPtr<Gio::SimpleAction> about_a_;
  Glib::RefPtr<Gio::SimpleAction> export_txt_a_;
  Glib::RefPtr<Gio::SimpleAction> export_yolo4_a_;
  Glib::RefPtr<Gio::SimpleAction> export_pytorch_a_;
  Glib::RefPtr<Gio::SimpleAction> export_ultralytics_detect_a_;
  Glib::RefPtr<Gio::SimpleAction> export_ultralytics_obb_a_;
  Glib::RefPtr<Gio::SimpleAction> export_ultralytics_segment_a_;
  Glib::RefPtr<Gio::SimpleAction> export_coco_a_;
  Glib::RefPtr<Gio::SimpleAction> webpage_open_a_;
  Glib::RefPtr<Gio::SimpleAction> auto_insert_a_;
};

#endif  // IMAGES_ANNOTATOR_PROJECT_GTKMM4_TESTS_EVENTHANDLERS_FAKE_WINDOW_LOADER_H
