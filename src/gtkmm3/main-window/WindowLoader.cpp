#include "src/gtkmm3/main-window/WindowLoader.h"

#include <cassert>
#include <memory>

#include "src/gtkmm3/MainWindowContext.h"
#include "src/gtkmm3/gtkmm_includes.h"
#include "src/gtkmm3/main-window/WindowDataContext.h"
#include "src/gtkmm3/main-window/custom-widgets/ImagePathLabel.h"
#include "src/log/log.h"

namespace templateGtkmm3::window
{

bool WindowLoader::load_window(std::shared_ptr<MainWindowContext> nmwctx)
{
  assert(nmwctx != nullptr);

  if (nmwctx == nullptr) {
    LOGE("No context was given");
    return false;
  }

  mwctx = nmwctx;

  LOGD("Creating the builder from the path " << mwctx->wctx->UI_res_path);

  builder = Gtk::Builder::create_from_resource(mwctx->wctx->UI_res_path);

  if (!builder) {
    LOGE("Failed to create the builder");
    return false;
  }

  auto cwaPlace = get_place_4_working_widget();

  assert(cwaPlace != nullptr);

  if (cwaPlace == nullptr) {
    LOGE("Fail to retrieve central working area placeholder widget");
    return false;
  }

  nmwctx->centralCanvas =
      mwctx->cwFactory->create_working_canvas(nmwctx->current_image);

  assert(nmwctx->centralCanvas != nullptr);

  cwaPlace->add(*nmwctx->centralCanvas);

  if (!all_widget_are_valid()) {
    LOGE("Not all widgets are present");
    return false;
  }

  if (!propagate_params()) {
    LOGE("Failure to propagate params");
    return false;
  }

  return true;
}

bool WindowLoader::propagate_params()
{
  assert(builder);

  if (!builder) {
    LOGE("No builder present");
    return false;
  }

  if (!all_widget_are_valid()) {
    LOGE("Not all widgets are present");
    return false;
  }

  assert(get_window() != nullptr);

  get_window()->maximize();

  if (!propagate_css_params()) {
    LOGE("Fail to propagate the css into the app window");
    return false;
  }

  return true;
}

bool WindowLoader::propagate_css_params()
{
  assert(mwctx != nullptr);
  assert(mwctx->wctx != nullptr);

  auto css = Gtk::CssProvider::create();

  assert(css);

  css->load_from_resource(mwctx->wctx->main_window_css);

  auto screen = Gdk::Display::get_default()->get_default_screen();
  Gtk::StyleContext::add_provider_for_screen(
      screen, css, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

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
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::Window>(mwctx->wctx->window_id);
}

Gtk::Button* WindowLoader::get_images_folder_open_button()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::Button>(mwctx->wctx->images_folder_open_button_id);
}

Gtk::Button* WindowLoader::get_annotations_db_open_button()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::Button>(mwctx->wctx->annotations_db_open_button_id);
}

Gtk::Button* WindowLoader::get_delete_current_image_selected_annotation()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::Button>(
      mwctx->wctx->delete_current_image_selected_annotation_id);
}

Gtk::SearchEntry* WindowLoader::get_annotation_search_entry()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::SearchEntry>(mwctx->wctx->annotation_search_entry_id);
}

Gtk::ListBox* WindowLoader::get_annotations_db_list()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::ListBox>(mwctx->wctx->annotations_db_list_id);
}

Gtk::ListBox* WindowLoader::get_images_list()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::ListBox>(mwctx->wctx->images_list_id);
}

Gtk::Button* WindowLoader::get_current_image_zoom_in()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::Button>(mwctx->wctx->current_image_zoom_in_id);
}

Gtk::Button* WindowLoader::get_current_image_zoom_out()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::Button>(mwctx->wctx->current_image_zoom_out_id);
}

bool WindowLoader::all_widget_are_valid()
{
  if (mwctx->wctx == nullptr) {
    LOGE("No window context given");
    return false;
  }

  const auto& idslist = mwctx->wctx->get_ids();

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

Gtk::Viewport* WindowLoader::get_place_4_working_widget()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::Viewport>(mwctx->wctx->place_4_working_widget_id);
}

Gtk::ImageMenuItem* WindowLoader::get_images_open_mi()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::ImageMenuItem>(mwctx->wctx->images_open_menu_item_id);
}

Gtk::ImageMenuItem* WindowLoader::get_annotations_db_open_mi()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::ImageMenuItem>(mwctx->wctx->annotations_db_open_menu);
}

Gtk::ImageMenuItem* WindowLoader::get_annotations_db_save_mi()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::ImageMenuItem>(
      mwctx->wctx->annotations_db_save_menu_id);
}

Gtk::ImageMenuItem* WindowLoader::get_annotations_db_saveas_mi()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::ImageMenuItem>(
      mwctx->wctx->annotations_db_save_as_menu_id);
}

Gtk::ImageMenuItem* WindowLoader::get_annotations_project_close_mi()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::ImageMenuItem>(
      mwctx->wctx->annotations_project_close_id);
}

Gtk::ListBox* WindowLoader::get_current_image_annotations()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::ListBox>(mwctx->wctx->current_image_annotations_id);
}

Gtk::Button* WindowLoader::get_copy_annotation_name_button()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::Button>(
      mwctx->wctx->copy_annotation_name_to_current_rect_id);
}

Gtk::Entry* WindowLoader::get_edit_current_rect_entry()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::Entry>(
      mwctx->wctx->current_image_selected_annotation_edit_id);
}

}  // namespace templateGtkmm3::window
