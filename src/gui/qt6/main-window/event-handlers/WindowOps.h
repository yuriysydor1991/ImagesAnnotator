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

#ifndef IMAGES_ANNOTATOR_PROJECT_QT6_EVENTHANDLERS_WINDOWOPS_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_QT6_EVENTHANDLERS_WINDOWOPS_CLASS_H

#include <cstddef>
#include <functional>
#include <memory>
#include <string>

#include "src/gui/qt6/ComponentTypesAliases.h"
#include "src/gui/qt6/qt_includes.h"
#include "src/helpers/TypeHelper.h"

namespace templateQt6
{
class MainWindowContext;
}  // namespace templateQt6

namespace templateQt6::window::event_handlers
{

/**
 * @brief Shared infrastructure object for the decomposed window event handlers
 * - the Qt analogue of templateGtkmm3's WindowOps. A single instance is created
 * by the WindowEventsHandler and injected into every concrete handler.
 *
 * The view/model operations mirror the GTKmm WindowOps one to one; the only
 * substantive change is the list handling: GTKmm kept every row in the
 * Gtk::ListBox and toggled per-row visibility for the search filters, while the
 * Qt lists (QListWidgets) are rebuilt from a filtered "displayed" subset. So
 * "the visible rows" become displayedImages / displayedAnnotations here, the
 * same way the wxWidgets port models them.
 */
class WindowOps : virtual public ComponentTypesAliases,
                  virtual public ::helpers::TypeHelper
{
 public:
  virtual ~WindowOps() = default;
  explicit WindowOps(std::shared_ptr<MainWindowContext> nmwctx);

  /// @brief The value QListWidget::currentRow() reports when nothing is
  /// selected (the Qt analogue of wxNOT_FOUND / a null Gtk::ListBoxRow).
  inline static constexpr const int no_selection = -1;

  void update_images_list();
  void update_action_buttons_sensitivity();
  void update_current_rects_list();
  void update_rect_edit_entry();
  void update_annotations_list();
  void update_current_annotations_selection();
  void update_status_bar(const std::string& nstatus);
  void update_title(const std::string& ntitle);
  void update_statuses();
  void update_image_zoom();

  std::string compute_title(const bool changes);

  void show_spinner();
  void hide_spinner();
  void show_error_dialog(const std::string& emsg);

  /// @brief Asks user about unsaved changes. @return true if the project may be
  /// closed.
  bool ask_about_unsaved_changes();

  /// @brief Asks about annotations being deleted with the image. @return true
  /// if the image should be deleted together with its annotations.
  bool ask_about_rects_delete();

  void save_project();
  void save_project_as();

  void apply_annotations_filter();
  void apply_images_filter();
  void select_all_annotations_name(const std::string& name);
  size_t get_annotated_images_count();

  /// @brief Clears every list widget and its backing model (project close).
  void clear_all_lists();

  // --- Qt list <-> data helpers ---
  int images_selected_index();
  void select_image_row(int row);
  ImagePathLabelPtr images_row_label(int row);

  /// @brief Repaint the current image's row background to match its
  /// has-annotations state, without rebuilding the whole list (the Qt analogue
  /// of toggling the image_row_has_records CSS class on the row).
  void update_current_image_row_marking();

  std::function<bool(const ImagePathLabelPtr& ptr)> get_annotated_images_op();

  std::shared_ptr<MainWindowContext> mwctx;

  /// @brief The image / annotation rows currently shown (== the search-filtered
  /// subset of mwctx->imagesVDB / mwctx->annotationsList).
  ImagesVisualDB displayedImages;
  AnnotationsVisualList displayedAnnotations;

  bool dragging{false};
  bool isOverResize{false};
  bool lastChangedStatus{false};
  bool spinnerOn{false};

  inline static const std::string changesI = "*";

 private:
  void rebuild_images_view();
  void rebuild_annotations_view();
  void rebuild_current_rects_view();

  /// @brief Paint (or reset) one images-list row to the annotated colouring.
  void mark_images_row(QListWidgetItem* item, const bool annotated);
};

}  // namespace templateQt6::window::event_handlers

#endif  // IMAGES_ANNOTATOR_PROJECT_QT6_EVENTHANDLERS_WINDOWOPS_CLASS_H
