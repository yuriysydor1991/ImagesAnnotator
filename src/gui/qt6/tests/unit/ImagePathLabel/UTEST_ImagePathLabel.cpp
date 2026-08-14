#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "src/annotator-events/events/ImageRecord.h"
#include "src/gui/qt6/main-window/custom-widgets/ImagePathLabel.h"

using namespace testing;
using events::events::ImageRecord;
using imagesAnnotatorQt6::window::custom_widgets::ImagePathLabel;

TEST(UTEST_Qt6ImagePathLabel, exposes_the_records_full_path_as_its_text)
{
  auto ir = ImageRecord::create("a.png", "/d");

  ImagePathLabel label{ir};

  EXPECT_EQ(label.get_text(), "/d/a.png");
  EXPECT_EQ(label.get_image_rec(), ir);
}

TEST(UTEST_Qt6ImagePathLabel, has_records_mark_is_added_and_removed)
{
  auto ir = ImageRecord::create("a.png", "/d");

  ImagePathLabel label{ir};

  EXPECT_FALSE(label.has_records());

  EXPECT_TRUE(label.mark_as_has_records());
  EXPECT_TRUE(label.has_records());

  EXPECT_TRUE(label.remove_has_records_mark());
  EXPECT_FALSE(label.has_records());
}
