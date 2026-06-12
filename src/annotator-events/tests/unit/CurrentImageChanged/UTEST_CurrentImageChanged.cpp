#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

#include "src/annotator-events/events/CurrentImageChanged.h"
#include "src/annotator-events/events/ImageRecord.h"

using namespace testing;
using events::events::CurrentImageChanged;
using events::events::ImageRecord;

TEST(UTEST_CurrentImageChanged, constructor_stores_the_record)
{
  auto ir = ImageRecord::create("a.png", "/d");

  CurrentImageChanged e{ir};

  EXPECT_EQ(e.new_current_image, ir);
}

TEST(UTEST_CurrentImageChanged, constructor_accepts_a_null_record)
{
  CurrentImageChanged e{nullptr};

  EXPECT_EQ(e.new_current_image, nullptr);
}
