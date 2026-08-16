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

#include "src/gui/qt6/main-window/WindowLoader.h"

#include <cassert>

#include "project-global-decls.h"
#include "src/gui/qt6/MainWindowContext.h"
#include "src/gui/qt6/main-window/custom-widgets/CustomWidgetsFactory.h"
#include "src/gui/qt6/qt_str_utils.h"
#include "src/log/log.h"

namespace imagesAnnotatorQt6::window
{

bool WindowLoader::load_window(std::shared_ptr<MainWindowContext> nmwctx)
{
  assert(nmwctx != nullptr);

  if (nmwctx == nullptr) {
    LOGE("No context was given");
    return false;
  }

  mwctx = nmwctx;

  window = std::make_unique<QMainWindow>();
  window->setObjectName(QString::fromUtf8(WindowDataContext::window_id));
  window->resize(default_window_width, default_window_height);

  build_menu_bar();

  // QMainWindow::statusBar() creates the bar on first use, mirroring how the
  // Glade file declares one for the GTKmm window.
  window->statusBar()->setObjectName(
      QString::fromUtf8(WindowDataContext::main_window_status_bar_id));

  // A QSplitter takes any widget created with it as its parent into its own
  // layout, in creation order, so the panes below are added just by being built
  // with the right parent - the images list on the left, then the working
  // canvas and the annotations panel nested in a second splitter on the right.
  auto* splitMain = new QSplitter{Qt::Horizontal, window.get()};

  build_left_panel(splitMain);

  auto* splitCR = new QSplitter{Qt::Horizontal, splitMain};

  build_center_panel(splitCR);
  build_right_panel(splitCR);

  splitCR->setChildrenCollapsible(false);
  splitCR->setSizes({default_window_width - left_pane_width - right_pane_width,
                     right_pane_width});

  splitMain->setChildrenCollapsible(false);
  splitMain->setSizes(
      {left_pane_width, default_window_width - left_pane_width});

  window->setCentralWidget(splitMain);

  if (!all_widget_are_valid()) {
    LOGE("Not all widgets are present");
    return false;
  }

  if (!propagate_params()) {
    LOGE("Failure to propagate params");
    return false;
  }

  return true;
}

bool WindowLoader::propagate_params()
{
  assert(window != nullptr);

  if (window == nullptr) {
    LOGE("No window present");
    return false;
  }

  if (!all_widget_are_valid()) {
    LOGE("Not all widgets are present");
    return false;
  }

  window->showMaximized();
  update_window_title();
  set_window_icon();

  if (!propagate_css_params()) {
    LOGE("Fail to propagate the css into the app window");
    return false;
  }

  assert(mwctx->centralCanvas != nullptr);

  // The GTKmm loader centres the drawing area inside its viewport with
  // Gtk::ALIGN_CENTER; a QScrollArea centres its widget through its alignment.
  get_central_scrolled_window()->setAlignment(Qt::AlignCenter);

  if (!propagate_menu_shortcuts()) {
    LOGE("Failure during shortcuts propagation");
    return false;
  }

  decorate_button_icons();

  return true;
}

void WindowLoader::decorate_button_icons()
{
  using custom_widgets::CustomWidgetsFactory;

  // Give the main-window action buttons the same stock-style icon + label look
  // as the dialog buttons, using the very same themed (Adwaita) freedesktop
  // icon names the GTKmm3 loader uses.
  CustomWidgetsFactory::add_button_icon(*get_images_folder_open_button(),
                                        QStringLiteral("folder-open-symbolic"));
  CustomWidgetsFactory::add_button_icon(
      *get_annotations_db_open_button(),
      QStringLiteral("document-open-symbolic"));
  CustomWidgetsFactory::add_button_icon(*get_current_image_zoom_in(),
                                        QStringLiteral("zoom-in-symbolic"));
  CustomWidgetsFactory::add_button_icon(*get_current_image_zoom_out(),
                                        QStringLiteral("zoom-out-symbolic"));
  CustomWidgetsFactory::add_button_icon(*get_prev_file_button(),
                                        QStringLiteral("go-previous-symbolic"));
  CustomWidgetsFactory::add_button_icon(*get_next_file_button(),
                                        QStringLiteral("go-next-symbolic"));
  CustomWidgetsFactory::add_button_icon(*get_delete_image_record(),
                                        QStringLiteral("user-trash-symbolic"));
  CustomWidgetsFactory::add_button_icon(
      *get_delete_current_image_selected_annotation(),
      QStringLiteral("edit-delete-symbolic"));
  CustomWidgetsFactory::add_button_icon(*get_copy_annotation_name_button(),
                                        QStringLiteral("edit-copy-symbolic"));
  CustomWidgetsFactory::add_button_icon(*get_copy_current_annotation_button(),
                                        QStringLiteral("edit-copy-symbolic"));
}

bool WindowLoader::propagate_menu_shortcuts()
{
  // The accelerators are attached to the QActions while the menu bar is built
  // (see add_menu_action()), which is Qt's equivalent of the GTKmm loader
  // hanging a Gtk::AccelGroup off the window. Nothing is left to do here beyond
  // confirming every accelerated action really exists, so a renamed id fails
  // loudly instead of silently dropping a keyboard shortcut.
  const QAction* const accelerated[] = {
      get_annotations_db_open_mi(),
      get_annotations_db_save_mi(),
      get_annotations_db_saveas_mi(),
      get_annotations_project_close_mi(),
      get_images_open_mi(),
      get_images_web_page_open_menu_item_mi(),
      auto_insert_current_annotation_to_whole_mi()};

  for (const QAction* const action : accelerated) {
    if (action == nullptr) {
      LOGE("An accelerated menu action is missing");
      return false;
    }

    if (action->shortcut().isEmpty()) {
      LOGE("A menu action carries no keyboard shortcut");
      return false;
    }
  }

  return true;
}

void WindowLoader::update_window_title(const std::string& openedFile)
{
  static const std::string basicTitle =
      project_decls::PROJECT_NAME + " " + project_decls::PROJECT_BUILD_VERSION;

  const std::string usedTitle =
      openedFile.empty() ? basicTitle : basicTitle + " [" + openedFile + "]";

  if (window != nullptr) {
    window->setWindowTitle(u8(usedTitle));
  }
}

void WindowLoader::set_window_icon()
{
  // Attach the logo baked into the binary as the window icon. On X11 it feeds
  // _NET_WM_ICON, which the window list / taskbar / alt-tab switcher use to
  // label the window. A failure is logged and swallowed on purpose: a missing
  // icon must never keep the GUI from starting. (On Wayland the panel icon
  // comes from the .desktop file matched through the app id instead - see
  // Qt6Initer::run().)
  const QPixmap logo = custom_widgets::CustomWidgetsFactory::load_logo();

  if (logo.isNull()) {
    LOGE("Failed to set the default window icon");
    return;
  }

  window->setWindowIcon(QIcon{logo});
}

bool WindowLoader::propagate_css_params()
{
  assert(mwctx != nullptr);
  assert(mwctx->wctx != nullptr);

  QFile qss{QString::fromUtf8(WindowDataContext::main_window_qss)};

  if (!qss.open(QIODevice::ReadOnly | QIODevice::Text)) {
    LOGE("Failed to open the embedded style sheet "
         << WindowDataContext::main_window_qss);
    return false;
  }

  // Applied to the window rather than to the whole application: Qt cascades a
  // style sheet down the widget tree, so every child widget and every dialog
  // parented to the window picks it up, which is the reach the GTKmm
  // add_provider_for_screen() call has in practice.
  window->setStyleSheet(QString::fromUtf8(qss.readAll()));

  return true;
}

void WindowLoader::build_menu_bar()
{
  auto* menuBar = window->menuBar();

  auto* project = menuBar->addMenu(QStringLiteral("&Project"));
  add_menu_action(project, QStringLiteral("Open annotations DB"),
                  WindowDataContext::annotations_db_open_menu,
                  QKeySequence{Qt::CTRL | Qt::Key_O});
  add_menu_action(project, QStringLiteral("Save"),
                  WindowDataContext::annotations_db_save_menu_id,
                  QKeySequence{Qt::CTRL | Qt::Key_S});
  add_menu_action(project, QStringLiteral("Save As"),
                  WindowDataContext::annotations_db_save_as_menu_id,
                  QKeySequence{Qt::CTRL | Qt::SHIFT | Qt::Key_S});
  project->addSeparator();
  add_menu_action(project, QStringLiteral("Export plain txt to folder"),
                  WindowDataContext::annotations_export_txt_2_folder_id);
  add_menu_action(project, QStringLiteral("Export YOLO4 to folder"),
                  WindowDataContext::annotations_export_yolo4_2_folder2_id);
  add_menu_action(
      project, QStringLiteral("Export Ultralytics YOLO detection to folder"),
      WindowDataContext::annotations_export_ultralytics_detect_2_folder_id);
  add_menu_action(
      project,
      QStringLiteral("Export Ultralytics YOLO oriented boxes to folder"),
      WindowDataContext::annotations_export_ultralytics_obb_2_folder_id);
  add_menu_action(
      project, QStringLiteral("Export Ultralytics YOLO segmentation to folder"),
      WindowDataContext::annotations_export_ultralytics_segment_2_folder_id);
  add_menu_action(project, QStringLiteral("Export COCO JSON to folder"),
                  WindowDataContext::annotations_export_coco_2_folder_id);
  add_menu_action(project, QStringLiteral("Export Pascal VOC XML to folder"),
                  WindowDataContext::annotations_export_pascal_voc_2_folder_id);
  add_menu_action(project, QStringLiteral("Export Create ML JSON to folder"),
                  WindowDataContext::annotations_export_createml_2_folder_id);
  add_menu_action(
      project, QStringLiteral("Export PyTorch Vision to folder"),
      WindowDataContext::annotations_export_pytorchvision_2_folder_id);
  project->addSeparator();
  add_menu_action(project, QStringLiteral("Close project"),
                  WindowDataContext::annotations_project_close_id,
                  QKeySequence{Qt::CTRL | Qt::Key_W});

  auto* annotations = menuBar->addMenu(QStringLiteral("&Annotations"));
  add_menu_action(annotations,
                  QStringLiteral("Insert current annotation to whole image"),
                  WindowDataContext::auto_insert_current_annotation_to_whole_id,
                  QKeySequence{Qt::CTRL | Qt::SHIFT | Qt::Key_F});

  auto* images = menuBar->addMenu(QStringLiteral("&Images"));
  add_menu_action(images, QStringLiteral("Open images folder"),
                  WindowDataContext::images_open_menu_item_id,
                  QKeySequence{Qt::CTRL | Qt::SHIFT | Qt::Key_O});
  add_menu_action(images, QStringLiteral("Open web page images"),
                  WindowDataContext::images_web_page_open_menu_item_id,
                  QKeySequence{Qt::CTRL | Qt::SHIFT | Qt::Key_U});

  auto* help = menuBar->addMenu(QStringLiteral("&Help"));
  add_menu_action(help, QStringLiteral("About"),
                  WindowDataContext::about_menu_item_id);
}

QAction* WindowLoader::add_menu_action(QMenu* menu, const QString& text,
                                       const std::string& id,
                                       const QKeySequence& shortcut)
{
  assert(menu != nullptr);
  assert(window != nullptr);

  // Parented to the window (not to the menu) so findChild() from the window
  // reaches it the same way the GTKmm builder reaches a menu item by id, and so
  // the action outlives any menu rebuild.
  auto* action = new QAction{text, window.get()};

  action->setObjectName(QString::fromStdString(id));

  if (!shortcut.isEmpty()) {
    action->setShortcut(shortcut);
  }

  menu->addAction(action);

  return action;
}

QWidget* WindowLoader::build_left_panel(QWidget* parent)
{
  auto* panel = new QWidget{parent};
  auto* layout = new QVBoxLayout{panel};

  auto* imagesFolderOpenB =
      new QPushButton{QStringLiteral("Open images folder"), panel};
  imagesFolderOpenB->setObjectName(
      QString::fromUtf8(WindowDataContext::images_folder_open_button_id));

  auto* imagesSearchE = new QLineEdit{panel};
  imagesSearchE->setObjectName(
      QString::fromUtf8(WindowDataContext::images_search_entry_id));
  imagesSearchE->setPlaceholderText(QStringLiteral("Search images"));
  imagesSearchE->setClearButtonEnabled(true);

  auto* imagesList = new QListWidget{panel};
  imagesList->setObjectName(
      QString::fromUtf8(WindowDataContext::images_list_id));
  imagesList->setSelectionMode(QAbstractItemView::SingleSelection);
  imagesList->setUniformItemSizes(true);

  auto* deleteImageB = new QPushButton{QStringLiteral("Delete image"), panel};
  deleteImageB->setObjectName(
      QString::fromUtf8(WindowDataContext::delete_image_record_id));

  layout->addWidget(imagesFolderOpenB);
  layout->addWidget(imagesSearchE);
  layout->addWidget(imagesList, 1);
  layout->addWidget(deleteImageB);

  return panel;
}

QWidget* WindowLoader::build_center_panel(QWidget* parent)
{
  auto* panel = new QWidget{parent};
  auto* layout = new QVBoxLayout{panel};

  auto* toolbar = new QHBoxLayout{};

  auto* zoomInB = new QPushButton{QStringLiteral("Zoom in"), panel};
  zoomInB->setObjectName(
      QString::fromUtf8(WindowDataContext::current_image_zoom_in_id));

  auto* zoomOutB = new QPushButton{QStringLiteral("Zoom out"), panel};
  zoomOutB->setObjectName(
      QString::fromUtf8(WindowDataContext::current_image_zoom_out_id));

  auto* prevFileB = new QPushButton{QStringLiteral("Previous"), panel};
  prevFileB->setObjectName(
      QString::fromUtf8(WindowDataContext::previous_image_button_id));

  auto* nextFileB = new QPushButton{QStringLiteral("Next"), panel};
  nextFileB->setObjectName(
      QString::fromUtf8(WindowDataContext::next_image_button_id));

  toolbar->addWidget(zoomInB);
  toolbar->addWidget(zoomOutB);
  toolbar->addWidget(prevFileB);
  toolbar->addWidget(nextFileB);
  toolbar->addStretch(1);

  auto* scrolled = new QScrollArea{panel};
  scrolled->setObjectName(
      QString::fromUtf8(WindowDataContext::central_canvas_scrolled_window_id));

  mwctx->centralCanvas =
      mwctx->cwFactory->create_working_canvas(scrolled, mwctx->current_image);

  assert(mwctx->centralCanvas != nullptr);

  mwctx->centralCanvas->setObjectName(
      QString::fromUtf8(WindowDataContext::central_working_canvas_id));

  // setWidgetResizable stays off: the canvas sizes itself to the scaled image
  // (see CentralWorkingCanvas::set_pixmap), which is what makes the scroll bars
  // appear and keeps the canvas coordinates equal to the image ones.
  scrolled->setWidget(mwctx->centralCanvas);

  layout->addLayout(toolbar);
  layout->addWidget(scrolled, 1);

  return panel;
}

QWidget* WindowLoader::build_right_panel(QWidget* parent)
{
  auto* panel = new QWidget{parent};
  auto* layout = new QVBoxLayout{panel};

  auto* annotationsDbOpenB =
      new QPushButton{QStringLiteral("Open annotations DB"), panel};
  annotationsDbOpenB->setObjectName(
      QString::fromUtf8(WindowDataContext::annotations_db_open_button_id));

  auto* annotationSearchE = new QLineEdit{panel};
  annotationSearchE->setObjectName(
      QString::fromUtf8(WindowDataContext::annotation_search_entry_id));
  annotationSearchE->setPlaceholderText(QStringLiteral("Search annotations"));
  annotationSearchE->setClearButtonEnabled(true);

  auto* annotationsDbList = new QListWidget{panel};
  annotationsDbList->setObjectName(
      QString::fromUtf8(WindowDataContext::annotations_db_list_id));
  annotationsDbList->setSelectionMode(QAbstractItemView::SingleSelection);

  auto* copyAnnotationNameB =
      new QPushButton{QStringLiteral("Copy annotation name"), panel};
  copyAnnotationNameB->setObjectName(QString::fromUtf8(
      WindowDataContext::copy_annotation_name_to_current_rect_id));

  auto* duplicateAnnotationB =
      new QPushButton{QStringLiteral("Copy annotation"), panel};
  duplicateAnnotationB->setObjectName(
      QString::fromUtf8(WindowDataContext::duplicate_current_annotation_id));

  auto* rectEditEntry = new QLineEdit{panel};
  rectEditEntry->setObjectName(QString::fromUtf8(
      WindowDataContext::current_image_selected_annotation_edit_id));
  rectEditEntry->setToolTip(
      QStringLiteral("Edit name for current image selected annotation"));

  auto* currentImageAnnotations = new QListWidget{panel};
  currentImageAnnotations->setObjectName(
      QString::fromUtf8(WindowDataContext::current_image_annotations_id));
  currentImageAnnotations->setSelectionMode(QAbstractItemView::SingleSelection);

  auto* deleteAnnotationB =
      new QPushButton{QStringLiteral("Delete annotation"), panel};
  deleteAnnotationB->setObjectName(QString::fromUtf8(
      WindowDataContext::delete_current_image_selected_annotation_id));

  layout->addWidget(annotationsDbOpenB);
  layout->addWidget(new QLabel{QStringLiteral("All annotations list"), panel});
  layout->addWidget(annotationSearchE);
  layout->addWidget(annotationsDbList, 1);
  layout->addWidget(
      new QLabel{QStringLiteral("Current image annotations:"), panel});
  layout->addWidget(copyAnnotationNameB);
  layout->addWidget(duplicateAnnotationB);
  layout->addWidget(rectEditEntry);
  layout->addWidget(currentImageAnnotations, 1);
  layout->addWidget(deleteAnnotationB);

  return panel;
}

template <class ObjectType>
ObjectType* WindowLoader::get_object(const std::string& id)
{
  assert(window != nullptr);

  if (window == nullptr) {
    LOGE("No window available");
    return {};
  }

  LOGT("Trying to find object by id: " << id);

  ObjectType* tptr = window->findChild<ObjectType*>(
      QString::fromStdString(id), Qt::FindChildrenRecursively);

  if (tptr == nullptr) {
    LOGE("Failure during retrieving the object by id: " << id);
  }

  assert(tptr != nullptr);

  return tptr;
}

bool WindowLoader::all_widget_are_valid()
{
  if (mwctx == nullptr || mwctx->wctx == nullptr) {
    LOGE("No window context given");
    return false;
  }

  if (window == nullptr) {
    LOGE("No window was built");
    return false;
  }

  const auto& idslist = mwctx->wctx->get_ids();

  for (const auto& id : idslist) {
    // The window is the root of the search, so findChild() can never return it;
    // it is the only id verified against the loader's own pointer.
    if (id == WindowDataContext::window_id) {
      continue;
    }

    const QObject* tptr = get_object<QObject>(id);

    if (tptr == nullptr) {
      LOGE("Found the absent widget by id: " << id);
      return false;
    }

    LOGT("Widget " << id << " is present");
  }

  return true;
}

QMainWindow* WindowLoader::get_window() { return window.get(); }

QPushButton* WindowLoader::get_images_folder_open_button()
{
  return get_object<QPushButton>(
      WindowDataContext::images_folder_open_button_id);
}

QPushButton* WindowLoader::get_annotations_db_open_button()
{
  return get_object<QPushButton>(
      WindowDataContext::annotations_db_open_button_id);
}

QPushButton* WindowLoader::get_delete_current_image_selected_annotation()
{
  return get_object<QPushButton>(
      WindowDataContext::delete_current_image_selected_annotation_id);
}

QLineEdit* WindowLoader::get_annotation_search_entry()
{
  return get_object<QLineEdit>(WindowDataContext::annotation_search_entry_id);
}

QListWidget* WindowLoader::get_annotations_db_list()
{
  return get_object<QListWidget>(WindowDataContext::annotations_db_list_id);
}

QListWidget* WindowLoader::get_images_list()
{
  return get_object<QListWidget>(WindowDataContext::images_list_id);
}

QPushButton* WindowLoader::get_current_image_zoom_in()
{
  return get_object<QPushButton>(WindowDataContext::current_image_zoom_in_id);
}

QPushButton* WindowLoader::get_current_image_zoom_out()
{
  return get_object<QPushButton>(WindowDataContext::current_image_zoom_out_id);
}

QPushButton* WindowLoader::get_copy_annotation_name_button()
{
  return get_object<QPushButton>(
      WindowDataContext::copy_annotation_name_to_current_rect_id);
}

QLineEdit* WindowLoader::get_edit_current_rect_entry()
{
  return get_object<QLineEdit>(
      WindowDataContext::current_image_selected_annotation_edit_id);
}

QPushButton* WindowLoader::get_prev_file_button()
{
  return get_object<QPushButton>(WindowDataContext::previous_image_button_id);
}

QPushButton* WindowLoader::get_next_file_button()
{
  return get_object<QPushButton>(WindowDataContext::next_image_button_id);
}

QStatusBar* WindowLoader::get_window_status_bar()
{
  return get_object<QStatusBar>(WindowDataContext::main_window_status_bar_id);
}

QPushButton* WindowLoader::get_copy_current_annotation_button()
{
  return get_object<QPushButton>(
      WindowDataContext::duplicate_current_annotation_id);
}

QScrollArea* WindowLoader::get_central_scrolled_window()
{
  return get_object<QScrollArea>(
      WindowDataContext::central_canvas_scrolled_window_id);
}

QLineEdit* WindowLoader::get_images_search_entry()
{
  return get_object<QLineEdit>(WindowDataContext::images_search_entry_id);
}

QPushButton* WindowLoader::get_delete_image_record()
{
  return get_object<QPushButton>(WindowDataContext::delete_image_record_id);
}

QListWidget* WindowLoader::get_current_image_annotations()
{
  return get_object<QListWidget>(
      WindowDataContext::current_image_annotations_id);
}

QAction* WindowLoader::get_images_open_mi()
{
  return get_object<QAction>(WindowDataContext::images_open_menu_item_id);
}

QAction* WindowLoader::get_annotations_db_open_mi()
{
  return get_object<QAction>(WindowDataContext::annotations_db_open_menu);
}

QAction* WindowLoader::get_annotations_db_save_mi()
{
  return get_object<QAction>(WindowDataContext::annotations_db_save_menu_id);
}

QAction* WindowLoader::get_annotations_db_saveas_mi()
{
  return get_object<QAction>(WindowDataContext::annotations_db_save_as_menu_id);
}

QAction* WindowLoader::get_annotations_project_close_mi()
{
  return get_object<QAction>(WindowDataContext::annotations_project_close_id);
}

QAction* WindowLoader::get_about_mi()
{
  return get_object<QAction>(WindowDataContext::about_menu_item_id);
}

QAction* WindowLoader::get_export_txt2_folder_mi()
{
  return get_object<QAction>(
      WindowDataContext::annotations_export_txt_2_folder_id);
}

QAction* WindowLoader::get_export_yolo4_folder_mi()
{
  return get_object<QAction>(
      WindowDataContext::annotations_export_yolo4_2_folder2_id);
}

QAction* WindowLoader::get_export_ultralytics_detect_folder_mi()
{
  return get_object<QAction>(
      WindowDataContext::annotations_export_ultralytics_detect_2_folder_id);
}

QAction* WindowLoader::get_export_ultralytics_obb_folder_mi()
{
  return get_object<QAction>(
      WindowDataContext::annotations_export_ultralytics_obb_2_folder_id);
}

QAction* WindowLoader::get_export_ultralytics_segment_folder_mi()
{
  return get_object<QAction>(
      WindowDataContext::annotations_export_ultralytics_segment_2_folder_id);
}

QAction* WindowLoader::get_export_coco_folder_mi()
{
  return get_object<QAction>(
      WindowDataContext::annotations_export_coco_2_folder_id);
}

QAction* WindowLoader::get_export_pascal_voc_folder_mi()
{
  return get_object<QAction>(
      WindowDataContext::annotations_export_pascal_voc_2_folder_id);
}

QAction* WindowLoader::get_export_createml_folder_mi()
{
  return get_object<QAction>(
      WindowDataContext::annotations_export_createml_2_folder_id);
}

QAction* WindowLoader::get_export_pytorchvision_folder_mi()
{
  return get_object<QAction>(
      WindowDataContext::annotations_export_pytorchvision_2_folder_id);
}

QAction* WindowLoader::get_images_web_page_open_menu_item_mi()
{
  return get_object<QAction>(
      WindowDataContext::images_web_page_open_menu_item_id);
}

QAction* WindowLoader::auto_insert_current_annotation_to_whole_mi()
{
  return get_object<QAction>(
      WindowDataContext::auto_insert_current_annotation_to_whole_id);
}

}  // namespace imagesAnnotatorQt6::window
