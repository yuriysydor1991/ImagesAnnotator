#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

#include "GtkInitFixture.h"
#include "src/annotator-events/events/ImageRecordRect.h"
#include "src/gtkmm3/main-window/custom-widgets/ImageRectsLabel.h"

using namespace testing;
using events::events::ImageRecordRect;
using templateGtkmm3::window::custom_widgets::ImageRectsLabel;

class CTEST_ImageRectsLabel : public GtkInitFixture
{
};

TEST_F(CTEST_ImageRectsLabel, default_label_holds_no_rect)
{
  ImageRectsLabel label;

  EXPECT_EQ(label.get(), nullptr);
}

TEST_F(CTEST_ImageRectsLabel, set_stores_the_rect_and_shows_its_name)
{
  ImageRectsLabel label;
  auto rect = std::make_shared<ImageRecordRect>("cat", 1, 2, 3, 4);

  label.set(rect);

  EXPECT_EQ(label.get(), rect);
  EXPECT_EQ(label.get_text(), "cat");
}
