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
#include "src/gtkmm3/main-window/custom-widgets/AllAnnotationsLabel.h"
#include "src/gtkmm3/main-window/custom-widgets/CentralWorkingCanvas.h"
#include "src/gtkmm3/main-window/custom-widgets/ImagePathLabel.h"
#include "src/gtkmm3/main-window/custom-widgets/ImageRectsLabel.h"

namespace templateGtkmm3::window::custom_widgets
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
  CustomWidgetsFactory() = default;

  virtual std::shared_ptr<ImagePathLabel> create_image_db_label(
      std::shared_ptr<ImageRecord> ir);

  virtual std::shared_ptr<CentralWorkingCanvas> create_working_canvas(
      std::shared_ptr<ImagePathLabel>& nci);

  virtual ImagesVisualDB create_images_visual_db(ImageRecordsSet& queue);

  virtual std::shared_ptr<Gtk::FileChooserDialog> create_json_db_dialog(
      Gtk::Window* parentWindow);
  virtual std::shared_ptr<Gtk::FileChooserDialog> create_folder_choose_dialog(
      Gtk::Window* parentWindow);
  virtual std::shared_ptr<Gtk::FileChooserDialog> create_save_json_db_dialog(
      Gtk::Window* parentWindow);

  virtual ImageRectsLabelPtr create_rect_label(ImageRecordRectPtr rect);
  virtual ImageRectsLabelSet create_rects_labels(ImageRecordRectSet& rectRecs);

  virtual AnnotationsVisualList create_annotations_labels(
      const AnnotationsList& strList);

  virtual void prepare_about(Gtk::AboutDialog* about,
                             Gtk::Window* parentWindow);

  virtual std::shared_ptr<Gtk::FileChooserDialog>
  create_txt_export_folder_choose_dialog(Gtk::Window* parentWindow);

  virtual std::shared_ptr<Gtk::MessageDialog> create_error_dialog(
      const std::string& errDesc, Gtk::Window* parentWindow);

  virtual std::shared_ptr<Gtk::MessageDialog> create_save_changes_ask_user(
      Gtk::Window* parentWindow);

  virtual std::shared_ptr<Gtk::FileChooserDialog>
  create_yolo4_export_folder_choose_dialog(Gtk::Window* parentWindow);

  virtual std::shared_ptr<Gtk::FileChooserDialog>
  create_pytorch_export_folder_choose_dialog(Gtk::Window* parentWindow);

 private:
  std::shared_ptr<Gtk::FileChooserDialog>
  create_folder_choose_dialog_with_title(Gtk::Window* parentWindow,
                                         const std::string& title);
};

}  // namespace templateGtkmm3::window::custom_widgets

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM_WINDOWLOADER_CUSTOMWIDGETS_FACTORY_CLASS_H
