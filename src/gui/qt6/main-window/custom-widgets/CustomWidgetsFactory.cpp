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

#include "src/gui/qt6/main-window/custom-widgets/CustomWidgetsFactory.h"

#include <cassert>

#include "project-global-decls.h"
#include "src/gui/qt6/main-window/WindowDataContext.h"
#include "src/gui/qt6/qt_str_utils.h"
#include "src/helpers/ImageLoader.h"
#include "src/log/log.h"

namespace imagesAnnotatorQt6::window::custom_widgets
{

using imagesAnnotatorQt6::from_qt;
using imagesAnnotatorQt6::u8;

namespace
{
/// @brief The annotations project files filter, shared by the open and save
/// dialogs (the Qt analogue of the Gtk::FileFilter the GTKmm factory attaches).
const char* const jsonDbFilter = "JSON files (*.json);;All files (*)";
}  // namespace

void CustomWidgetsFactory::add_button_icon(QPushButton& button,
                                           const QString& icon_name)
{
  if (icon_name.isEmpty()) {
    return;
  }

  const QIcon icon = QIcon::fromTheme(icon_name);

  if (icon.isNull()) {
    LOGD("The current icon theme has no icon named "
         << from_qt(icon_name) << ", leaving the button text-only");
    return;
  }

  button.setIcon(icon);
}

QPixmap CustomWidgetsFactory::load_logo()
{
  const QPixmap logo{QString::fromUtf8(WindowDataContext::logo_res_path)};

  if (logo.isNull()) {
    LOGE("Failed to decode the embedded application logo");
  }

  return logo;
}

ImagePathLabelPtr CustomWidgetsFactory::create_image_db_label(
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

ImageRectsLabelPtr CustomWidgetsFactory::create_rect_label(
    ImageRecordRectPtr rect)
{
  auto rt = std::make_shared<ImageRectsLabel>();

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

CentralWorkingCanvas* CustomWidgetsFactory::create_working_canvas(
    QWidget* parent, std::shared_ptr<ImagePathLabel>& nci)
{
  return new CentralWorkingCanvas(parent, nci);
}

bool CustomWidgetsFactory::ask_open_json_db(QWidget* parent, std::string& out)
{
  const QString selected = QFileDialog::getOpenFileName(
      parent, QStringLiteral("Select annotations JSON db file"), QString{},
      QString::fromUtf8(jsonDbFilter));

  if (selected.isEmpty()) {
    return false;
  }

  out = from_qt(selected);

  return !out.empty();
}

bool CustomWidgetsFactory::ask_save_json_db(QWidget* parent, std::string& out)
{
  const QString selected = QFileDialog::getSaveFileName(
      parent, QStringLiteral("Save annotations JSON db file"), QString{},
      QString::fromUtf8(jsonDbFilter));

  if (selected.isEmpty()) {
    return false;
  }

  out = from_qt(selected);

  return !out.empty();
}

bool CustomWidgetsFactory::ask_folder(QWidget* parent, const std::string& title,
                                      std::string& out)
{
  const QString selected =
      QFileDialog::getExistingDirectory(parent, u8(title), QString{});

  if (selected.isEmpty()) {
    return false;
  }

  out = from_qt(selected);

  return !out.empty();
}

bool CustomWidgetsFactory::ask_images_folder(QWidget* parent, std::string& out)
{
  return ask_folder(parent, "Select directory containing source images", out);
}

bool CustomWidgetsFactory::ask_txt_export_folder(QWidget* parent,
                                                 std::string& out)
{
  return ask_folder(parent, "Select plain txt files export directory", out);
}

bool CustomWidgetsFactory::ask_yolo4_export_folder(QWidget* parent,
                                                   std::string& out)
{
  return ask_folder(parent, "Select YOLO4 export directory", out);
}

bool CustomWidgetsFactory::ask_ultralytics_detect_export_folder(
    QWidget* parent, std::string& out)
{
  return ask_folder(parent,
                    "Select Ultralytics YOLO detection export directory", out);
}

bool CustomWidgetsFactory::ask_ultralytics_obb_export_folder(QWidget* parent,
                                                             std::string& out)
{
  return ask_folder(
      parent, "Select Ultralytics YOLO oriented bounding box export directory",
      out);
}

bool CustomWidgetsFactory::ask_ultralytics_segment_export_folder(
    QWidget* parent, std::string& out)
{
  return ask_folder(
      parent, "Select Ultralytics YOLO segmentation export directory", out);
}

bool CustomWidgetsFactory::ask_coco_export_folder(QWidget* parent,
                                                  std::string& out)
{
  return ask_folder(parent, "Select COCO export directory", out);
}

bool CustomWidgetsFactory::ask_pascal_voc_export_folder(QWidget* parent,
                                                        std::string& out)
{
  return ask_folder(parent, "Select Pascal VOC export directory", out);
}

bool CustomWidgetsFactory::ask_pytorch_export_folder(QWidget* parent,
                                                     std::string& out)
{
  return ask_folder(parent, "Select PyTorch Vision export directory", out);
}

bool CustomWidgetsFactory::ask_webpage_url(QWidget* parent, std::string& out)
{
  const std::string cacheInfo =
      "All the network cache will be stored at " +
      helpers::ImageLoader::get_cache_top_level().string();

  bool accepted{false};

  const QString entered =
      QInputDialog::getText(parent, QStringLiteral("Enter the web page URL"),
                            u8("Enter the images web page URL.\n" + cacheInfo),
                            QLineEdit::Normal, QString{}, &accepted);

  if (!accepted) {
    return false;
  }

  out = from_qt(entered);

  return !out.empty();
}

CustomWidgetsFactory::SaveAnswer CustomWidgetsFactory::ask_save_changes(
    QWidget* parent)
{
  QMessageBox box{parent};

  box.setIcon(QMessageBox::Question);
  box.setWindowTitle(QStringLiteral("Save changes"));
  box.setText(QStringLiteral("Do you want to save changes?"));
  box.setStandardButtons(QMessageBox::Save | QMessageBox::Discard |
                         QMessageBox::Cancel);
  box.setDefaultButton(QMessageBox::Save);

  switch (box.exec()) {
    case QMessageBox::Save:
      return SaveAnswer::Save;
    case QMessageBox::Discard:
      return SaveAnswer::Discard;
    default:
      return SaveAnswer::Cancel;
  }
}

CustomWidgetsFactory::LostRectsAnswer CustomWidgetsFactory::ask_lost_rects(
    QWidget* parent)
{
  QMessageBox box{parent};

  box.setIcon(QMessageBox::Question);
  box.setWindowTitle(QStringLiteral("Delete annotations"));
  box.setText(QStringLiteral("Images contains annotations. Delete them too?"));

  // Qt has no standard button pair matching "Delete" / "Leave", so both are
  // added explicitly and identified by their returned pointers.
  auto* deleteButton =
      box.addButton(QStringLiteral("Delete"), QMessageBox::DestructiveRole);
  auto* leaveButton =
      box.addButton(QStringLiteral("Leave"), QMessageBox::RejectRole);
  box.addButton(QMessageBox::Cancel);
  box.setDefaultButton(QMessageBox::Cancel);

  box.exec();

  if (box.clickedButton() == deleteButton) {
    return LostRectsAnswer::Delete;
  }

  if (box.clickedButton() == leaveButton) {
    return LostRectsAnswer::Leave;
  }

  return LostRectsAnswer::Cancel;
}

void CustomWidgetsFactory::show_error(QWidget* parent,
                                      const std::string& errDesc)
{
  QMessageBox::critical(parent, QStringLiteral("Error!"), u8(errDesc));
}

void CustomWidgetsFactory::show_about(QWidget* parent)
{
  QMessageBox box{parent};

  box.setWindowTitle(u8("About " + project_decls::PROJECT_NAME));
  box.setText(u8(project_decls::PROJECT_NAME + " " +
                 project_decls::PROJECT_BUILD_VERSION));
  box.setInformativeText(
      u8(project_decls::PROJECT_DESCRIPTION + "\n\n" +
         "git commit: " + project_decls::PROJECT_BUILD_COMMIT + "\n" +
         "website: " + project_decls::PROJECT_HOMEPAGE_URL + "\n" +
         "maintainer: " + project_decls::PROJECT_MAINTAINER + " " +
         project_decls::PROJECT_MAINTAINER_EMAIL));
  box.setStandardButtons(QMessageBox::Close);

  const QPixmap logo = load_logo();

  if (!logo.isNull()) {
    box.setIconPixmap(logo);
  }

  box.exec();
}

}  // namespace imagesAnnotatorQt6::window::custom_widgets
