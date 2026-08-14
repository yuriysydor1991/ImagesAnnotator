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

#include "src/gui/gtkmm4/main-window/WindowLoader.h"

#include <glibmm/main.h>

#include <cassert>
#include <memory>

#include "project-global-decls.h"
#include "src/gui/gtkmm4/MainWindowContext.h"
#include "src/gui/gtkmm4/gtkmm_includes.h"
#include "src/gui/gtkmm4/main-window/WindowDataContext.h"
#include "src/gui/gtkmm4/main-window/custom-widgets/ImagePathLabel.h"
#include "src/log/log.h"

namespace imagesAnnotatorGtkmm4::window
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

  // GTK4: single-child containers use set_child() instead of add().
  cwaPlace->set_child(*nmwctx->centralCanvas);

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
  set_window_icon();

  if (!propagate_css_params()) {
    LOGE("Fail to propagate the css into the app window");
    return false;
  }

  if (!attach_spinner()) {
    LOGE("Failure while creating window spinner");
    return false;
  }

  assert(mwctx->centralCanvas != nullptr);

  mwctx->centralCanvas->set_halign(Gtk::Align::CENTER);

  if (!create_actions()) {
    LOGE("Failure during the menu actions creation");
    return false;
  }

  if (!init_paned_layout()) {
    LOGE("Failure during the columns layout initialization");
    return false;
  }

  decorate_button_icons();

  if (!prepare_dialogs()) {
    LOGE("Failure during the dialogs preparation");
    return false;
  }

  return true;
}

void WindowLoader::decorate_button_icons()
{
  using custom_widgets::CustomWidgetsFactory;

  // Give the main-window action buttons the same stock-style icon + label look
  // as the dialog buttons, using themed (Adwaita) freedesktop icon names.
  CustomWidgetsFactory::add_button_icon(*get_images_folder_open_button(),
                                        "folder-open-symbolic");
  CustomWidgetsFactory::add_button_icon(*get_annotations_db_open_button(),
                                        "document-open-symbolic");
  CustomWidgetsFactory::add_button_icon(*get_current_image_zoom_in(),
                                        "zoom-in-symbolic");
  CustomWidgetsFactory::add_button_icon(*get_current_image_zoom_out(),
                                        "zoom-out-symbolic");
  CustomWidgetsFactory::add_button_icon(*get_prev_file_button(),
                                        "go-previous-symbolic");
  CustomWidgetsFactory::add_button_icon(*get_next_file_button(),
                                        "go-next-symbolic");
  CustomWidgetsFactory::add_button_icon(*get_delete_image_record(),
                                        "user-trash-symbolic");
  CustomWidgetsFactory::add_button_icon(
      *get_delete_current_image_selected_annotation(), "edit-delete-symbolic");
  CustomWidgetsFactory::add_button_icon(*get_copy_annotation_name_button(),
                                        "edit-copy-symbolic");
  CustomWidgetsFactory::add_button_icon(*get_copy_current_annotation_button(),
                                        "edit-copy-symbolic");
}

bool WindowLoader::prepare_dialogs()
{
  assert(mwctx != nullptr);
  assert(mwctx->cwFactory != nullptr);
  assert(get_window() != nullptr);

  auto& factory = *mwctx->cwFactory;
  auto& parent = *get_window();

  auto* jsonOpen = get_json_db_open_dialog();
  auto* jsonSave = get_json_db_save_dialog();
  auto* imagesFolder = get_images_folder_choose_dialog();
  auto* txtFolder = get_txt_export_folder_dialog();
  auto* yolo4Folder = get_yolo4_export_folder_dialog();
  auto* pytorchFolder = get_pytorch_export_folder_dialog();
  auto* error = get_error_dialog();
  auto* saveChanges = get_save_changes_ask_dialog();
  auto* lostRects = get_lost_rects_ask_dialog();

  // The C++ dialogs took the main window as their transient parent at
  // construction; the builder-defined shells are reparented onto it here.
  for (Gtk::Window* dialog :
       {static_cast<Gtk::Window*>(jsonOpen), static_cast<Gtk::Window*>(jsonSave),
        static_cast<Gtk::Window*>(imagesFolder),
        static_cast<Gtk::Window*>(txtFolder),
        static_cast<Gtk::Window*>(yolo4Folder),
        static_cast<Gtk::Window*>(pytorchFolder),
        static_cast<Gtk::Window*>(error),
        static_cast<Gtk::Window*>(saveChanges),
        static_cast<Gtk::Window*>(lostRects)}) {
    assert(dialog != nullptr);
    dialog->set_transient_for(parent);
  }

  // Apply the one-time decoration GtkBuilder cannot express: the JSON dialogs
  // get the file filters plus the localized Cancel / Select buttons, the folder
  // choosers just the buttons, and the two question dialogs their button sets.
  // The error dialog needs neither - its GTK-localized OK button is declared in
  // the .ui via buttons=ok.
  factory.prepare_json_db_dialog(jsonOpen);
  factory.prepare_json_db_dialog(jsonSave);
  factory.prepare_folder_choose_dialog(imagesFolder);
  factory.prepare_folder_choose_dialog(txtFolder);
  factory.prepare_folder_choose_dialog(yolo4Folder);
  factory.prepare_folder_choose_dialog(pytorchFolder);
  factory.prepare_save_changes_ask_user(saveChanges);
  factory.prepare_ask_user_about_lost_rects(lostRects);

  return true;
}

bool WindowLoader::init_paned_layout()
{
  auto* outer =
      get_widget<Gtk::Paned>(WindowDataContext::main_columns_paned_id);
  auto* inner =
      get_widget<Gtk::Paned>(WindowDataContext::left_columns_paned_id);

  assert(outer != nullptr);
  assert(inner != nullptr);

  // The central image working area is the primary widget. Only the central
  // column grows or shrinks with the window; the left (images) and right
  // (annotations) management panels keep their width. This mirrors the GTKmm3
  // layout and stops the side panels from squishing the image area (it also
  // makes the split robust to the asynchronous window maximization: since the
  // sides do not resize, they keep the width set below).
  outer->set_resize_start_child(true);  // left + central column absorbs resizes
  outer->set_resize_end_child(false);   // annotations panel keeps its width
  inner->set_resize_start_child(false);  // images panel keeps its width
  inner->set_resize_end_child(true);     // central image area absorbs resizes

  // The paned width is only known once the (maximized) window has been mapped
  // and laid out, so the initial split is applied a single time from an idle
  // callback scheduled on the first map.
  outer->signal_map().connect([this, outer, inner]() {
    if (paned_positions_done) {
      return;
    }
    paned_positions_done = true;

    // Retry on idle until the paned has been allocated a real width, then apply
    // the split once and stop.
    Glib::signal_idle().connect([outer, inner]() -> bool {
      const int w = outer->get_width();

      if (w <= 0) {
        return true;  // not laid out yet, try again on the next idle
      }

      // Give each side management panel a comfortable fixed width; the central
      // image column takes all the remaining space.
      outer->set_position(w - side_panel_width);  // annotations panel width
      inner->set_position(side_panel_width);      // images panel width

      return false;  // done
    });
  });

  return true;
}

bool WindowLoader::attach_spinner()
{
  auto* overlay = get_main_overlay();
  auto* spinner = get_spinner();

  assert(overlay != nullptr);
  assert(spinner != nullptr);

  overlay->add_overlay(*spinner);

  spinner->set_valign(Gtk::Align::CENTER);
  spinner->set_halign(Gtk::Align::CENTER);
  spinner->set_size_request(spinner_size, spinner_size);

  return true;
}

bool WindowLoader::create_actions()
{
  assert(get_window() != nullptr);

  // GTK4 replaces GtkImageMenuItem widgets with a Gio::Menu model plus
  // Gio::SimpleActions. The actions live in an action group installed on the
  // window under the "win" prefix; the menu model (GtkmmWindow.ui) references
  // them as "win.<name>".
  actionGroup = Gio::SimpleActionGroup::create();

  action_images_open =
      actionGroup->add_action(WindowDataContext::action_images_open);
  action_annotations_db_open =
      actionGroup->add_action(WindowDataContext::action_annotations_db_open);
  action_annotations_db_save =
      actionGroup->add_action(WindowDataContext::action_annotations_db_save);
  action_annotations_db_saveas =
      actionGroup->add_action(WindowDataContext::action_annotations_db_saveas);
  action_annotations_project_close = actionGroup->add_action(
      WindowDataContext::action_annotations_project_close);
  action_about = actionGroup->add_action(WindowDataContext::action_about);
  action_export_txt =
      actionGroup->add_action(WindowDataContext::action_export_txt);
  action_export_yolo4 =
      actionGroup->add_action(WindowDataContext::action_export_yolo4);
  action_export_pytorchvision =
      actionGroup->add_action(WindowDataContext::action_export_pytorchvision);
  action_images_web_page_open =
      actionGroup->add_action(WindowDataContext::action_images_web_page_open);
  action_auto_insert_annotation =
      actionGroup->add_action(WindowDataContext::action_auto_insert_annotation);

  get_window()->insert_action_group(WindowDataContext::action_group_prefix,
                                    actionGroup);

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

void WindowLoader::set_window_icon()
{
  // GTK4 dropped the pixbuf window-icon API (there is no Gtk::Window::set_icon);
  // the only accessible route is the themed-name one. set_default_icon_name()
  // points every window at the icon named after the application id
  // (project_decls::PROJECT_FLATPAK_URL - the same id handed to
  // Gtk::Application::create), which GTK and, on Wayland, the compositor / GNOME
  // Shell resolve against the icon theme. This is the in-binary half of the
  // wiring: the icon shows on the panel once a matching <app-id>.png +
  // <app-id>.desktop are installed into the icon theme, and it already lets a
  // running window associate with that launcher.
  Gtk::Window::set_default_icon_name(project_decls::PROJECT_FLATPAK_URL);
}

bool WindowLoader::propagate_css_params()
{
  assert(mwctx != nullptr);
  assert(mwctx->wctx != nullptr);

  auto css = Gtk::CssProvider::create();

  assert(css);

  css->load_from_resource(mwctx->wctx->main_window_css);

  // GTK4 attaches CSS to a Gdk::Display, not a Gdk::Screen.
  auto display = Gdk::Display::get_default();
  Gtk::StyleProvider::add_provider_for_display(
      display, css, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

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

  WidgetType* tptr = builder->get_widget<WidgetType>(id);

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

Glib::RefPtr<Gio::SimpleAction> WindowLoader::get_images_open_action()
{
  return action_images_open;
}

Glib::RefPtr<Gio::SimpleAction> WindowLoader::get_annotations_db_open_action()
{
  return action_annotations_db_open;
}

Glib::RefPtr<Gio::SimpleAction> WindowLoader::get_annotations_db_save_action()
{
  return action_annotations_db_save;
}

Glib::RefPtr<Gio::SimpleAction> WindowLoader::get_annotations_db_saveas_action()
{
  return action_annotations_db_saveas;
}

Glib::RefPtr<Gio::SimpleAction>
WindowLoader::get_annotations_project_close_action()
{
  return action_annotations_project_close;
}

Glib::RefPtr<Gio::SimpleAction> WindowLoader::get_about_action()
{
  return action_about;
}

Glib::RefPtr<Gio::SimpleAction> WindowLoader::get_export_txt_action()
{
  return action_export_txt;
}

Glib::RefPtr<Gio::SimpleAction> WindowLoader::get_export_yolo4_action()
{
  return action_export_yolo4;
}

Glib::RefPtr<Gio::SimpleAction> WindowLoader::get_export_pytorchvision_action()
{
  return action_export_pytorchvision;
}

Glib::RefPtr<Gio::SimpleAction> WindowLoader::get_images_web_page_open_action()
{
  return action_images_web_page_open;
}

Glib::RefPtr<Gio::SimpleAction>
WindowLoader::get_auto_insert_annotation_action()
{
  return action_auto_insert_annotation;
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

Gtk::FileChooserDialog* WindowLoader::get_json_db_open_dialog()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::FileChooserDialog>(
      mwctx->wctx->json_db_open_dialog_id);
}

Gtk::FileChooserDialog* WindowLoader::get_json_db_save_dialog()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::FileChooserDialog>(
      mwctx->wctx->json_db_save_dialog_id);
}

Gtk::FileChooserDialog* WindowLoader::get_images_folder_choose_dialog()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::FileChooserDialog>(
      mwctx->wctx->images_folder_choose_dialog_id);
}

Gtk::FileChooserDialog* WindowLoader::get_txt_export_folder_dialog()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::FileChooserDialog>(
      mwctx->wctx->txt_export_folder_dialog_id);
}

Gtk::FileChooserDialog* WindowLoader::get_yolo4_export_folder_dialog()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::FileChooserDialog>(
      mwctx->wctx->yolo4_export_folder_dialog_id);
}

Gtk::FileChooserDialog* WindowLoader::get_pytorch_export_folder_dialog()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::FileChooserDialog>(
      mwctx->wctx->pytorch_export_folder_dialog_id);
}

Gtk::MessageDialog* WindowLoader::get_error_dialog()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::MessageDialog>(mwctx->wctx->error_dialog_id);
}

Gtk::MessageDialog* WindowLoader::get_save_changes_ask_dialog()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::MessageDialog>(mwctx->wctx->save_changes_ask_dialog_id);
}

Gtk::MessageDialog* WindowLoader::get_lost_rects_ask_dialog()
{
  assert(mwctx->wctx != nullptr);

  return get_widget<Gtk::MessageDialog>(mwctx->wctx->lost_rects_ask_dialog_id);
}

}  // namespace imagesAnnotatorGtkmm4::window
