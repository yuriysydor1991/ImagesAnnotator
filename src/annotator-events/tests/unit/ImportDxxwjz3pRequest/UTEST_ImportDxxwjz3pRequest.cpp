#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "src/annotator-events/events/ImportDxxwjz3pRequest.h"

using namespace testing;
using events::events::ImportDxxwjz3pRequest;

TEST(UTEST_ImportDxxwjz3pRequest, constructor_stores_the_path)
{
  ImportDxxwjz3pRequest e{"/in"};

  EXPECT_EQ(e.src_folder_path, "/in");
}

// The image measuring service is attached by the GUI toolkit which submits the
// request, so a freshly built one carries none.
TEST(UTEST_ImportDxxwjz3pRequest, sizer_defaults_to_null)
{
  ImportDxxwjz3pRequest e{"/in"};

  EXPECT_EQ(e.sizer, nullptr);
}
