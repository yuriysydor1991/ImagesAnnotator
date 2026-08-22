#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "src/annotator-events/events/ImportCreateMLRequest.h"

using namespace testing;
using events::events::ImportCreateMLRequest;

TEST(UTEST_ImportCreateMLRequest, constructor_stores_the_folder_path)
{
  ImportCreateMLRequest e{"/in"};

  EXPECT_EQ(e.src_folder_path, "/in");
}

// The image measuring service is attached by the GUI toolkit which submits the
// request, so a freshly built one carries none.
TEST(UTEST_ImportCreateMLRequest, sizer_defaults_to_null)
{
  ImportCreateMLRequest e{"/in"};

  EXPECT_EQ(e.sizer, nullptr);
}
