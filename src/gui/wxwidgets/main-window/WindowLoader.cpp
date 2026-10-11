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

#include "src/gui/wxwidgets/main-window/WindowLoader.h"

#include <cassert>

#include "project-global-decls.h"
#include "src/gui/wxwidgets/MainWindowContext.h"
#include "src/gui/wxwidgets/main-window/custom-widgets/CustomWidgetsFactory.h"
#include "src/log/log.h"

namespace imagesAnnotatorWx::window
{

namespace
{
constexpr int kBorder = 3;
}  // namespace

bool WindowLoader::load_window(std::shared_ptr<MainWindowContext> nmwctx)
{
  assert(nmwctx != nullptr);

  if (nmwctx == nullptr) {
    LOGE("No context was given");
    return false;
  }

  mwctx = nmwctx;

  frame = new wxFrame(nullptr, wxID_ANY, "ImagesAnnotator", wxDefaultPosition,
                      wxSize(1200, 800));

  frame->SetMenuBar(build_menu_bar());
  statusBar = frame->CreateStatusBar(1);
  set_window_icon();
  update_window_title();

  auto* splitMain = new wxSplitterWindow(frame, wxID_ANY, wxDefaultPosition,
                                         wxDefaultSize, wxSP_LIVE_UPDATE);
  auto* leftPanel = build_left_panel(splitMain);

  auto* splitCR = new wxSplitterWindow(splitMain, wxID_ANY, wxDefaultPosition,
                                       wxDefaultSize, wxSP_LIVE_UPDATE);
  auto* centerPanel = build_center_panel(splitCR);
  auto* rightPanel = build_right_panel(splitCR);

  splitCR->SetMinimumPaneSize(140);
  splitCR->SplitVertically(centerPanel, rightPanel, -320);

  splitMain->SetMinimumPaneSize(140);
  splitMain->SplitVertically(leftPanel, splitCR, 260);

  auto* frameSizer = new wxBoxSizer(wxVERTICAL);
  frameSizer->Add(splitMain, 1, wxEXPAND);
  frame->SetSizer(frameSizer);

  frame->Maximize();

  return all_widget_are_valid();
}

wxMenuBar* WindowLoader::build_menu_bar()
{
  auto* project = new wxMenu();
  project->Append(WindowDataContext::ID_ANNOTATIONS_DB_OPEN,
                  "Open annotations DB\tCtrl+O");
  project->Append(WindowDataContext::ID_ANNOTATIONS_DB_SAVE, "Save\tCtrl+S");
  project->Append(WindowDataContext::ID_ANNOTATIONS_DB_SAVE_AS,
                  "Save As\tCtrl+Shift+S");
  project->AppendSeparator();
  project->Append(WindowDataContext::ID_EXPORT_TXT,
                  "Export plain txt to folder");
  // the darknetxx layouts, the newest format first, above the YOLO4 one they
  // grew out of
  auto* darknetxx = new wxMenu();
  darknetxx->Append(WindowDataContext::ID_EXPORT_DXXWJZ3P, "dxxwjz3p project");
  darknetxx->Append(WindowDataContext::ID_EXPORT_DXXWJZ2P, "dxxwjz2p project");
  darknetxx->Append(WindowDataContext::ID_EXPORT_DXXWJZ3,
                    "dxxwjz3 with YOLO4 folder");
  darknetxx->Append(WindowDataContext::ID_EXPORT_DXXWJZ2,
                    "dxxwjz2 with YOLO4 folder");
  project->AppendSubMenu(darknetxx, "Export DarknetXX to folder");
  project->Append(WindowDataContext::ID_EXPORT_YOLO4, "Export YOLO4 to folder");
  project->Append(WindowDataContext::ID_EXPORT_ULTRALYTICS_DETECT,
                  "Export Ultralytics YOLO detection to folder");
  project->Append(WindowDataContext::ID_EXPORT_ULTRALYTICS_OBB,
                  "Export Ultralytics YOLO oriented boxes to folder");
  project->Append(WindowDataContext::ID_EXPORT_ULTRALYTICS_SEGMENT,
                  "Export Ultralytics YOLO segmentation to folder");
  project->Append(WindowDataContext::ID_EXPORT_COCO,
                  "Export COCO JSON to folder");
  project->Append(WindowDataContext::ID_EXPORT_PASCAL_VOC,
                  "Export Pascal VOC XML to folder");
  project->Append(WindowDataContext::ID_EXPORT_CREATEML,
                  "Export Create ML JSON to folder");
  project->Append(WindowDataContext::ID_EXPORT_PYTORCH,
                  "Export PyTorch Vision to folder");
  project->AppendSeparator();
  project->Append(WindowDataContext::ID_IMPORT_TXT,
                  "Import plain txt from folder");
  // the darknetxx layouts, the newest format first, above the YOLO4 one they
  // grew out of; the two projects are picked as a file, the rest as a folder
  auto* darknetxxImport = new wxMenu();
  darknetxxImport->Append(WindowDataContext::ID_IMPORT_DXXWJZ3P,
                          "dxxwjz3p project");
  darknetxxImport->Append(WindowDataContext::ID_IMPORT_DXXWJZ2P,
                          "dxxwjz2p project");
  darknetxxImport->Append(WindowDataContext::ID_IMPORT_DXXWJZ3,
                          "dxxwjz3 with YOLO4 folder");
  darknetxxImport->Append(WindowDataContext::ID_IMPORT_DXXWJZ2,
                          "dxxwjz2 with YOLO4 folder");
  project->AppendSubMenu(darknetxxImport, "Import DarknetXX");
  project->Append(WindowDataContext::ID_IMPORT_YOLO4,
                  "Import YOLO4 from folder");
  project->Append(WindowDataContext::ID_IMPORT_ULTRALYTICS_DETECT,
                  "Import Ultralytics YOLO detection from folder");
  project->Append(WindowDataContext::ID_IMPORT_ULTRALYTICS_OBB,
                  "Import Ultralytics YOLO oriented boxes from folder");
  project->Append(WindowDataContext::ID_IMPORT_ULTRALYTICS_SEGMENT,
                  "Import Ultralytics YOLO segmentation from folder");
  project->Append(WindowDataContext::ID_IMPORT_COCO,
                  "Import COCO JSON from folder");
  project->Append(WindowDataContext::ID_IMPORT_PASCAL_VOC,
                  "Import Pascal VOC XML from folder");
  project->Append(WindowDataContext::ID_IMPORT_CREATEML,
                  "Import Create ML JSON from folder");
  project->Append(WindowDataContext::ID_IMPORT_PYTORCH,
                  "Import PyTorch Vision from folder");
  project->AppendSeparator();
  project->Append(WindowDataContext::ID_ANNOTATIONS_PROJECT_CLOSE,
                  "Close project\tCtrl+W");

  auto* annotations = new wxMenu();
  annotations->Append(WindowDataContext::ID_AUTO_INSERT_TO_WHOLE,
                      "Insert current annotation to whole image\tCtrl+Shift+F");

  auto* images = new wxMenu();
  images->Append(WindowDataContext::ID_IMAGES_OPEN,
                 "Open images folder\tCtrl+Shift+O");
  images->Append(WindowDataContext::ID_IMAGES_WEB_PAGE_OPEN,
                 "Open web page images\tCtrl+Shift+U");

  auto* help = new wxMenu();
  help->Append(WindowDataContext::ID_ABOUT, "About");

  auto* mb = new wxMenuBar();
  mb->Append(project, "&Project");
  mb->Append(annotations, "&Annotations");
  mb->Append(images, "&Images");
  mb->Append(help, "&Help");

  return mb;
}

wxWindow* WindowLoader::build_left_panel(wxWindow* parent)
{
  auto* panel = new wxPanel(parent);
  auto* sizer = new wxBoxSizer(wxVERTICAL);

  imagesFolderOpenB = new wxButton(panel, wxID_ANY, "Open images folder");
  imagesSearchE = new wxSearchCtrl(panel, wxID_ANY);
  imagesSearchE->ShowCancelButton(true);

  imagesList = new wxListCtrl(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                              wxLC_REPORT | wxLC_SINGLE_SEL | wxLC_NO_HEADER);
  imagesList->InsertColumn(0, "Images");
  // Keep the single column filling the list width as it resizes.
  imagesList->Bind(wxEVT_SIZE, [this](wxSizeEvent& e) {
    if (imagesList->GetColumnCount() > 0) {
      imagesList->SetColumnWidth(0, imagesList->GetClientSize().GetWidth());
    }
    e.Skip();
  });

  deleteImageB = new wxButton(panel, wxID_ANY, "Delete image");

  sizer->Add(imagesFolderOpenB, 0, wxEXPAND | wxALL, kBorder);
  sizer->Add(imagesSearchE, 0, wxEXPAND | wxALL, kBorder);
  sizer->Add(imagesList, 1, wxEXPAND | wxALL, kBorder);
  sizer->Add(deleteImageB, 0, wxEXPAND | wxALL, kBorder);

  panel->SetSizer(sizer);

  return panel;
}

wxWindow* WindowLoader::build_center_panel(wxWindow* parent)
{
  auto* panel = new wxPanel(parent);
  auto* sizer = new wxBoxSizer(wxVERTICAL);

  auto* toolbar = new wxBoxSizer(wxHORIZONTAL);
  zoomInB = new wxButton(panel, wxID_ANY, "Zoom in");
  zoomOutB = new wxButton(panel, wxID_ANY, "Zoom out");
  prevFileB = new wxButton(panel, wxID_ANY, "Previous");
  nextFileB = new wxButton(panel, wxID_ANY, "Next");
  toolbar->Add(zoomInB, 0, wxALL, kBorder);
  toolbar->Add(zoomOutB, 0, wxALL, kBorder);
  toolbar->Add(prevFileB, 0, wxALL, kBorder);
  toolbar->Add(nextFileB, 0, wxALL, kBorder);

  canvasHost = panel;
  mwctx->centralCanvas =
      mwctx->cwFactory->create_working_canvas(panel, mwctx->current_image);

  sizer->Add(toolbar, 0, wxEXPAND);
  sizer->Add(mwctx->centralCanvas, 1, wxEXPAND | wxALL, kBorder);

  panel->SetSizer(sizer);

  return panel;
}

wxWindow* WindowLoader::build_right_panel(wxWindow* parent)
{
  auto* panel = new wxPanel(parent);
  auto* sizer = new wxBoxSizer(wxVERTICAL);

  annotationsDbOpenB = new wxButton(panel, wxID_ANY, "Open annotations DB");
  annotationSearchE = new wxSearchCtrl(panel, wxID_ANY);
  annotationSearchE->ShowCancelButton(true);
  annotationsDbList = new wxListBox(panel, wxID_ANY);

  copyAnnotationNameB = new wxButton(panel, wxID_ANY, "Copy annotation name");
  duplicateAnnotationB = new wxButton(panel, wxID_ANY, "Copy annotation");
  rectEditEntry = new wxTextCtrl(panel, wxID_ANY);
  rectEditEntry->SetToolTip("Edit name for current image selected annotation");
  currentImageAnnotations = new wxListBox(panel, wxID_ANY);
  deleteAnnotationB = new wxButton(panel, wxID_ANY, "Delete annotation");

  sizer->Add(annotationsDbOpenB, 0, wxEXPAND | wxALL, kBorder);
  sizer->Add(new wxStaticText(panel, wxID_ANY, "All annotations list"), 0,
             wxALL, kBorder);
  sizer->Add(annotationSearchE, 0, wxEXPAND | wxALL, kBorder);
  sizer->Add(annotationsDbList, 1, wxEXPAND | wxALL, kBorder);
  sizer->Add(new wxStaticText(panel, wxID_ANY, "Current image annotations:"), 0,
             wxALL, kBorder);
  sizer->Add(copyAnnotationNameB, 0, wxEXPAND | wxALL, kBorder);
  sizer->Add(duplicateAnnotationB, 0, wxEXPAND | wxALL, kBorder);
  sizer->Add(rectEditEntry, 0, wxEXPAND | wxALL, kBorder);
  sizer->Add(currentImageAnnotations, 1, wxEXPAND | wxALL, kBorder);
  sizer->Add(deleteAnnotationB, 0, wxEXPAND | wxALL, kBorder);

  panel->SetSizer(sizer);

  return panel;
}

void WindowLoader::set_window_icon()
{
  const wxBitmap logo = custom_widgets::CustomWidgetsFactory::load_logo();

  if (!logo.IsOk()) {
    LOGE("Failed to set the default window icon");
    return;
  }

  wxIcon icon;
  icon.CopyFromBitmap(logo);
  frame->SetIcon(icon);
}

void WindowLoader::update_window_title(const std::string& openedFile)
{
  static const std::string basicTitle =
      project_decls::PROJECT_NAME + " " + project_decls::PROJECT_BUILD_VERSION;

  const std::string usedTitle =
      openedFile.empty() ? basicTitle : basicTitle + " [" + openedFile + "]";

  if (frame != nullptr) {
    frame->SetTitle(wxString::FromUTF8(usedTitle.c_str()));
  }
}

bool WindowLoader::all_widget_are_valid()
{
  const bool ok =
      frame != nullptr && imagesFolderOpenB != nullptr &&
      annotationsDbOpenB != nullptr && zoomInB != nullptr &&
      zoomOutB != nullptr && prevFileB != nullptr && nextFileB != nullptr &&
      deleteImageB != nullptr && deleteAnnotationB != nullptr &&
      copyAnnotationNameB != nullptr && duplicateAnnotationB != nullptr &&
      imagesSearchE != nullptr && annotationSearchE != nullptr &&
      imagesList != nullptr && annotationsDbList != nullptr &&
      currentImageAnnotations != nullptr && rectEditEntry != nullptr &&
      statusBar != nullptr &&
      (mwctx == nullptr || mwctx->centralCanvas != nullptr);

  if (!ok) {
    LOGE("Some of the window widgets were not created");
  }

  return ok;
}

wxFrame* WindowLoader::get_window() { return frame; }
wxButton* WindowLoader::get_images_folder_open_button()
{
  return imagesFolderOpenB;
}
wxButton* WindowLoader::get_annotations_db_open_button()
{
  return annotationsDbOpenB;
}
wxButton* WindowLoader::get_delete_current_image_selected_annotation()
{
  return deleteAnnotationB;
}
wxSearchCtrl* WindowLoader::get_annotation_search_entry()
{
  return annotationSearchE;
}
wxListBox* WindowLoader::get_annotations_db_list() { return annotationsDbList; }
wxListCtrl* WindowLoader::get_images_list() { return imagesList; }
wxButton* WindowLoader::get_current_image_zoom_in() { return zoomInB; }
wxButton* WindowLoader::get_current_image_zoom_out() { return zoomOutB; }
wxButton* WindowLoader::get_copy_annotation_name_button()
{
  return copyAnnotationNameB;
}
wxTextCtrl* WindowLoader::get_edit_current_rect_entry()
{
  return rectEditEntry;
}
wxButton* WindowLoader::get_prev_file_button() { return prevFileB; }
wxButton* WindowLoader::get_next_file_button() { return nextFileB; }
wxStatusBar* WindowLoader::get_window_status_bar() { return statusBar; }
wxButton* WindowLoader::get_copy_current_annotation_button()
{
  return duplicateAnnotationB;
}
wxSearchCtrl* WindowLoader::get_images_search_entry() { return imagesSearchE; }
wxButton* WindowLoader::get_delete_image_record() { return deleteImageB; }
wxListBox* WindowLoader::get_current_image_annotations()
{
  return currentImageAnnotations;
}

}  // namespace imagesAnnotatorWx::window
