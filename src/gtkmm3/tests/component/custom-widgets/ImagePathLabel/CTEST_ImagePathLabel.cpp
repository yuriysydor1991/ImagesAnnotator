#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

#include "GtkInitFixture.h"
#include "src/annotator-events/events/ImageRecord.h"
#include "src/gtkmm3/main-window/custom-widgets/ImagePathLabel.h"

using namespace testing;
using events::events::ImageRecord;
using templateGtkmm3::window::custom_widgets::ImagePathLabel;

class CTEST_ImagePathLabel : public GtkInitFixture
{
};

TEST_F(CTEST_ImagePathLabel, exposes_the_wrapped_image_record)
{
  auto ir = ImageRecord::create("a.png", "/d");

  ImagePathLabel label{ir};

  EXPECT_EQ(label.get_image_rec(), ir);
}

TEST_F(CTEST_ImagePathLabel, displays_the_records_full_path)
{
  auto ir = ImageRecord::create("a.png", "/d");

  ImagePathLabel label{ir};

  EXPECT_EQ(label.get_text(), "/d/a.png");
}

TEST_F(CTEST_ImagePathLabel, has_records_mark_is_added_and_removed)
{
  auto ir = ImageRecord::create("a.png", "/d");

  ImagePathLabel label{ir};

  EXPECT_TRUE(label.mark_as_has_records());
  EXPECT_TRUE(label.remove_has_records_mark());
}
