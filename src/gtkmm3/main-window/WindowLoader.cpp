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
  update_window_title();

  if (!propagate_css_params()) {
    LOGE("Fail to propagate the css into the app window");
    return false;
  }

  if (!attach_spinner()) {
    LOGE("Failure while creating window spinner");
    return false;
  }

  assert(mwctx->centralCanvas != nullptr);

  mwctx->centralCanvas->set_halign(Gtk::ALIGN_CENTER);

  if (!propagate_menu_shortcuts()) {
    LOGE("Failure during shortcuts propagation");
    return false;
  }

  return true;
}

bool WindowLoader::attach_spinner()
{
  auto* overlay = get_main_overlay();
  auto* spinner = get_spinner();

  assert(overlay != nullptr);
  assert(spinner != nullptr);

  overlay->add_overlay(*spinner);

  spinner->set_valign(Gtk::ALIGN_CENTER);
  spinner->set_halign(Gtk::ALIGN_CENTER);
  spinner->set_size_request(spinner_size, spinner_size);

  return true;
}

bool WindowLoader::propagate_menu_shortcuts()
{
  assert(builder);

  if (!builder) {
    LOGE("No builder present");
    return false;
  }

  auto accel_group = Gtk::AccelGroup::create();

  // Add Ctrl+O accelerator to item_open
  get_annotations_db_open_mi()->add_accelerator("activate", accel_group,
                                                GDK_KEY_o, Gdk::CONTROL_MASK,
                                                Gtk::ACCEL_VISIBLE);

  get_annotations_db_save_mi()->add_accelerator("activate", accel_group,
                                                GDK_KEY_s, Gdk::CONTROL_MASK,
                                                Gtk::ACCEL_VISIBLE);

  get_annotations_db_saveas_mi()->add_accelerator(
      "activate", accel_group, GDK_KEY_s, Gdk::CONTROL_MASK | Gdk::SHIFT_MASK,
      Gtk::ACCEL_VISIBLE);

  get_annotations_project_close_mi()->add_accelerator(
      "activate", accel_group, GDK_KEY_w, Gdk::CONTROL_MASK,
      Gtk::ACCEL_VISIBLE);

  get_images_open_mi()->add_accelerator("activate", accel_group, GDK_KEY_o,
                                        Gdk::CONTROL_MASK | Gdk::SHIFT_MASK,
                                        Gtk::ACCEL_VISIBLE);

  get_images_web_page_open_menu_item_mi()->add_accelerator(
      "activate", accel_group, GDK_KEY_u, Gdk::CONTROL_MASK | Gdk::SHIFT_MASK,
      Gtk::ACCEL_VISIBLE);

  get_window()->add_accel_group(accel_group);

  return true;
}

void WindowLoader::update_window_title(const std::string& openedFile)
{
  static const std::string basicTitle =
      project_decls::PROJECT_NAME + " " + project_decls::PROJECT_BUILD_VERSION;

  const auto usedTitle =
      openedFile.empty() ? basicTitle : basicTitle + " [" + openedFile + "]";

  get_window()->set_title(usedTitle);
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

  if (tptr == nullptr) {
    LOGE("Failure during retrieving the widget by id: " << id);
  }

  assert(tptr != nullptr);

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

Gtk::Button* WindowLoader::get_prev_file_button()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::Button>(mwctx->wctx->previous_image_button_id);
}

Gtk::Button* WindowLoader::get_next_file_button()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::Button>(mwctx->wctx->next_image_button_id);
}

Gtk::Statusbar* WindowLoader::get_window_status_bar()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::Statusbar>(mwctx->wctx->main_window_status_bar_id);
}

Gtk::Button* WindowLoader::get_copy_current_annotation_button()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::Button>(mwctx->wctx->duplicate_current_annotation_id);
}

Gtk::Overlay* WindowLoader::get_main_overlay()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::Overlay>(mwctx->wctx->main_window_overlay_id);
}

Gtk::Spinner* WindowLoader::get_spinner()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::Spinner>(mwctx->wctx->spinner_id);
}

Gtk::ScrolledWindow* WindowLoader::get_central_scrolled_window()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::ScrolledWindow>(
      mwctx->wctx->central_canvas_scrolled_window_id);
}

Gtk::AboutDialog* WindowLoader::get_about()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::AboutDialog>(mwctx->wctx->about_dialog_id);
}

Gtk::ImageMenuItem* WindowLoader::get_about_mi()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::ImageMenuItem>(mwctx->wctx->about_menu_item_id);
}

Gtk::ImageMenuItem* WindowLoader::get_export_txt2_folder_mi()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::ImageMenuItem>(
      mwctx->wctx->annotations_export_txt_2_folder_id);
}

Gtk::ImageMenuItem* WindowLoader::get_export_yolo4_folder_mi()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::ImageMenuItem>(
      mwctx->wctx->annotations_export_yolo4_2_folder2_id);
}

Gtk::ImageMenuItem* WindowLoader::get_export_pytorchvision_folder_mi()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::ImageMenuItem>(
      mwctx->wctx->annotations_export_pytorchvision_2_folder_id);
}

Gtk::SearchEntry* WindowLoader::get_images_search_entry()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::SearchEntry>(mwctx->wctx->images_search_entry_id);
}

Gtk::Button* WindowLoader::get_delete_image_record()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::Button>(mwctx->wctx->delete_image_record_id);
}

Gtk::Dialog* WindowLoader::get_web_page_url_asker()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::Dialog>(mwctx->wctx->web_page_images_url_asker_id);
}

Gtk::ImageMenuItem* WindowLoader::get_images_web_page_open_menu_item_mi()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::ImageMenuItem>(
      mwctx->wctx->images_web_page_open_menu_item_id);
}

Gtk::Entry* WindowLoader::get_images_web_page_url_entry()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::Entry>(mwctx->wctx->images_web_page_url_entry_id);
}

Gtk::Label* WindowLoader::get_images_web_page_cache_folder()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::Label>(mwctx->wctx->images_web_page_cache_folder_id);
}

}  // namespace templateGtkmm3::window
