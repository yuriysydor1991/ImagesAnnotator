/**
 * The "Simplified BSD License"
 *
 * Copyright (c) 2026, Yurii Sydor (yuriysydor1991@gmail.com)
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

#ifndef IMAGES_ANNOTATOR_PROJECT_QT6_CUSTOM_WIDGETS_CUSTOMWIDGETSFACTORY_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_QT6_CUSTOM_WIDGETS_CUSTOMWIDGETSFACTORY_CLASS_H

#include <memory>
#include <string>
#include <vector>

#include "src/annotator-events/events/ImageRecord.h"
#include "src/annotator-events/events/ImageRecordRect.h"
#include "src/annotator-events/events/ImagesPathsDBProvider.h"
#include "src/gui/qt6/main-window/custom-widgets/AllAnnotationsLabel.h"
#include "src/gui/qt6/main-window/custom-widgets/CentralWorkingCanvas.h"
#include "src/gui/qt6/main-window/custom-widgets/ImagePathLabel.h"
#include "src/gui/qt6/main-window/custom-widgets/ImageRectsLabel.h"
#include "src/gui/qt6/qt_includes.h"

namespace imagesAnnotatorQt6::window::custom_widgets
{

/**
 * @brief Builds the Qt row-data holders, the working canvas and the common
 * dialogs - the Qt analogue of imagesAnnotatorGtkmm3's CustomWidgetsFactory.
 *
 * The GTKmm factory returned configured Gtk::Dialog objects the callers then
 * run(); the Qt dialogs used here are the modal convenience ones (QFileDialog /
 * QMessageBox / QInputDialog), so the helpers run the dialog inline and hand
 * back only the result. This keeps every dialog definition in one place,
 * exactly as the GTKmm factory did.
 */
class CustomWidgetsFactory
{
 public:
  using ImageRecord = events::events::ImageRecord;
  using ImageRecordsSet = events::events::ImageRecordsSet;
  using ImagesVisualDB = std::vector<ImagePathLabelPtr>;
  using ImageRecordRectPtr = events::events::ImageRecordRectPtr;
  using ImageRecordRectSet = events::events::ImageRecordRectSet;
  using AnnotationsList =
      events::events::ImagesPathsDBProvider::AnnotationsList;
  using AnnotationsVisualList = std::vector<AllAnnotationsLabelPtr>;

  /// @brief User's answer to the "save unsaved changes?" prompt.
  enum class SaveAnswer
  {
    Cancel,
    Discard,
    Save
  };

  /// @brief User's answer to the "image has annotations, delete them too?"
  /// prompt.
  enum class LostRectsAnswer
  {
    Cancel,
    Leave,
    Delete
  };

  virtual ~CustomWidgetsFactory() = default;
  CustomWidgetsFactory() = default;

  /**
   * @brief Give a button the themed freedesktop icon named @a icon_name next to
   * its existing text label, reproducing the classic stock-button look (the Qt
   * analogue of the GTKmm factory's identically named helper, which the GTKmm3
   * window loader uses to decorate the same buttons). No-op when the name is
   * empty or the current icon theme has no such icon. Static so both this
   * factory and the window loader can share it.
   */
  static void add_button_icon(QPushButton& button, const QString& icon_name);

  // --- row-data holder / canvas factories (identical to the GTKmm factory) ---

  virtual ImagePathLabelPtr create_image_db_label(
      std::shared_ptr<ImageRecord> ir);
  virtual ImagesVisualDB create_images_visual_db(ImageRecordsSet& queue);

  virtual ImageRectsLabelPtr create_rect_label(ImageRecordRectPtr rect);
  virtual ImageRectsLabelSet create_rects_labels(ImageRecordRectSet& rectRecs);

  virtual AnnotationsVisualList create_annotations_labels(
      const AnnotationsList& strList);

  virtual CentralWorkingCanvas* create_working_canvas(
      QWidget* parent, std::shared_ptr<ImagePathLabel>& nci);

  // --- dialogs (run inline, return the outcome) ---

  virtual bool ask_open_json_db(QWidget* parent, std::string& out);
  virtual bool ask_save_json_db(QWidget* parent, std::string& out);
  virtual bool ask_images_folder(QWidget* parent, std::string& out);
  virtual bool ask_txt_export_folder(QWidget* parent, std::string& out);
  virtual bool ask_yolo4_export_folder(QWidget* parent, std::string& out);
  virtual bool ask_pytorch_export_folder(QWidget* parent, std::string& out);
  virtual bool ask_ultralytics_detect_export_folder(QWidget* parent,
                                                    std::string& out);
  virtual bool ask_ultralytics_obb_export_folder(QWidget* parent,
                                                 std::string& out);
  virtual bool ask_ultralytics_segment_export_folder(QWidget* parent,
                                                     std::string& out);
  virtual bool ask_coco_export_folder(QWidget* parent, std::string& out);
  virtual bool ask_pascal_voc_export_folder(QWidget* parent, std::string& out);
  virtual bool ask_createml_export_folder(QWidget* parent, std::string& out);
  virtual bool ask_webpage_url(QWidget* parent, std::string& out);

  virtual SaveAnswer ask_save_changes(QWidget* parent);
  virtual LostRectsAnswer ask_lost_rects(QWidget* parent);

  virtual void show_error(QWidget* parent, const std::string& errDesc);
  virtual void show_about(QWidget* parent);

  /// @brief The logo embedded in the binary through the Qt resource
  /// src/gui/qt6/ui/template.qrc, used for the About dialog and the main window
  /// icon. Returns a null pixmap if the embedded asset fails to decode (never
  /// fatal).
  static QPixmap load_logo();

 private:
  bool ask_folder(QWidget* parent, const std::string& title, std::string& out);
};

}  // namespace imagesAnnotatorQt6::window::custom_widgets

#endif  // IMAGES_ANNOTATOR_PROJECT_QT6_CUSTOM_WIDGETS_CUSTOMWIDGETSFACTORY_CLASS_H
