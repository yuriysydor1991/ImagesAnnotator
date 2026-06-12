#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include "src/annotator-events/events/ImageRecord.h"
#include "src/helpers/SortHelper.h"

using namespace testing;
using events::events::ImageRecord;
using events::events::ImageRecordPtr;
using events::events::ImageRecordsSet;
using helpers::SortHelper;

namespace
{

ImageRecordPtr rec(const std::string& path, const std::string& absdir = "")
{
  return ImageRecord::create(path, absdir);
}

}  // namespace

TEST(UTEST_SortHelper, returns_a_valid_callable)
{
  auto sorter = SortHelper::get_image_records_sorter();

  EXPECT_TRUE(static_cast<bool>(sorter));
}

TEST(UTEST_SortHelper, orders_records_by_full_path_ascending)
{
  auto sorter = SortHelper::get_image_records_sorter();

  auto a = rec("a.png");
  auto b = rec("b.png");

  EXPECT_TRUE(sorter(a, b));
  EXPECT_FALSE(sorter(b, a));
}

TEST(UTEST_SortHelper, equal_full_paths_are_not_less_than_each_other)
{
  auto sorter = SortHelper::get_image_records_sorter();

  auto a = rec("same.png", "/root");
  auto b = rec("same.png", "/root");

  EXPECT_FALSE(sorter(a, b));
  EXPECT_FALSE(sorter(b, a));
}

TEST(UTEST_SortHelper, comparison_uses_the_absolute_directory_prefix)
{
  auto sorter = SortHelper::get_image_records_sorter();

  // Full paths compared are "/a/z.png" and "/z/a.png": the directory prefix
  // wins, so the record under "/a" sorts before the one under "/z" even though
  // its file name ("z.png") is lexicographically greater.
  auto under_a = rec("z.png", "/a");
  auto under_z = rec("a.png", "/z");

  EXPECT_TRUE(sorter(under_a, under_z));
  EXPECT_FALSE(sorter(under_z, under_a));
}

TEST(UTEST_SortHelper, sorts_a_records_set_into_full_path_order)
{
  auto sorter = SortHelper::get_image_records_sorter();

  ImageRecordsSet recs{rec("c.png"), rec("a.png"), rec("b.png")};

  std::sort(recs.begin(), recs.end(), sorter);

  ASSERT_EQ(recs.size(), 3U);
  EXPECT_EQ(recs[0]->get_full_path(), "a.png");
  EXPECT_EQ(recs[1]->get_full_path(), "b.png");
  EXPECT_EQ(recs[2]->get_full_path(), "c.png");
}

TEST(UTEST_SortHelper, null_pointer_operands_compare_as_not_less)
{
  auto sorter = SortHelper::get_image_records_sorter();
  auto a = rec("a.png");

  EXPECT_FALSE(sorter(nullptr, a));
  EXPECT_FALSE(sorter(a, nullptr));
  EXPECT_FALSE(sorter(nullptr, nullptr));
}
