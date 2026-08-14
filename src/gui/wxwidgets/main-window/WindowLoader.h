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

#ifndef IMAGES_ANNOTATOR_PROJECT_WX_WINDOWLOADER_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_WX_WINDOWLOADER_CLASS_H

#include <memory>
#include <string>

#include "src/gui/wxwidgets/ComponentTypesAliases.h"
#include "src/gui/wxwidgets/main-window/WindowDataContext.h"
#include "src/gui/wxwidgets/wx_includes.h"

namespace imagesAnnotatorWx
{
class MainWindowContext;
}

namespace imagesAnnotatorWx::window
{

/**
 * @brief Builds the main window widget tree in code - the wxWidgets analogue of
 * imagesAnnotatorGtkmm3's WindowLoader, which instead inflated a Glade .ui
 * file. Every control is created here and exposed through a typed getter so the
 * event handlers can bind to and drive them, exactly as the GTKmm getters did.
 */
class WindowLoader : virtual public ComponentTypesAliases
{
 public:
  virtual ~WindowLoader() = default;
  WindowLoader() = default;

  virtual bool load_window(std::shared_ptr<MainWindowContext> nmwctx);

  virtual bool all_widget_are_valid();

  virtual wxFrame* get_window();
  virtual wxButton* get_images_folder_open_button();
  virtual wxButton* get_annotations_db_open_button();
  virtual wxButton* get_delete_current_image_selected_annotation();
  virtual wxSearchCtrl* get_annotation_search_entry();
  virtual wxListBox* get_annotations_db_list();
  virtual wxListCtrl* get_images_list();
  virtual wxButton* get_current_image_zoom_in();
  virtual wxButton* get_current_image_zoom_out();
  virtual wxButton* get_copy_annotation_name_button();
  virtual wxTextCtrl* get_edit_current_rect_entry();
  virtual wxButton* get_prev_file_button();
  virtual wxButton* get_next_file_button();
  virtual wxStatusBar* get_window_status_bar();
  virtual wxButton* get_copy_current_annotation_button();
  virtual wxSearchCtrl* get_images_search_entry();
  virtual wxButton* get_delete_image_record();
  virtual wxListBox* get_current_image_annotations();

  virtual void update_window_title(const std::string& openedFile = "");

 protected:
  virtual wxMenuBar* build_menu_bar();
  virtual wxWindow* build_left_panel(wxWindow* parent);
  virtual wxWindow* build_center_panel(wxWindow* parent);
  virtual wxWindow* build_right_panel(wxWindow* parent);
  virtual void set_window_icon();

 private:
  std::shared_ptr<MainWindowContext> mwctx;

  wxFrame* frame{nullptr};

  wxButton* imagesFolderOpenB{nullptr};
  wxButton* annotationsDbOpenB{nullptr};
  wxButton* zoomInB{nullptr};
  wxButton* zoomOutB{nullptr};
  wxButton* prevFileB{nullptr};
  wxButton* nextFileB{nullptr};
  wxButton* deleteImageB{nullptr};
  wxButton* deleteAnnotationB{nullptr};
  wxButton* copyAnnotationNameB{nullptr};
  wxButton* duplicateAnnotationB{nullptr};

  wxSearchCtrl* imagesSearchE{nullptr};
  wxSearchCtrl* annotationSearchE{nullptr};

  wxListCtrl* imagesList{nullptr};
  wxListBox* annotationsDbList{nullptr};
  wxListBox* currentImageAnnotations{nullptr};

  wxTextCtrl* rectEditEntry{nullptr};
  wxStatusBar* statusBar{nullptr};

  wxWindow* canvasHost{nullptr};
};

}  // namespace imagesAnnotatorWx::window

#endif  // IMAGES_ANNOTATOR_PROJECT_WX_WINDOWLOADER_CLASS_H
