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

#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM_WINDOWLOADER_CUSTOMWIDGETS_FACTORY_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM_WINDOWLOADER_CUSTOMWIDGETS_FACTORY_CLASS_H

#include <memory>
#include <vector>

#include "src/annotator-events/events/ImageRecord.h"
#include "src/annotator-events/events/ImageRecordRect.h"
#include "src/annotator-events/events/ImagesPathsDBProvider.h"
#include "src/gui/gtkmm4/main-window/custom-widgets/AllAnnotationsLabel.h"
#include "src/gui/gtkmm4/main-window/custom-widgets/CentralWorkingCanvas.h"
#include "src/gui/gtkmm4/main-window/custom-widgets/ImagePathLabel.h"
#include "src/gui/gtkmm4/main-window/custom-widgets/ImageRectsLabel.h"

namespace imagesAnnotatorGtkmm4::window::custom_widgets
{

/**
 * @brief Class to hold the window loader routines from the UI xml files.
 */
class CustomWidgetsFactory
{
 public:
  using ImageRecord = events::events::ImageRecord;
  using ImageRecordsSet = events::events::ImageRecordsSet;
  using ImagesVisualDB = std::vector<std::shared_ptr<ImagePathLabel>>;
  using ImageRecordRectPtr = events::events::ImageRecordRectPtr;
  using ImageRecordRectSet = events::events::ImageRecordRectSet;
  using AnnotationsList =
      events::events::ImagesPathsDBProvider::AnnotationsList;
  using AnnotationsVisualList =
      std::vector<std::shared_ptr<AllAnnotationsLabel>>;

  virtual ~CustomWidgetsFactory() = default;

  /**
   * @brief Add a themed icon next to a button's existing (translated) text
   * label, reproducing the pre-GTK4 stock-button look. No-op when @a icon_name
   * is empty. Static so both the dialog factory and the main window loader can
   * share it.
   */
  static void add_button_icon(Gtk::Button& button,
                              const Glib::ustring& icon_name);

  /**
   * @brief Restore a reused (hide-on-close) file-chooser dialog to the clean
   * state a freshly constructed one would have: re-activate the default (first)
   * filter and, for SAVE choosers, clear the typed file name. Called before each
   * showing so the shared .ui instance does not carry a stale filter, selection
   * or - for Save-As - a stale overwrite target between invocations. Static so
   * WindowOps::run_dialog can apply it to every chooser it shows.
   */
  static void reset_file_chooser(Gtk::FileChooserDialog& dialog);
  CustomWidgetsFactory() = default;

  virtual std::shared_ptr<ImagePathLabel> create_image_db_label(
      std::shared_ptr<ImageRecord> ir);

  virtual std::shared_ptr<CentralWorkingCanvas> create_working_canvas(
      std::shared_ptr<ImagePathLabel>& nci);

  virtual ImagesVisualDB create_images_visual_db(ImageRecordsSet& queue);

  virtual ImageRectsLabelPtr create_rect_label(ImageRecordRectPtr rect);
  virtual ImageRectsLabelSet create_rects_labels(ImageRecordRectSet& rectRecs);

  virtual AnnotationsVisualList create_annotations_labels(
      const AnnotationsList& strList);

  virtual void prepare_about(Gtk::AboutDialog* about,
                             Gtk::Window* parentWindow);

  /**
   * @brief Decorate a builder-defined JSON file-chooser dialog (the open or the
   * save one) with the "JSON files" / "All files" filters and the localized,
   * icon-decorated Cancel / Select action buttons. GtkBuilder cannot express
   * either from the .ui file, so WindowLoader applies this once at load.
   * Idempotent - re-invoking it on an already prepared dialog is a no-op.
   */
  virtual void prepare_json_db_dialog(Gtk::FileChooserDialog* dialog);

  /**
   * @brief Decorate the builder-defined darknetxx dxxwjz3p project file
   * chooser of the import with the "darknetxx dxxwjz3p projects" / "All files"
   * filters and the localized, icon-decorated Cancel / Select action buttons.
   * Idempotent.
   */
  virtual void prepare_dxxwjz3p_import_file_choose_dialog(
      Gtk::FileChooserDialog* dialog);

  /**
   * @brief Decorate the builder-defined darknetxx dxxwjz2p project file
   * chooser of the import with the "darknetxx dxxwjz2p projects" / "All files"
   * filters and the localized, icon-decorated Cancel / Select action buttons.
   * Idempotent.
   */
  virtual void prepare_dxxwjz2p_import_file_choose_dialog(
      Gtk::FileChooserDialog* dialog);

  /**
   * @brief Decorate a builder-defined folder-choosing dialog with the localized,
   * icon-decorated Cancel / Select action buttons. Idempotent.
   */
  virtual void prepare_folder_choose_dialog(Gtk::FileChooserDialog* dialog);

  /**
   * @brief Add the localized, icon-decorated Cancel / Discard / Save buttons
   * (and the default response) to the builder-defined "save changes?" question
   * dialog. Idempotent.
   */
  virtual void prepare_save_changes_ask_user(Gtk::MessageDialog* dialog);

  /**
   * @brief Add the localized, icon-decorated Cancel / Leave / Delete buttons
   * (and the default response) to the builder-defined "delete annotations too?"
   * question dialog. Idempotent.
   */
  virtual void prepare_ask_user_about_lost_rects(Gtk::MessageDialog* dialog);

  virtual void prepare_url_asker(Gtk::Dialog* dialog,
                                 Gtk::Label* cachePathLabel,
                                 Gtk::Window* parentWindow);
};

}  // namespace imagesAnnotatorGtkmm4::window::custom_widgets

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM_WINDOWLOADER_CUSTOMWIDGETS_FACTORY_CLASS_H
