#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <set>

#include "src/gui/wxwidgets/main-window/WindowDataContext.h"

using namespace testing;
using imagesAnnotatorWx::window::WindowDataContext;

TEST(UTEST_WxWindowDataContext, get_ids_is_not_empty)
{
  EXPECT_FALSE(WindowDataContext::get_ids().empty());
}

TEST(UTEST_WxWindowDataContext, get_ids_contains_the_core_menu_ids)
{
  const auto& ids = WindowDataContext::get_ids();

  EXPECT_EQ(ids.count(WindowDataContext::ID_IMAGES_OPEN), 1U);
  EXPECT_EQ(ids.count(WindowDataContext::ID_ANNOTATIONS_DB_OPEN), 1U);
  EXPECT_EQ(ids.count(WindowDataContext::ID_EXPORT_PYTORCH), 1U);
  EXPECT_EQ(ids.count(WindowDataContext::ID_ABOUT), 1U);
}

TEST(UTEST_WxWindowDataContext, get_ids_contains_every_import_menu_id)
{
  const auto& ids = WindowDataContext::get_ids();

  EXPECT_EQ(ids.count(WindowDataContext::ID_IMPORT_TXT), 1U);
  EXPECT_EQ(ids.count(WindowDataContext::ID_IMPORT_YOLO4), 1U);
  EXPECT_EQ(ids.count(WindowDataContext::ID_IMPORT_ULTRALYTICS_DETECT), 1U);
  EXPECT_EQ(ids.count(WindowDataContext::ID_IMPORT_ULTRALYTICS_OBB), 1U);
  EXPECT_EQ(ids.count(WindowDataContext::ID_IMPORT_ULTRALYTICS_SEGMENT), 1U);
  EXPECT_EQ(ids.count(WindowDataContext::ID_IMPORT_COCO), 1U);
  EXPECT_EQ(ids.count(WindowDataContext::ID_IMPORT_PASCAL_VOC), 1U);
  EXPECT_EQ(ids.count(WindowDataContext::ID_IMPORT_CREATEML), 1U);
  EXPECT_EQ(ids.count(WindowDataContext::ID_IMPORT_PYTORCH), 1U);
}

TEST(UTEST_WxWindowDataContext, all_ids_are_valid_custom_command_ids)
{
  for (const int id : WindowDataContext::get_ids()) {
    EXPECT_GT(id, wxID_HIGHEST);
  }
}

TEST(UTEST_WxWindowDataContext, get_ids_returns_a_stable_reference)
{
  EXPECT_EQ(&WindowDataContext::get_ids(), &WindowDataContext::get_ids());
}
