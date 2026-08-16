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

#include "src/gui/wxwidgets/main-window/custom-widgets/CustomWidgetsFactory.h"

#include <cassert>

#include "project-global-decls.h"
#include "src/gui/wxwidgets/wx_str_utils.h"
#include "src/helpers/ImageLoader.h"
#include "src/log/log.h"
#include "wx_embedded_logo.h"

namespace imagesAnnotatorWx::window::custom_widgets
{

using imagesAnnotatorWx::from_wx;
using imagesAnnotatorWx::u8;

wxBitmap CustomWidgetsFactory::load_logo()
{
  wxMemoryInputStream stream(wx_embedded_logo_png, wx_embedded_logo_png_len);
  wxImage img(stream, wxBITMAP_TYPE_PNG);

  if (!img.IsOk()) {
    LOGE("Failed to decode the embedded application logo");
    return wxBitmap();
  }

  return wxBitmap(img);
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
    wxWindow* parent, std::shared_ptr<ImagePathLabel>& nci)
{
  return new CentralWorkingCanvas(parent, nci);
}

bool CustomWidgetsFactory::ask_open_json_db(wxWindow* parent, std::string& out)
{
  wxFileDialog dlg(parent, "Select annotations JSON db file", "", "",
                   "JSON files (*.json)|*.json|All files (*)|*",
                   wxFD_OPEN | wxFD_FILE_MUST_EXIST);

  if (dlg.ShowModal() != wxID_OK) {
    return false;
  }

  out = from_wx(dlg.GetPath());

  return !out.empty();
}

bool CustomWidgetsFactory::ask_save_json_db(wxWindow* parent, std::string& out)
{
  wxFileDialog dlg(parent, "Save annotations JSON db file", "", "",
                   "JSON files (*.json)|*.json|All files (*)|*",
                   wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

  if (dlg.ShowModal() != wxID_OK) {
    return false;
  }

  out = from_wx(dlg.GetPath());

  return !out.empty();
}

bool CustomWidgetsFactory::ask_folder(wxWindow* parent,
                                      const std::string& title,
                                      std::string& out)
{
  wxDirDialog dlg(parent, u8(title), "", wxDD_DEFAULT_STYLE);

  if (dlg.ShowModal() != wxID_OK) {
    return false;
  }

  out = from_wx(dlg.GetPath());

  return !out.empty();
}

bool CustomWidgetsFactory::ask_images_folder(wxWindow* parent, std::string& out)
{
  return ask_folder(parent, "Select directory containing source images", out);
}

bool CustomWidgetsFactory::ask_txt_export_folder(wxWindow* parent,
                                                 std::string& out)
{
  return ask_folder(parent, "Select plain txt files export directory", out);
}

bool CustomWidgetsFactory::ask_yolo4_export_folder(wxWindow* parent,
                                                   std::string& out)
{
  return ask_folder(parent, "Select YOLO4 export directory", out);
}

bool CustomWidgetsFactory::ask_ultralytics_detect_export_folder(
    wxWindow* parent, std::string& out)
{
  return ask_folder(parent,
                    "Select Ultralytics YOLO detection export directory", out);
}

bool CustomWidgetsFactory::ask_ultralytics_obb_export_folder(wxWindow* parent,
                                                             std::string& out)
{
  return ask_folder(
      parent, "Select Ultralytics YOLO oriented bounding box export directory",
      out);
}

bool CustomWidgetsFactory::ask_ultralytics_segment_export_folder(
    wxWindow* parent, std::string& out)
{
  return ask_folder(
      parent, "Select Ultralytics YOLO segmentation export directory", out);
}

bool CustomWidgetsFactory::ask_coco_export_folder(wxWindow* parent,
                                                  std::string& out)
{
  return ask_folder(parent, "Select COCO export directory", out);
}

bool CustomWidgetsFactory::ask_pascal_voc_export_folder(wxWindow* parent,
                                                        std::string& out)
{
  return ask_folder(parent, "Select Pascal VOC export directory", out);
}

bool CustomWidgetsFactory::ask_createml_export_folder(wxWindow* parent,
                                                      std::string& out)
{
  return ask_folder(parent, "Select Create ML export directory", out);
}

bool CustomWidgetsFactory::ask_pytorch_export_folder(wxWindow* parent,
                                                     std::string& out)
{
  return ask_folder(parent, "Select PyTorch Vision export directory", out);
}

bool CustomWidgetsFactory::ask_webpage_url(wxWindow* parent, std::string& out)
{
  const std::string cacheInfo =
      "All the network cache will be stored at " +
      helpers::ImageLoader::get_cache_top_level().string();

  wxTextEntryDialog dlg(parent,
                        "Enter the images web page URL.\n" + u8(cacheInfo),
                        "Enter the web page URL");

  if (dlg.ShowModal() != wxID_OK) {
    return false;
  }

  out = from_wx(dlg.GetValue());

  return !out.empty();
}

CustomWidgetsFactory::SaveAnswer CustomWidgetsFactory::ask_save_changes(
    wxWindow* parent)
{
  wxMessageDialog dlg(parent, "Do you want to save changes?", "Save changes",
                      wxYES_NO | wxCANCEL | wxICON_QUESTION);
  dlg.SetYesNoCancelLabels("Save", "Discard", "Cancel");

  switch (dlg.ShowModal()) {
    case wxID_YES:
      return SaveAnswer::Save;
    case wxID_NO:
      return SaveAnswer::Discard;
    default:
      return SaveAnswer::Cancel;
  }
}

CustomWidgetsFactory::LostRectsAnswer CustomWidgetsFactory::ask_lost_rects(
    wxWindow* parent)
{
  wxMessageDialog dlg(parent, "Images contains annotations. Delete them too?",
                      "Delete annotations",
                      wxYES_NO | wxCANCEL | wxICON_QUESTION);
  dlg.SetYesNoCancelLabels("Delete", "Leave", "Cancel");

  switch (dlg.ShowModal()) {
    case wxID_YES:
      return LostRectsAnswer::Delete;
    case wxID_NO:
      return LostRectsAnswer::Leave;
    default:
      return LostRectsAnswer::Cancel;
  }
}

void CustomWidgetsFactory::show_error(wxWindow* parent,
                                      const std::string& errDesc)
{
  wxMessageDialog dlg(parent, u8(errDesc), "Error!", wxOK | wxICON_ERROR);
  dlg.ShowModal();
}

void CustomWidgetsFactory::show_about(wxWindow* parent)
{
  wxAboutDialogInfo info;

  info.SetName(u8(project_decls::PROJECT_NAME));
  info.SetVersion(u8(project_decls::PROJECT_BUILD_VERSION));
  info.SetDescription(u8(project_decls::PROJECT_DESCRIPTION + "\ngit commit: " +
                         project_decls::PROJECT_BUILD_COMMIT));
  info.SetWebSite(u8(project_decls::PROJECT_HOMEPAGE_URL));
  info.AddDeveloper(u8(project_decls::PROJECT_MAINTAINER + " " +
                       project_decls::PROJECT_MAINTAINER_EMAIL));

  const wxBitmap logo = load_logo();
  if (logo.IsOk()) {
    wxIcon icon;
    icon.CopyFromBitmap(logo);
    info.SetIcon(icon);
  }

  wxAboutBox(info, parent);
}

}  // namespace imagesAnnotatorWx::window::custom_widgets
