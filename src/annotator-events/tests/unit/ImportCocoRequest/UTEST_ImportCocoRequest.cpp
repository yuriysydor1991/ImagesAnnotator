#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "src/annotator-events/events/ImportCocoRequest.h"

using namespace testing;
using events::events::ImportCocoRequest;

TEST(UTEST_ImportCocoRequest, constructor_stores_the_folder_path)
{
  ImportCocoRequest e{"/in"};

  EXPECT_EQ(e.src_folder_path, "/in");
}

// The image measuring service is attached by the GUI toolkit which submits the
// request, so a freshly built one carries none.
TEST(UTEST_ImportCocoRequest, sizer_defaults_to_null)
{
  ImportCocoRequest e{"/in"};

  EXPECT_EQ(e.sizer, nullptr);
}
