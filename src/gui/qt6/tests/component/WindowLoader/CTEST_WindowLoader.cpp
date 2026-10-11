#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

#include "QtInitFixture.h"
#include "src/gui/qt6/MainWindowContext.h"
#include "src/gui/qt6/main-window/WindowDataContext.h"
#include "src/gui/qt6/main-window/WindowLoader.h"
#include "src/gui/qt6/qt_includes.h"

using namespace testing;
using imagesAnnotatorQt6::MainWindowContext;
using imagesAnnotatorQt6::window::WindowDataContext;

/**
 * @brief Exercises the hand-coded Qt window against the very same widget id set
 * the GTKmm loader resolves through its Glade builder: every id in
 * WindowDataContext has to be reachable by QObject::findChild(), which is what
 * all_widget_are_valid() checks, and every typed getter has to hand back the
 * widget behind it.
 */
class CTEST_Qt6WindowLoader : public QtInitFixture
{
 protected:
  void SetUp() override
  {
    QtInitFixture::SetUp();

    mwctx = MainWindowContext::build_context();

    ASSERT_NE(mwctx, nullptr);
    ASSERT_NE(mwctx->wloader, nullptr);
    ASSERT_TRUE(mwctx->wloader->load_window(mwctx));
  }

  void TearDown() override
  {
    // clear() drops the loader, which owns (and therefore destroys) the window,
    // and breaks the context <-> loader shared_ptr cycle - exactly what
    // Qt6Initer::deinit() does after the event loop returns.
    mwctx->clear();
  }

  std::shared_ptr<MainWindowContext> mwctx;
};

TEST_F(CTEST_Qt6WindowLoader, builds_every_declared_widget)
{
  EXPECT_NE(mwctx->wloader->get_window(), nullptr);
  EXPECT_NE(mwctx->centralCanvas, nullptr);
  EXPECT_TRUE(mwctx->wloader->all_widget_are_valid());
}

TEST_F(CTEST_Qt6WindowLoader, every_typed_getter_resolves)
{
  auto& wl = *mwctx->wloader;

  EXPECT_NE(wl.get_images_folder_open_button(), nullptr);
  EXPECT_NE(wl.get_annotations_db_open_button(), nullptr);
  EXPECT_NE(wl.get_delete_current_image_selected_annotation(), nullptr);
  EXPECT_NE(wl.get_annotation_search_entry(), nullptr);
  EXPECT_NE(wl.get_annotations_db_list(), nullptr);
  EXPECT_NE(wl.get_images_list(), nullptr);
  EXPECT_NE(wl.get_current_image_zoom_in(), nullptr);
  EXPECT_NE(wl.get_current_image_zoom_out(), nullptr);
  EXPECT_NE(wl.get_copy_annotation_name_button(), nullptr);
  EXPECT_NE(wl.get_edit_current_rect_entry(), nullptr);
  EXPECT_NE(wl.get_prev_file_button(), nullptr);
  EXPECT_NE(wl.get_next_file_button(), nullptr);
  EXPECT_NE(wl.get_window_status_bar(), nullptr);
  EXPECT_NE(wl.get_copy_current_annotation_button(), nullptr);
  EXPECT_NE(wl.get_central_scrolled_window(), nullptr);
  EXPECT_NE(wl.get_images_search_entry(), nullptr);
  EXPECT_NE(wl.get_delete_image_record(), nullptr);
  EXPECT_NE(wl.get_current_image_annotations(), nullptr);
}

TEST_F(CTEST_Qt6WindowLoader, every_menu_action_resolves)
{
  auto& wl = *mwctx->wloader;

  EXPECT_NE(wl.get_images_open_mi(), nullptr);
  EXPECT_NE(wl.get_annotations_db_open_mi(), nullptr);
  EXPECT_NE(wl.get_annotations_db_save_mi(), nullptr);
  EXPECT_NE(wl.get_annotations_db_saveas_mi(), nullptr);
  EXPECT_NE(wl.get_annotations_project_close_mi(), nullptr);
  EXPECT_NE(wl.get_about_mi(), nullptr);
  EXPECT_NE(wl.get_export_txt2_folder_mi(), nullptr);
  EXPECT_NE(wl.get_export_yolo4_folder_mi(), nullptr);
  EXPECT_NE(wl.get_export_pytorchvision_folder_mi(), nullptr);
  EXPECT_NE(wl.get_import_dxxwjz3p_project_mi(), nullptr);
  EXPECT_NE(wl.get_import_dxxwjz2p_project_mi(), nullptr);
  EXPECT_NE(wl.get_import_dxxwjz3_folder_mi(), nullptr);
  EXPECT_NE(wl.get_import_dxxwjz2_folder_mi(), nullptr);
  EXPECT_NE(wl.get_images_web_page_open_menu_item_mi(), nullptr);
  EXPECT_NE(wl.auto_insert_current_annotation_to_whole_mi(), nullptr);
}

TEST_F(CTEST_Qt6WindowLoader, darknetxx_import_submenu_sits_above_yolo4_import)
{
  auto& wl = *mwctx->wloader;

  QAction* const yolo4Import = wl.get_import_yolo4_folder_mi();

  ASSERT_NE(yolo4Import, nullptr);

  const auto menus = wl.get_window()->findChildren<QMenu*>();

  QMenu* project{nullptr};

  for (QMenu* menu : menus) {
    if (menu->actions().contains(yolo4Import)) {
      project = menu;
    }
  }

  ASSERT_NE(project, nullptr);

  // the entry right above the YOLO4 import is the darknetxx submenu
  const auto projectActions = project->actions();
  const auto yolo4At = projectActions.indexOf(yolo4Import);

  ASSERT_GT(yolo4At, 0);

  QMenu* darknetxx{nullptr};

  for (QMenu* menu : menus) {
    if (menu->menuAction() == projectActions.at(yolo4At - 1)) {
      darknetxx = menu;
    }
  }

  ASSERT_NE(darknetxx, nullptr);
  EXPECT_TRUE(darknetxx->title().contains(QStringLiteral("DarknetXX")));

  const QList<QAction*> expected{
      wl.get_import_dxxwjz3p_project_mi(), wl.get_import_dxxwjz2p_project_mi(),
      wl.get_import_dxxwjz3_folder_mi(), wl.get_import_dxxwjz2_folder_mi()};

  EXPECT_EQ(darknetxx->actions(), expected);

  ASSERT_EQ(darknetxx->actions().size(), 4);
  EXPECT_EQ(darknetxx->actions().at(0)->text(),
            QStringLiteral("dxxwjz3p project"));
  EXPECT_EQ(darknetxx->actions().at(1)->text(),
            QStringLiteral("dxxwjz2p project"));
  EXPECT_EQ(darknetxx->actions().at(2)->text(),
            QStringLiteral("dxxwjz3 with YOLO4 folder"));
  EXPECT_EQ(darknetxx->actions().at(3)->text(),
            QStringLiteral("dxxwjz2 with YOLO4 folder"));
}

TEST_F(CTEST_Qt6WindowLoader, carries_the_gtkmm_menu_accelerators)
{
  auto& wl = *mwctx->wloader;

  EXPECT_EQ(wl.get_annotations_db_open_mi()->shortcut(),
            QKeySequence(Qt::CTRL | Qt::Key_O));
  EXPECT_EQ(wl.get_annotations_db_save_mi()->shortcut(),
            QKeySequence(Qt::CTRL | Qt::Key_S));
  EXPECT_EQ(wl.get_annotations_db_saveas_mi()->shortcut(),
            QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_S));
  EXPECT_EQ(wl.get_annotations_project_close_mi()->shortcut(),
            QKeySequence(Qt::CTRL | Qt::Key_W));
  EXPECT_EQ(wl.get_images_open_mi()->shortcut(),
            QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_O));
  EXPECT_EQ(wl.get_images_web_page_open_menu_item_mi()->shortcut(),
            QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_U));
  EXPECT_EQ(wl.auto_insert_current_annotation_to_whole_mi()->shortcut(),
            QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_F));
}

TEST_F(CTEST_Qt6WindowLoader, the_canvas_lives_inside_the_scroll_area)
{
  EXPECT_EQ(mwctx->wloader->get_central_scrolled_window()->widget(),
            mwctx->centralCanvas);
}

TEST_F(CTEST_Qt6WindowLoader, window_title_carries_the_opened_project)
{
  mwctx->wloader->update_window_title("some-project.json");

  EXPECT_TRUE(mwctx->wloader->get_window()->windowTitle().contains(
      QStringLiteral("some-project.json")));

  mwctx->wloader->update_window_title();

  EXPECT_FALSE(mwctx->wloader->get_window()->windowTitle().contains(
      QStringLiteral("some-project.json")));
}

TEST_F(CTEST_Qt6WindowLoader, embedded_resources_are_compiled_in)
{
  // The logo and the style sheet come from src/gui/qt6/ui/template.qrc, which
  // AUTORCC links into the binary; a broken resource path would leave the
  // window unstyled and iconless without any other symptom.
  EXPECT_FALSE(
      QPixmap{QString::fromUtf8(WindowDataContext::logo_res_path)}.isNull());

  QFile qss{QString::fromUtf8(WindowDataContext::main_window_qss)};
  EXPECT_TRUE(qss.open(QIODevice::ReadOnly | QIODevice::Text));

  EXPECT_FALSE(mwctx->wloader->get_window()->styleSheet().isEmpty());
}
