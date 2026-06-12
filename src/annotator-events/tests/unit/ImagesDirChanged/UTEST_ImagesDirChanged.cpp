#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "src/annotator-events/events/ImagesDirChanged.h"

using namespace testing;
using events::events::ImagesDirChanged;

TEST(UTEST_ImagesDirChanged, constructor_stores_the_value)
{
  ImagesDirChanged e{"/imgs"};

  EXPECT_EQ(e.images_dir, "/imgs");
}
