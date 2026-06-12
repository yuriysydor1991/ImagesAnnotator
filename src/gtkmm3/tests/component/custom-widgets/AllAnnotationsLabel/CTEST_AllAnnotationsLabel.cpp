#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "GtkInitFixture.h"
#include "src/gtkmm3/main-window/custom-widgets/AllAnnotationsLabel.h"

using namespace testing;
using templateGtkmm3::window::custom_widgets::AllAnnotationsLabel;

class CTEST_AllAnnotationsLabel : public GtkInitFixture
{
};

TEST_F(CTEST_AllAnnotationsLabel, displays_the_constructed_text)
{
  AllAnnotationsLabel label{"cat"};

  EXPECT_EQ(label.get_text(), "cat");
}

TEST_F(CTEST_AllAnnotationsLabel, supports_an_empty_name)
{
  AllAnnotationsLabel label{""};

  EXPECT_TRUE(label.get_text().empty());
}
