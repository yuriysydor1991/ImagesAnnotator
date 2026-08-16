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

#ifndef IMAGES_ANNOTATOR_PROJECT_WX_CUSTOM_WIDGETS_CUSTOMWIDGETSFACTORY_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_WX_CUSTOM_WIDGETS_CUSTOMWIDGETSFACTORY_CLASS_H

#include <memory>
#include <string>
#include <vector>

#include "src/annotator-events/events/ImageRecord.h"
#include "src/annotator-events/events/ImageRecordRect.h"
#include "src/annotator-events/events/ImagesPathsDBProvider.h"
#include "src/gui/wxwidgets/main-window/custom-widgets/AllAnnotationsLabel.h"
#include "src/gui/wxwidgets/main-window/custom-widgets/CentralWorkingCanvas.h"
#include "src/gui/wxwidgets/main-window/custom-widgets/ImagePathLabel.h"
#include "src/gui/wxwidgets/main-window/custom-widgets/ImageRectsLabel.h"
#include "src/gui/wxwidgets/wx_includes.h"

namespace imagesAnnotatorWx::window::custom_widgets
{

/**
 * @brief Builds the wx row-data holders, the working canvas and the common
 * dialogs - the wxWidgets analogue of imagesAnnotatorGtkmm3's
 * CustomWidgetsFactory.
 *
 * The GTKmm factory returned configured Gtk::Dialog objects the callers then
 * run(); wxWidgets dialogs are short-lived and owned by their parent, so the
 * dialog helpers here run the dialog inline and hand back only the result. This
 * keeps every dialog definition in one place, exactly as the GTKmm factory did.
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

  // --- row-data holder / canvas factories (identical to the GTKmm factory) ---

  virtual ImagePathLabelPtr create_image_db_label(
      std::shared_ptr<ImageRecord> ir);
  virtual ImagesVisualDB create_images_visual_db(ImageRecordsSet& queue);

  virtual ImageRectsLabelPtr create_rect_label(ImageRecordRectPtr rect);
  virtual ImageRectsLabelSet create_rects_labels(ImageRecordRectSet& rectRecs);

  virtual AnnotationsVisualList create_annotations_labels(
      const AnnotationsList& strList);

  virtual CentralWorkingCanvas* create_working_canvas(
      wxWindow* parent, std::shared_ptr<ImagePathLabel>& nci);

  // --- dialogs (run inline, return the outcome) ---

  virtual bool ask_open_json_db(wxWindow* parent, std::string& out);
  virtual bool ask_save_json_db(wxWindow* parent, std::string& out);
  virtual bool ask_images_folder(wxWindow* parent, std::string& out);
  virtual bool ask_txt_export_folder(wxWindow* parent, std::string& out);
  virtual bool ask_yolo4_export_folder(wxWindow* parent, std::string& out);
  virtual bool ask_pytorch_export_folder(wxWindow* parent, std::string& out);
  virtual bool ask_ultralytics_detect_export_folder(wxWindow* parent,
                                                    std::string& out);
  virtual bool ask_ultralytics_obb_export_folder(wxWindow* parent,
                                                 std::string& out);
  virtual bool ask_ultralytics_segment_export_folder(wxWindow* parent,
                                                     std::string& out);
  virtual bool ask_coco_export_folder(wxWindow* parent, std::string& out);
  virtual bool ask_pascal_voc_export_folder(wxWindow* parent, std::string& out);
  virtual bool ask_createml_export_folder(wxWindow* parent, std::string& out);
  virtual bool ask_webpage_url(wxWindow* parent, std::string& out);

  virtual SaveAnswer ask_save_changes(wxWindow* parent);
  virtual LostRectsAnswer ask_lost_rects(wxWindow* parent);

  virtual void show_error(wxWindow* parent, const std::string& errDesc);
  virtual void show_about(wxWindow* parent);

  /// @brief The logo baked into the binary, used for the About dialog and the
  /// main window icon. Returns an invalid bitmap if the embedded asset fails to
  /// decode (never fatal).
  static wxBitmap load_logo();

 private:
  bool ask_folder(wxWindow* parent, const std::string& title, std::string& out);
};

}  // namespace imagesAnnotatorWx::window::custom_widgets

#endif  // IMAGES_ANNOTATOR_PROJECT_WX_CUSTOM_WIDGETS_CUSTOMWIDGETSFACTORY_CLASS_H
