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

#ifndef IMAGES_ANNOTATOR_PROJECT_GTKMM_EVENTHANDLERS_WINDOWOPS_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_GTKMM_EVENTHANDLERS_WINDOWOPS_CLASS_H

#include <functional>
#include <memory>
#include <string>

#include "src/gui/gtkmm3/ComponentTypesAliases.h"
#include "src/gui/gtkmm3/gtkmm_includes.h"
#include "src/helpers/TypeHelper.h"

namespace templateGtkmm3
{
class MainWindowContext;
}  // namespace templateGtkmm3

namespace templateGtkmm3::window::event_handlers
{

/**
 * @brief Shared infrastructure object for the decomposed window event handlers.
 *
 * Holds the common state and the view/dialog/model operations that the
 * individual event handlers reuse. A single instance is created by the
 * WindowEventsHandler and injected into every concrete event handler through
 * its constructor.
 */
class WindowOps : virtual public ComponentTypesAliases,
                  virtual public ::helpers::TypeHelper
{
 public:
  virtual ~WindowOps() = default;
  explicit WindowOps(std::shared_ptr<MainWindowContext> nmwctx);

  void clean_list_box(Gtk::ListBox* listBox);
  void select_list_box_child(Gtk::ListBox* listBox, Gtk::Widget* child);

  void update_images_list();

  /// @brief Recomputes the sensitivity of the image navigation /
  /// view-manipulation and the annotation-management buttons from the current
  /// state, so each button is insensitive exactly when clicking it would be a
  /// no-op:
  ///   - Previous / Next: no visible image row before / after the selection
  ///     (visibility follows the images search filter);
  ///   - Zoom in / out: the current image is already at ImageRecord::max/minScale;
  ///   - Delete image: no images loaded;
  ///   - Copy annotation: no annotation selected on the current image;
  ///   - Copy annotation name: no annotation selected on the current image, or
  ///     no all-annotations-list selection to copy the name from;
  ///   - Delete annotation: no images loaded.
  /// Called from every place that changes the relevant state - update_images_list(),
  /// update_current_rects_list(), update_image_zoom(), the images-search and the
  /// current-annotation-selection handlers - and once at startup (via the initial
  /// update_images_list()) so the buttons start correctly disabled. The image /
  /// annotations DB "open" buttons are intentionally left untouched: they are the
  /// entry points that populate those lists.
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

  /// @brief Asks user about unsaved changes
  /// @return Returns true if project may be closed.
  bool ask_about_unsaved_changes();

  /// @brief Asks about annotations being deleted with the image
  /// @return Returns a true value if image should be deleted with
  /// the annotations
  bool ask_about_rects_delete();

  /// @brief Stores the current project, falling back to "save as" when no
  /// path is associated with the opened project yet.
  void save_project();
  /// @brief Asks the user for a destination and stores the current project.
  void save_project_as();

  /// @brief Filters the all annotations list box by the current search text.
  void apply_annotations_filter();

  void select_all_annotations_name(const std::string& name);
  size_t get_annotated_images_count();

  std::function<bool(const ImagePathLabelPtr& ptr)> get_annotated_images_op();
  std::function<bool(const Gtk::Widget* w)> get_list_row_name_searcher_op(
      const std::string& name);

  std::shared_ptr<MainWindowContext> mwctx;

  bool dragging{false};
  bool isOverResize{false};
  bool lastChangedStatus{false};

  inline static constexpr const char* const overlay_class =
      "almost_non_transparent";
  inline static const std::string changesI = "*";
};

}  // namespace templateGtkmm3::window::event_handlers

#endif  // IMAGES_ANNOTATOR_PROJECT_GTKMM_EVENTHANDLERS_WINDOWOPS_CLASS_H
