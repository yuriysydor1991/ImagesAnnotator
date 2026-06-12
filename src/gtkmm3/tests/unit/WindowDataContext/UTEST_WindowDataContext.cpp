#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <set>
#include <string>

#include "src/gtkmm3/main-window/WindowDataContext.h"

using namespace testing;
using templateGtkmm3::window::WindowDataContext;

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
