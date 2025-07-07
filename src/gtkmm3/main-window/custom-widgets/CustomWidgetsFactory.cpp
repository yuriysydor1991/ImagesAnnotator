#include "src/gtkmm3/main-window/custom-widgets/CustomWidgetsFactory.h"

#include <cassert>
#include <memory>
#include <vector>

#include "project-global-decls.h"
#include "src/annotator-events/events/ImageRecord.h"
#include "src/annotator-events/events/ImagesPathsDBProvider.h"
#include "src/gtkmm3/MainWindowContext.h"
#include "src/gtkmm3/main-window/WindowDataContext.h"
#include "src/gtkmm3/main-window/custom-widgets/ImagePathLabel.h"
#include "src/gtkmm3/main-window/custom-widgets/ImageRectsLabel.h"
#include "src/log/log.h"

namespace templateGtkmm3::window::custom_widgets
{

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

  canvas->add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK |
                     Gdk::POINTER_MOTION_MASK);

  return canvas;
}

std::shared_ptr<Gtk::FileChooserDialog>
CustomWidgetsFactory::create_json_db_dialog(Gtk::Window* parentWindow)
{
  auto dialog = std::make_shared<Gtk::FileChooserDialog>(
      *parentWindow, "Select annotations JSON db file",
      Gtk::FILE_CHOOSER_ACTION_OPEN);

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

  dialog->add_button("_Cancel", Gtk::RESPONSE_CANCEL);
  dialog->add_button("_Select", Gtk::RESPONSE_OK);

  return dialog;
}

std::shared_ptr<Gtk::FileChooserDialog>
CustomWidgetsFactory::create_save_json_db_dialog(Gtk::Window* parentWindow)
{
  auto dialog = std::make_shared<Gtk::FileChooserDialog>(
      *parentWindow, "Save annotations JSON db file",
      Gtk::FILE_CHOOSER_ACTION_SAVE);

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

  dialog->add_button("_Cancel", Gtk::RESPONSE_CANCEL);
  dialog->add_button("_Select", Gtk::RESPONSE_OK);

  return dialog;
}

std::shared_ptr<Gtk::FileChooserDialog>
CustomWidgetsFactory::create_folder_choose_dialog(Gtk::Window* parentWindow)
{
  auto dialog = std::make_shared<Gtk::FileChooserDialog>(
      *parentWindow, "Select directory containing source images",
      Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);

  dialog->add_button("_Cancel", Gtk::RESPONSE_CANCEL);
  dialog->add_button("_Select", Gtk::RESPONSE_OK);

  return dialog;
}

std::shared_ptr<Gtk::FileChooserDialog>
CustomWidgetsFactory::create_txt_export_folder_choose_dialog(
    Gtk::Window* parentWindow)
{
  auto dialog = std::make_shared<Gtk::FileChooserDialog>(
      *parentWindow, "Select plain txt files export directory",
      Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);

  dialog->add_button("_Cancel", Gtk::RESPONSE_CANCEL);
  dialog->add_button("_Select", Gtk::RESPONSE_OK);

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

    about->set_logo(pixbuf);
  }
  catch (const Glib::Error& ex) {
    LOGE("Failed to load icon: " << ex.what());
  }
}

std::shared_ptr<Gtk::MessageDialog> CustomWidgetsFactory::create_error_dialog(
    const std::string& errDesc, Gtk::Window* parentWindow)
{
  auto dialog = std::make_shared<Gtk::MessageDialog>(
      *parentWindow, "Error!", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK,
      true  // modal
  );

  dialog->set_secondary_text(errDesc);

  return dialog;
}

}  // namespace templateGtkmm3::window::custom_widgets
