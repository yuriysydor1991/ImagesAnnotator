#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "src/annotator-events/events/DeleteCurrentImageRequest.h"

using namespace testing;
using events::events::DeleteCurrentImageRequest;

TEST(UTEST_DeleteCurrentImageRequest, constructor_stores_the_value)
{
  DeleteCurrentImageRequest e{"/d/a.png"};

  EXPECT_EQ(e.image_full_path, "/d/a.png");
}
