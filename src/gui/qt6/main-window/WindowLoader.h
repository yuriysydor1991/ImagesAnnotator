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

#ifndef IMAGES_ANNOTATOR_PROJECT_QT6_WINDOWLOADER_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_QT6_WINDOWLOADER_CLASS_H

#include <memory>
#include <string>

#include "src/gui/qt6/ComponentTypesAliases.h"
#include "src/gui/qt6/main-window/WindowDataContext.h"
#include "src/gui/qt6/qt_includes.h"

namespace imagesAnnotatorQt6
{
class MainWindowContext;
}

namespace imagesAnnotatorQt6::window
{

/**
 * @brief Builds the main window widget tree in code - the Qt analogue of
 * imagesAnnotatorGtkmm3's WindowLoader, which instead inflated a Glade .ui
 * file.
 *
 * Every control is created here, given the WindowDataContext object name its
 * GTKmm counterpart carries as a builder id, and exposed through a typed getter
 * so the event handlers can connect to and drive them, exactly as the GTKmm
 * getters did. all_widget_are_valid() then resolves every id through
 * QObject::findChild(), which is the direct analogue of the GTKmm loader
 * resolving them through the builder.
 *
 * The QMainWindow is a top-level widget with no parent, so it has no Qt owner
 * and this class owns it through a unique_ptr; every other widget is owned by
 * the window through Qt's parent-child ownership.
 */
class WindowLoader : virtual public ComponentTypesAliases
{
 public:
  virtual ~WindowLoader() = default;
  WindowLoader() = default;

  virtual bool load_window(std::shared_ptr<MainWindowContext> nmwctx);

  virtual bool all_widget_are_valid();

  virtual QMainWindow* get_window();
  virtual QPushButton* get_images_folder_open_button();
  virtual QPushButton* get_annotations_db_open_button();
  virtual QPushButton* get_delete_current_image_selected_annotation();
  virtual QLineEdit* get_annotation_search_entry();
  virtual QListWidget* get_annotations_db_list();
  virtual QListWidget* get_images_list();
  virtual QPushButton* get_current_image_zoom_in();
  virtual QPushButton* get_current_image_zoom_out();
  virtual QPushButton* get_copy_annotation_name_button();
  virtual QLineEdit* get_edit_current_rect_entry();
  virtual QPushButton* get_prev_file_button();
  virtual QPushButton* get_next_file_button();
  virtual QStatusBar* get_window_status_bar();
  virtual QPushButton* get_copy_current_annotation_button();
  virtual QScrollArea* get_central_scrolled_window();
  virtual QLineEdit* get_images_search_entry();
  virtual QPushButton* get_delete_image_record();
  virtual QListWidget* get_current_image_annotations();

  /// @brief menus

  virtual QAction* get_images_open_mi();
  virtual QAction* get_annotations_db_open_mi();
  virtual QAction* get_annotations_db_save_mi();
  virtual QAction* get_annotations_db_saveas_mi();
  virtual QAction* get_annotations_project_close_mi();
  virtual QAction* get_about_mi();
  virtual QAction* get_export_txt2_folder_mi();
  virtual QAction* get_export_yolo4_folder_mi();
  virtual QAction* get_export_pytorchvision_folder_mi();
  virtual QAction* get_images_web_page_open_menu_item_mi();
  virtual QAction* auto_insert_current_annotation_to_whole_mi();

  virtual void update_window_title(const std::string& openedFile = "");

 protected:
  virtual bool propagate_params();
  virtual bool propagate_css_params();
  virtual void set_window_icon();
  virtual void decorate_button_icons();
  virtual bool propagate_menu_shortcuts();

  virtual void build_menu_bar();
  virtual QWidget* build_left_panel(QWidget* parent);
  virtual QWidget* build_center_panel(QWidget* parent);
  virtual QWidget* build_right_panel(QWidget* parent);

 private:
  /// @brief The Qt counterpart of the GTKmm loader's builder lookup: resolves
  /// the widget/action carrying @a id as its object name below the main window.
  template <class ObjectType>
  ObjectType* get_object(const std::string& id);

  /// @brief Create a menu action, name it after @a id and attach @a shortcut.
  /// Actions are parented to the window so findChild() reaches them.
  QAction* add_menu_action(QMenu* menu, const QString& text,
                           const std::string& id,
                           const QKeySequence& shortcut = QKeySequence{});

  inline static constexpr const int default_window_width = 1200;
  inline static constexpr const int default_window_height = 800;
  inline static constexpr const int left_pane_width = 260;
  inline static constexpr const int right_pane_width = 320;
  inline static constexpr const int min_pane_width = 140;

  std::shared_ptr<MainWindowContext> mwctx;

  std::unique_ptr<QMainWindow> window;
};

}  // namespace imagesAnnotatorQt6::window

#endif  // IMAGES_ANNOTATOR_PROJECT_QT6_WINDOWLOADER_CLASS_H
