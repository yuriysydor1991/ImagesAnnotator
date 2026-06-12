#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "src/annotator-events/events/ImageRecordRect.h"

using namespace testing;
using events::events::ImageRecordRect;
using events::events::ImageRecordRectPtr;
using events::events::ImageRecordRectSet;

namespace
{

ImageRecordRectPtr rect(const std::string& name, int x, int y, int w, int h)
{
  return std::make_shared<ImageRecordRect>(name, x, y, w, h);
}

}  // namespace

TEST(UTEST_ImageRecordRect, default_constructed_rect_is_zeroed)
{
  ImageRecordRect r;

  EXPECT_TRUE(r.name.empty());
  EXPECT_EQ(r.x, 0);
  EXPECT_EQ(r.y, 0);
  EXPECT_EQ(r.width, 0);
  EXPECT_EQ(r.height, 0);
}

TEST(UTEST_ImageRecordRect, parameterized_constructor_sets_all_fields)
{
  ImageRecordRect r{"cat", 1, 2, 30, 40};

  EXPECT_EQ(r.name, "cat");
  EXPECT_EQ(r.x, 1);
  EXPECT_EQ(r.y, 2);
  EXPECT_EQ(r.width, 30);
  EXPECT_EQ(r.height, 40);
}

TEST(UTEST_ImageRecordRect, duplicate_shared_is_a_distinct_equal_object)
{
  auto orig = rect("dog", 5, 6, 7, 8);

  auto dup = orig->duplicate_shared();

  ASSERT_NE(dup, nullptr);
  EXPECT_NE(dup.get(), orig.get());
  EXPECT_TRUE(ImageRecordRect::equal(orig, dup));
}

TEST(UTEST_ImageRecordRect, equal_ptr_same_pointer_is_equal)
{
  auto r = rect("x", 1, 1, 1, 1);

  EXPECT_TRUE(ImageRecordRect::equal(r, r));
}

TEST(UTEST_ImageRecordRect, equal_ptr_same_fields_is_equal)
{
  EXPECT_TRUE(ImageRecordRect::equal(rect("x", 1, 2, 3, 4),
                                     rect("x", 1, 2, 3, 4)));
}

TEST(UTEST_ImageRecordRect, equal_ptr_detects_each_differing_field)
{
  auto base = rect("x", 1, 2, 3, 4);

  EXPECT_FALSE(ImageRecordRect::equal(base, rect("y", 1, 2, 3, 4)));
  EXPECT_FALSE(ImageRecordRect::equal(base, rect("x", 9, 2, 3, 4)));
  EXPECT_FALSE(ImageRecordRect::equal(base, rect("x", 1, 9, 3, 4)));
  EXPECT_FALSE(ImageRecordRect::equal(base, rect("x", 1, 2, 9, 4)));
  EXPECT_FALSE(ImageRecordRect::equal(base, rect("x", 1, 2, 3, 9)));
}

TEST(UTEST_ImageRecordRect, equal_set_same_reference_is_equal)
{
  ImageRecordRectSet s{rect("a", 0, 0, 1, 1)};

  EXPECT_TRUE(ImageRecordRect::equal(s, s));
}

TEST(UTEST_ImageRecordRect, equal_set_compares_elementwise)
{
  ImageRecordRectSet l{rect("a", 0, 0, 1, 1), rect("b", 1, 1, 2, 2)};
  ImageRecordRectSet r{rect("a", 0, 0, 1, 1), rect("b", 1, 1, 2, 2)};

  EXPECT_TRUE(ImageRecordRect::equal(l, r));

  ImageRecordRectSet diff{rect("a", 0, 0, 1, 1), rect("B", 1, 1, 2, 2)};

  EXPECT_FALSE(ImageRecordRect::equal(l, diff));
}

TEST(UTEST_ImageRecordRect, equal_set_of_different_sizes_is_not_equal)
{
  ImageRecordRectSet l{rect("a", 0, 0, 1, 1), rect("b", 1, 1, 2, 2)};
  ImageRecordRectSet r{rect("a", 0, 0, 1, 1)};

  EXPECT_FALSE(ImageRecordRect::equal(l, r));
}
