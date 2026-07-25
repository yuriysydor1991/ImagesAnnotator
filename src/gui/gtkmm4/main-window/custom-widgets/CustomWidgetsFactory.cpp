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

#include "src/gui/gtkmm4/main-window/custom-widgets/CustomWidgetsFactory.h"

#include <cassert>
#include <glib.h>
#include <memory>
#include <string>
#include <vector>

#include "project-global-decls.h"
#include "src/annotator-events/events/ImageRecord.h"
#include "src/annotator-events/events/ImagesPathsDBProvider.h"
#include "src/gui/gtkmm4/MainWindowContext.h"
#include "src/gui/gtkmm4/main-window/WindowDataContext.h"
#include "src/gui/gtkmm4/main-window/custom-widgets/ImagePathLabel.h"
#include "src/gui/gtkmm4/main-window/custom-widgets/ImageRectsLabel.h"
#include "src/helpers/ImageLoader.h"
#include "src/log/log.h"

namespace templateGtkmm4::window::custom_widgets
{

namespace
{

// Reuse GTK's own localized standard-button labels instead of hardcoding
// English. The mnemonics passed below ("_Cancel", "_Save", ...) are verbatim
// GTK msgids, so g_dgettext() against GTK's gettext domain returns them
// translated to the user's locale, mnemonic preserved. Labels that are not part
// of GTK's catalog (e.g. "_Load", "_Leave") gracefully fall back to the msgid.
// Requires the locale to be initialized once, which Application::run() does via
// setlocale().
Glib::ustring gtk_std_label(const char* msgid)
{
  return g_dgettext("gtk40", msgid);
}

// The themed icon paired with each standard button label, restoring the
// pre-GTK4 stock-button icons. Empty string => no icon.
const char* std_button_icon(const std::string& msgid)
{
  if (msgid == "_Cancel") return "window-close-symbolic";
  if (msgid == "_Select") return "object-select-symbolic";
  if (msgid == "_Open" || msgid == "_Load") return "document-open-symbolic";
  if (msgid == "_Save") return "document-save-symbolic";
  if (msgid == "_Discard") return "edit-delete-symbolic";
  if (msgid == "_Delete") return "user-trash-symbolic";
  if (msgid == "_Leave") return "go-previous-symbolic";
  return "";
}

// Add a localized, icon-decorated standard action button to a dialog.
Gtk::Button* add_std_button(Gtk::Dialog& dialog, const char* msgid,
                            Gtk::ResponseType response)
{
  auto* button = dialog.add_button(gtk_std_label(msgid), response);
  CustomWidgetsFactory::add_button_icon(*button, std_button_icon(msgid));
  return button;
}

}  // namespace

void CustomWidgetsFactory::add_button_icon(Gtk::Button& button,
                                           const Glib::ustring& icon_name)
{
  if (icon_name.empty()) {
    return;
  }

  // GTK4 Gtk::Button shows only one of {label, icon-name, child}. To keep the
  // (already localized) text and add an icon beside it, rebuild the child as an
  // icon + label box - the same structure Adw::ButtonContent produces.
  const Glib::ustring text = button.get_label();

  auto* box = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL, 6);
  box->set_halign(Gtk::Align::CENTER);

  auto* image = Gtk::make_managed<Gtk::Image>();
  image->set_from_icon_name(icon_name);
  box->append(*image);

  if (!text.empty()) {
    auto* label = Gtk::make_managed<Gtk::Label>(text);
    label->set_use_underline(true);
    label->set_mnemonic_widget(button);
    box->append(*label);
  }

  button.set_child(*box);
}

std::shared_ptr<ImagePathLabel> CustomWidgetsFactory::create_image_db_label(
    std::shared_ptr<ImageRecord> ir)
{
  assert(ir != nullptr);

  if (ir == nullptr) {
    LOGE("No valid image record pointer provided");
    return {};
  }

  auto rt = std::make_shared<ImagePathLabel>(ir);

  if (!rt->get_image_rec()->rects.empty()) {
    rt->mark_as_has_records();
  }

  return rt;
}

CustomWidgetsFactory::ImagesVisualDB
CustomWidgetsFactory::create_images_visual_db(ImageRecordsSet& queue)
{
  ImagesVisualDB vdb;

  vdb.reserve(queue.size());

  for (auto& ir : queue) {
    vdb.emplace_back(create_image_db_label(ir));
  }

  return vdb;
}

std::shared_ptr<CentralWorkingCanvas>
CustomWidgetsFactory::create_working_canvas(
    std::shared_ptr<ImagePathLabel>& nci)
{
  auto canvas = std::make_shared<CentralWorkingCanvas>(nci);

  // GTK4 has no per-widget event masks; the input controllers attached by the
  // mouse handler deliver the button/motion events instead.
  return canvas;
}

std::shared_ptr<Gtk::FileChooserDialog>
CustomWidgetsFactory::create_json_db_dialog(Gtk::Window* parentWindow)
{
  auto dialog = std::make_shared<Gtk::FileChooserDialog>(
      *parentWindow, "Select annotations JSON db file",
      Gtk::FileChooser::Action::OPEN);

  // Add a JSON filter
  auto filter_json = Gtk::FileFilter::create();
  filter_json->set_name("JSON files");
  filter_json->add_pattern("*.json");
  dialog->add_filter(filter_json);

  // Allow all files as a fallback
  auto filter_all = Gtk::FileFilter::create();
  filter_all->set_name("All files");
  filter_all->add_pattern("*");
  dialog->add_filter(filter_all);

  add_std_button(*dialog, "_Cancel", Gtk::ResponseType::CANCEL);
  add_std_button(*dialog, "_Select", Gtk::ResponseType::OK);

  return dialog;
}

std::shared_ptr<Gtk::FileChooserDialog>
CustomWidgetsFactory::create_save_json_db_dialog(Gtk::Window* parentWindow)
{
  auto dialog = std::make_shared<Gtk::FileChooserDialog>(
      *parentWindow, "Save annotations JSON db file",
      Gtk::FileChooser::Action::SAVE);

  // Add a JSON filter
  auto filter_json = Gtk::FileFilter::create();
  filter_json->set_name("JSON files");
  filter_json->add_pattern("*.json");
  dialog->add_filter(filter_json);

  // Allow all files as a fallback
  auto filter_all = Gtk::FileFilter::create();
  filter_all->set_name("All files");
  filter_all->add_pattern("*");
  dialog->add_filter(filter_all);

  add_std_button(*dialog, "_Cancel", Gtk::ResponseType::CANCEL);
  add_std_button(*dialog, "_Select", Gtk::ResponseType::OK);

  return dialog;
}

std::shared_ptr<Gtk::FileChooserDialog>
CustomWidgetsFactory::create_folder_choose_dialog(Gtk::Window* parentWindow)
{
  auto dialog = std::make_shared<Gtk::FileChooserDialog>(
      *parentWindow, "Select directory containing source images",
      Gtk::FileChooser::Action::SELECT_FOLDER);

  add_std_button(*dialog, "_Cancel", Gtk::ResponseType::CANCEL);
  add_std_button(*dialog, "_Select", Gtk::ResponseType::OK);

  return dialog;
}

std::shared_ptr<Gtk::FileChooserDialog>
CustomWidgetsFactory::create_txt_export_folder_choose_dialog(
    Gtk::Window* parentWindow)
{
  return create_folder_choose_dialog_with_title(
      parentWindow, "Select plain txt files export directory");
}

std::shared_ptr<Gtk::FileChooserDialog>
CustomWidgetsFactory::create_yolo4_export_folder_choose_dialog(
    Gtk::Window* parentWindow)
{
  return create_folder_choose_dialog_with_title(
      parentWindow, "Select YOLO4 export directory");
}

std::shared_ptr<Gtk::FileChooserDialog>
CustomWidgetsFactory::create_pytorch_export_folder_choose_dialog(
    Gtk::Window* parentWindow)
{
  return create_folder_choose_dialog_with_title(
      parentWindow, "Select PyTorch Vision export directory");
}

std::shared_ptr<Gtk::FileChooserDialog>
CustomWidgetsFactory::create_folder_choose_dialog_with_title(
    Gtk::Window* parentWindow, const std::string& title)
{
  auto dialog = std::make_shared<Gtk::FileChooserDialog>(
      *parentWindow, title, Gtk::FileChooser::Action::SELECT_FOLDER);

  add_std_button(*dialog, "_Cancel", Gtk::ResponseType::CANCEL);
  add_std_button(*dialog, "_Select", Gtk::ResponseType::OK);

  return dialog;
}

ImageRectsLabelPtr CustomWidgetsFactory::create_rect_label(
    ImageRecordRectPtr rect)
{
  ImageRectsLabelPtr rt = std::make_shared<ImageRectsLabel>();

  rt->set(rect);

  return rt;
}

ImageRectsLabelSet CustomWidgetsFactory::create_rects_labels(
    ImageRecordRectSet& rectRecs)
{
  ImageRectsLabelSet rectsLabels;

  rectsLabels.reserve(rectRecs.size());

  for (auto& rr : rectRecs) {
    rectsLabels.emplace_back(create_rect_label(rr));
  }

  return rectsLabels;
}

CustomWidgetsFactory::AnnotationsVisualList
CustomWidgetsFactory::create_annotations_labels(const AnnotationsList& strList)
{
  AnnotationsVisualList list;

  list.reserve(strList.size());

  for (const auto& s : strList) {
    list.emplace_back(std::make_shared<AllAnnotationsLabel>(s));
  }

  return list;
}

void CustomWidgetsFactory::prepare_about(Gtk::AboutDialog* about,
                                         Gtk::Window* parentWindow)
{
  assert(about != nullptr);
  assert(parentWindow != nullptr);

  if (about == nullptr) {
    LOGE("No about window valid pointer was given");
    return;
  }

  if (parentWindow == nullptr) {
    LOGE("No parent window was given");
    return;
  }

  about->set_authors({project_decls::PROJECT_MAINTAINER + " " +
                      project_decls::PROJECT_MAINTAINER_EMAIL});
  about->set_website(project_decls::PROJECT_HOMEPAGE_URL);
  about->set_version(project_decls::PROJECT_BUILD_VERSION);
  about->set_program_name(project_decls::PROJECT_NAME);

  about->set_modal(true);
  about->set_transient_for(*parentWindow);
  about->set_comments(project_decls::PROJECT_DESCRIPTION +
                      "\ngit commit: " + project_decls::PROJECT_BUILD_COMMIT);

  try {
    auto pixbuf =
        Gdk::Pixbuf::create_from_resource(WindowDataContext::logo_res_path);

    // GTK4 AboutDialog::set_logo takes a Gdk::Paintable, not a Gdk::Pixbuf.
    about->set_logo(Gdk::Texture::create_for_pixbuf(pixbuf));
  }
  catch (const Glib::Error& ex) {
    LOGE("Failed to load icon: " << ex.what());
  }
}

std::shared_ptr<Gtk::MessageDialog> CustomWidgetsFactory::create_error_dialog(
    const std::string& errDesc, Gtk::Window* parentWindow)
{
  auto dialog = std::make_shared<Gtk::MessageDialog>(
      *parentWindow, "Error!", false, Gtk::MessageType::ERROR,
      Gtk::ButtonsType::OK,
      true  // modal
  );

  dialog->set_secondary_text(errDesc);

  return dialog;
}

std::shared_ptr<Gtk::MessageDialog>
CustomWidgetsFactory::create_save_changes_ask_user(Gtk::Window* parentWindow)
{
  auto dialog = std::make_shared<Gtk::MessageDialog>(
      *parentWindow, "Do you want to save changes?", false,
      Gtk::MessageType::QUESTION, Gtk::ButtonsType::NONE, true);

  add_std_button(*dialog, "_Cancel", Gtk::ResponseType::CANCEL);
  add_std_button(*dialog, "_Discard", Gtk::ResponseType::NO);
  add_std_button(*dialog, "_Save", Gtk::ResponseType::YES);

  dialog->set_default_response(Gtk::ResponseType::NO);

  return dialog;
}

std::shared_ptr<Gtk::MessageDialog>
CustomWidgetsFactory::create_ask_user_about_lost_rects(
    Gtk::Window* parentWindow)
{
  auto dialog = std::make_shared<Gtk::MessageDialog>(
      *parentWindow, "Images contains annotations. Delete them too?", false,
      Gtk::MessageType::QUESTION, Gtk::ButtonsType::NONE, true);

  add_std_button(*dialog, "_Cancel", Gtk::ResponseType::CANCEL);
  add_std_button(*dialog, "_Leave", Gtk::ResponseType::NO);
  add_std_button(*dialog, "_Delete", Gtk::ResponseType::YES);

  dialog->set_default_response(Gtk::ResponseType::NO);

  return dialog;
}

void CustomWidgetsFactory::prepare_url_asker(Gtk::Dialog* dialog,
                                             Gtk::Label* cachePathLabel,
                                             Gtk::Window* parentWindow)
{
  assert(dialog != nullptr);
  assert(cachePathLabel != nullptr);

  if (dialog == nullptr) {
    LOGE("Invalid dialog pointer provided");
    return;
  }

  if (cachePathLabel != nullptr) {
    LOGT("Setting up the network cache label info");
    cachePathLabel->set_text(
        "All the network cache will be stored at " +
        helpers::ImageLoader::get_cache_top_level().string());
  }

  if (parentWindow != nullptr) {
    LOGT("Setting parent for dialog");
    dialog->set_transient_for(*parentWindow);
  }

  if (dialog->get_widget_for_response(Gtk::ResponseType::OK) != nullptr) {
    LOGT("Dialog already has buttons");
    return;
  }

  add_std_button(*dialog, "_Cancel", Gtk::ResponseType::CANCEL);
  add_std_button(*dialog, "_Load", Gtk::ResponseType::OK);

  dialog->set_default_response(Gtk::ResponseType::OK);
}

}  // namespace templateGtkmm4::window::custom_widgets
