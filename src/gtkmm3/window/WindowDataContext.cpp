#include "src/gtkmm3/window/WindowDataContext.h"

#include <cassert>
#include <memory>

#include "src/gtkmm3/gtkmm_includes.h"
#include "src/log/log.h"

namespace templateGtkmm3::window
{

bool WindowDataContext::all_widget_pointers_valid()
{
  return window != nullptr && images_open_button != nullptr &&
         annotations_open_button != nullptr &&
         accept_annotations_button != nullptr &&
         delete_image_annotation_button != nullptr &&
         annotations_search_entry != nullptr &&
         annotations_db_list != nullptr && images_list != nullptr &&
         image != nullptr;
}

const WindowDataContext::IdsT& WindowDataContext::get_ids()
{
  static const IdsT ids{window_id,
                        images_folder_open_button_id,
                        annotations_db_open_button_id,
                        accept_annotation_button_id,
                        delete_current_image_selected_annotation_id,
                        annotation_search_entry_id,
                        annotations_db_list_id,
                        images_list_id,
                        central_working_image_id};

  return ids;
}

/* The C++ containers can't store a references to the pointers or even
 * std::reference_wrapper-s to casted pointers so here is the code.
 */
void WindowDataContext::set_ptr(const std::string& id, Gtk::Widget* ptr)
{
  LOGT("Trying to find a widget for the pointer with id: " << id);

  Gtk::Window* twindow = dynamic_cast<Gtk::Window*>(ptr);
  if (twindow != nullptr && id == window_id) {
    window = twindow;
    return;
  }

  Gtk::Button* timages_open_button = dynamic_cast<Gtk::Button*>(ptr);
  if (timages_open_button != nullptr && id == images_folder_open_button_id) {
    images_open_button = timages_open_button;
    return;
  }

  Gtk::Button* tannotations_open_button = dynamic_cast<Gtk::Button*>(ptr);
  if (tannotations_open_button != nullptr &&
      id == annotations_db_open_button_id) {
    annotations_open_button = tannotations_open_button;
    return;
  }

  Gtk::Button* taccept_annotations_button = dynamic_cast<Gtk::Button*>(ptr);
  if (taccept_annotations_button != nullptr &&
      id == accept_annotation_button_id) {
    accept_annotations_button = taccept_annotations_button;
    return;
  }

  Gtk::Button* tdelete_image_annotation_button =
      dynamic_cast<Gtk::Button*>(ptr);
  if (tdelete_image_annotation_button != nullptr &&
      id == delete_current_image_selected_annotation_id) {
    delete_image_annotation_button = tdelete_image_annotation_button;
    return;
  }

  Gtk::SearchEntry* tannotations_search_entry =
      dynamic_cast<Gtk::SearchEntry*>(ptr);
  if (tannotations_search_entry != nullptr &&
      id == annotation_search_entry_id) {
    annotations_search_entry = tannotations_search_entry;
    return;
  }

  Gtk::ListBox* tannotations_db_list = dynamic_cast<Gtk::ListBox*>(ptr);
  if (tannotations_db_list != nullptr && id == annotations_db_list_id) {
    annotations_db_list = tannotations_db_list;
    return;
  }

  Gtk::ListBox* timages_list = dynamic_cast<Gtk::ListBox*>(ptr);
  if (timages_list != nullptr && id == images_list_id) {
    images_list = timages_list;
    return;
  }

  Gtk::Image* timage = dynamic_cast<Gtk::Image*>(ptr);
  if (timage != nullptr && id == central_working_image_id) {
    image = timage;
    return;
  }

  LOGE("Given pointer to the unknown object for the id: " << id);

  assert(false);
}

}  // namespace templateGtkmm3::window
