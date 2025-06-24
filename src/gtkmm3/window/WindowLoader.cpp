#include "src/gtkmm3/window/WindowLoader.h"

#include <cassert>
#include <memory>

#include "src/gtkmm3/gtkmm_includes.h"
#include "src/gtkmm3/window/WindowDataContext.h"
#include "src/log/log.h"

namespace templateGtkmm3::window
{

bool WindowLoader::load_window(std::shared_ptr<WindowDataContext> nwctx)
{
  assert(nwctx != nullptr);

  if (nwctx == nullptr) {
    LOGE("No context was given");
    return false;
  }

  wctx = nwctx;

  LOGD("Creating the builder from the path " << wctx->UI_res_path);

  builder = Gtk::Builder::create_from_resource(wctx->UI_res_path);

  if (!builder) {
    LOGE("Failed to create the builder");
    return false;
  }

  return true;
}

template <class WidgetType>
WidgetType* WindowLoader::get_widget(const std::string& id)
{
  assert(builder);

  if (!builder) {
    LOGE("No builder available");
    return {};
  }

  LOGT("Trying to find widget by id: " << id);

  WidgetType* tptr{nullptr};

  builder->get_widget(id, tptr);

  assert(tptr != nullptr);

  if (tptr == nullptr) {
    LOGE("Failure during retrieving the widget by id: " << id);
  }

  return tptr;
}

Gtk::Window* WindowLoader::get_window()
{
  assert(wctx != nullptr);

  return get_widget<Gtk::Window>(wctx->window_id);
}

Gtk::Button* WindowLoader::get_images_folder_open_button()
{
  assert(wctx != nullptr);

  return get_widget<Gtk::Button>(wctx->images_folder_open_button_id);
}

Gtk::Button* WindowLoader::get_annotations_db_open_button()
{
  assert(wctx != nullptr);

  return get_widget<Gtk::Button>(wctx->annotations_db_open_button_id);
}

Gtk::Button* WindowLoader::get_accept_annotation_button()
{
  assert(wctx != nullptr);

  return get_widget<Gtk::Button>(wctx->accept_annotation_button_id);
}

Gtk::Button* WindowLoader::get_delete_current_image_selected_annotation()
{
  assert(wctx != nullptr);

  return get_widget<Gtk::Button>(
      wctx->delete_current_image_selected_annotation_id);
}

Gtk::SearchEntry* WindowLoader::get_annotation_search_entry()
{
  assert(wctx != nullptr);

  return get_widget<Gtk::SearchEntry>(wctx->annotation_search_entry_id);
}

Gtk::ListBox* WindowLoader::get_annotations_db_list()
{
  assert(wctx != nullptr);

  return get_widget<Gtk::ListBox>(wctx->annotations_db_list_id);
}

Gtk::ListBox* WindowLoader::get_images_list()
{
  assert(wctx != nullptr);

  return get_widget<Gtk::ListBox>(wctx->images_list_id);
}

Gtk::Image* WindowLoader::get_image()
{
  assert(wctx != nullptr);

  return get_widget<Gtk::Image>(wctx->central_working_image_id);
}

bool WindowLoader::all_widget_are_valid()
{
  if (wctx == nullptr) {
    LOGE("No window context given");
    return false;
  }

  const auto& idslist = wctx->get_ids();

  for (const auto& id : idslist) {
    Gtk::Widget* tptr = get_widget<Gtk::Widget>(id);

    if (tptr == nullptr) {
      LOGE("Found the absent widget by id: " << id);
      return false;
    }

    LOGT("Widget " << id << " is present");
  }

  return true;
}

}  // namespace templateGtkmm3::window
