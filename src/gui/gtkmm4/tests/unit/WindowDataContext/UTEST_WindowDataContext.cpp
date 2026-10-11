#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <set>
#include <string>

#include "src/gui/gtkmm4/main-window/WindowDataContext.h"

using namespace testing;
using imagesAnnotatorGtkmm4::window::WindowDataContext;

TEST(UTEST_WindowDataContext, get_ids_is_not_empty)
{
  EXPECT_FALSE(WindowDataContext::get_ids().empty());
}

TEST(UTEST_WindowDataContext, get_ids_contains_the_core_widget_ids)
{
  const auto& ids = WindowDataContext::get_ids();

  EXPECT_EQ(ids.count(WindowDataContext::window_id), 1U);
  EXPECT_EQ(ids.count(WindowDataContext::images_list_id), 1U);
  EXPECT_EQ(ids.count(WindowDataContext::annotations_db_list_id), 1U);
  EXPECT_EQ(ids.count(WindowDataContext::current_image_annotations_id), 1U);
  EXPECT_EQ(ids.count(WindowDataContext::about_dialog_id), 1U);
  EXPECT_EQ(ids.count(WindowDataContext::spinner_id), 1U);
}

TEST(UTEST_WindowDataContext, get_ids_has_no_empty_entries)
{
  for (const auto& id : WindowDataContext::get_ids()) {
    EXPECT_FALSE(id.empty());
  }
}

TEST(UTEST_WindowDataContext, get_ids_returns_a_stable_reference)
{
  EXPECT_EQ(&WindowDataContext::get_ids(), &WindowDataContext::get_ids());
}

// The builder defined directory choosers of the darknetxx exports are looked
// up by these ids, so the window loader resolves them only once listed here.
TEST(UTEST_WindowDataContext, get_ids_contains_the_darknetxx_export_ids)
{
  const auto& ids = WindowDataContext::get_ids();

  EXPECT_EQ(ids.count(WindowDataContext::dxxwjz2_export_folder_dialog_id), 1U);
  EXPECT_EQ(ids.count(WindowDataContext::dxxwjz2p_export_folder_dialog_id), 1U);
  EXPECT_EQ(ids.count(WindowDataContext::dxxwjz3_export_folder_dialog_id), 1U);
  EXPECT_EQ(ids.count(WindowDataContext::dxxwjz3p_export_folder_dialog_id), 1U);
}

// The builder defined directory choosers of the darknetxx folder imports and
// the file choosers of the darknetxx project imports are looked up by these
// ids, so the window loader resolves them only once listed here.
TEST(UTEST_WindowDataContext, get_ids_contains_the_darknetxx_import_ids)
{
  const auto& ids = WindowDataContext::get_ids();

  EXPECT_EQ(ids.count(WindowDataContext::dxxwjz2_import_folder_dialog_id), 1U);
  EXPECT_EQ(ids.count(WindowDataContext::dxxwjz2p_import_file_dialog_id), 1U);
  EXPECT_EQ(ids.count(WindowDataContext::dxxwjz3_import_folder_dialog_id), 1U);
  EXPECT_EQ(ids.count(WindowDataContext::dxxwjz3p_import_file_dialog_id), 1U);
}

TEST(UTEST_WindowDataContext, get_ids_contains_every_import_id)
{
  const auto& ids = WindowDataContext::get_ids();

  EXPECT_EQ(ids.count(WindowDataContext::txt_import_folder_dialog_id), 1U);
  EXPECT_EQ(ids.count(WindowDataContext::yolo4_import_folder_dialog_id), 1U);
  EXPECT_EQ(
      ids.count(WindowDataContext::ultralytics_detect_import_folder_dialog_id),
      1U);
  EXPECT_EQ(
      ids.count(WindowDataContext::ultralytics_obb_import_folder_dialog_id),
      1U);
  EXPECT_EQ(
      ids.count(WindowDataContext::ultralytics_segment_import_folder_dialog_id),
      1U);
  EXPECT_EQ(ids.count(WindowDataContext::coco_import_folder_dialog_id), 1U);
  EXPECT_EQ(ids.count(WindowDataContext::pascal_voc_import_folder_dialog_id),
            1U);
  EXPECT_EQ(ids.count(WindowDataContext::createml_import_folder_dialog_id), 1U);
  EXPECT_EQ(ids.count(WindowDataContext::pytorch_import_folder_dialog_id), 1U);
  EXPECT_EQ(ids.count(WindowDataContext::dxxwjz2_import_folder_dialog_id), 1U);
  EXPECT_EQ(ids.count(WindowDataContext::dxxwjz2p_import_file_dialog_id), 1U);
  EXPECT_EQ(ids.count(WindowDataContext::dxxwjz3_import_folder_dialog_id), 1U);
  EXPECT_EQ(ids.count(WindowDataContext::dxxwjz3p_import_file_dialog_id), 1U);
}
