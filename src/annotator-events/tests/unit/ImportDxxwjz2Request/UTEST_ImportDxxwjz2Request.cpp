#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "src/annotator-events/events/ImportDxxwjz2Request.h"

using namespace testing;
using events::events::ImportDxxwjz2Request;

TEST(UTEST_ImportDxxwjz2Request, constructor_stores_the_path)
{
  ImportDxxwjz2Request e{"/in"};

  EXPECT_EQ(e.src_folder_path, "/in");
}

// The image measuring service is attached by the GUI toolkit which submits the
// request, so a freshly built one carries none.
TEST(UTEST_ImportDxxwjz2Request, sizer_defaults_to_null)
{
  ImportDxxwjz2Request e{"/in"};

  EXPECT_EQ(e.sizer, nullptr);
}
