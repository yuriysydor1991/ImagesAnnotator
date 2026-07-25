#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

#include "src/annotator-events/events/ImageRecord.h"
#include "src/gui/wxwidgets/main-window/custom-widgets/ImagePathLabel.h"

using namespace testing;
using events::events::ImageRecord;
using templateWx::window::custom_widgets::ImagePathLabel;

// The wx ImagePathLabel is a plain row-data holder (not a wx widget), so unlike
// its GTKmm counterpart it needs no display to construct and can be unit
// tested.

TEST(UTEST_WxImagePathLabel, exposes_the_wrapped_image_record)
{
  auto ir = ImageRecord::create("a.png", "/d");

  ImagePathLabel label{ir};

  EXPECT_EQ(label.get_image_rec(), ir);
}

TEST(UTEST_WxImagePathLabel, displays_the_records_full_path)
{
  auto ir = ImageRecord::create("a.png", "/d");

  ImagePathLabel label{ir};

  EXPECT_EQ(label.get_text(), "/d/a.png");
}

TEST(UTEST_WxImagePathLabel, has_records_mark_is_added_and_removed)
{
  auto ir = ImageRecord::create("a.png", "/d");

  ImagePathLabel label{ir};

  EXPECT_FALSE(label.has_records());

  EXPECT_TRUE(label.mark_as_has_records());
  EXPECT_TRUE(label.has_records());

  EXPECT_TRUE(label.remove_has_records_mark());
  EXPECT_FALSE(label.has_records());
}
