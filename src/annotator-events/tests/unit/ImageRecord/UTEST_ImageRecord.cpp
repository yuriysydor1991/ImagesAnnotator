#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "src/annotator-events/events/ImageRecord.h"
#include "src/annotator-events/events/ImageRecordRect.h"

using namespace testing;
using events::events::ImageRecord;
using events::events::ImageRecordPtr;
using events::events::ImageRecordRect;
using events::events::ImageRecordsSet;

namespace
{

ImageRecordPtr withRect(const std::string& path, const std::string& absdir,
                        const std::string& rectName)
{
  auto ir = ImageRecord::create(path, absdir);
  ir->rects.emplace_back(
      std::make_shared<ImageRecordRect>(rectName, 1, 2, 3, 4));
  ir->current_rect = ir->rects.back();
  return ir;
}

}  // namespace

TEST(UTEST_ImageRecord, create_sets_path_and_defaults)
{
  auto ir = ImageRecord::create("rel.png", "/abs");

  EXPECT_EQ(ir->path, "rel.png");
  EXPECT_EQ(ir->abs_dir_path, "/abs");
  EXPECT_TRUE(ir->rects.empty());
  EXPECT_EQ(ir->current_rect, nullptr);
  EXPECT_DOUBLE_EQ(ir->imageScale, ImageRecord::defaultInitScaling);
}

TEST(UTEST_ImageRecord, get_full_path_without_abs_dir_returns_path)
{
  EXPECT_EQ(ImageRecord::create("only.png", "")->get_full_path(), "only.png");
}

TEST(UTEST_ImageRecord, get_full_path_joins_abs_dir_and_path)
{
  EXPECT_EQ(ImageRecord::create("b.png", "/root")->get_full_path(),
            "/root/b.png");
}

TEST(UTEST_ImageRecord, get_full_path_prefers_the_tmp_path_when_set)
{
  auto ir = ImageRecord::create("b.png", "/root");
  ir->tmppath = "/tmp/cache/x.png";

  EXPECT_EQ(ir->get_full_path(), "/tmp/cache/x.png");
}

TEST(UTEST_ImageRecord, scale_step_in_and_out_change_the_scale_by_one_step)
{
  auto ir = ImageRecord::create("a.png", "");

  ir->scaleStepIn();
  EXPECT_DOUBLE_EQ(ir->imageScale,
                   ImageRecord::defaultInitScaling +
                       ImageRecord::defaultScaleStep);

  ir->scaleStepOut();
  EXPECT_DOUBLE_EQ(ir->imageScale, ImageRecord::defaultInitScaling);
}

TEST(UTEST_ImageRecord, erase_current_rect_without_current_rect_fails)
{
  auto ir = ImageRecord::create("a.png", "");

  EXPECT_FALSE(ir->erase_current_rect());
}

TEST(UTEST_ImageRecord, erase_current_rect_removes_it_and_clears_selection)
{
  auto ir = withRect("a.png", "/d", "cat");

  ASSERT_EQ(ir->rects.size(), 1U);
  EXPECT_TRUE(ir->erase_current_rect());
  EXPECT_TRUE(ir->rects.empty());
  EXPECT_EQ(ir->current_rect, nullptr);
}

TEST(UTEST_ImageRecord, erase_current_rect_not_in_rects_fails)
{
  auto ir = ImageRecord::create("a.png", "");
  ir->current_rect = std::make_shared<ImageRecordRect>("stray", 0, 0, 1, 1);

  EXPECT_FALSE(ir->erase_current_rect());
}

TEST(UTEST_ImageRecord, equal_ptr_same_fields_is_equal)
{
  EXPECT_TRUE(ImageRecord::equal(ImageRecord::create("p", "/d"),
                                 ImageRecord::create("p", "/d")));
}

TEST(UTEST_ImageRecord, equal_ptr_detects_differing_path)
{
  EXPECT_FALSE(ImageRecord::equal(ImageRecord::create("p", "/d"),
                                  ImageRecord::create("q", "/d")));
}

TEST(UTEST_ImageRecord, equal_ptr_detects_differing_rects)
{
  EXPECT_FALSE(ImageRecord::equal(withRect("p", "/d", "cat"),
                                  ImageRecord::create("p", "/d")));
}

TEST(UTEST_ImageRecord, duplicate_shared_is_a_distinct_deep_equal_copy)
{
  auto orig = withRect("p", "/d", "cat");
  orig->imageScale = 1.5;
  orig->iwidth = 10;
  orig->iheight = 20;

  auto dup = orig->duplicate_shared();

  ASSERT_NE(dup, nullptr);
  EXPECT_NE(dup.get(), orig.get());
  EXPECT_TRUE(ImageRecord::equal(orig, dup));

  ASSERT_EQ(dup->rects.size(), 1U);
  EXPECT_NE(dup->rects[0].get(), orig->rects[0].get());
  EXPECT_TRUE(ImageRecordRect::equal(orig->rects[0], dup->rects[0]));
}

TEST(UTEST_ImageRecord, duplicate_set_deep_copies_every_record)
{
  ImageRecordsSet orig{withRect("a.png", "/d", "cat"),
                       ImageRecord::create("b.png", "/d")};

  auto dup = ImageRecord::duplicate(orig);

  ASSERT_EQ(dup.size(), orig.size());
  EXPECT_NE(dup[0].get(), orig[0].get());
  EXPECT_TRUE(ImageRecord::equal(orig, dup));
}
