#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "src/annotator-events/events/ImportUltralyticsObbRequest.h"

using namespace testing;
using events::events::ImportUltralyticsObbRequest;

TEST(UTEST_ImportUltralyticsObbRequest, constructor_stores_the_folder_path)
{
  ImportUltralyticsObbRequest e{"/in"};

  EXPECT_EQ(e.src_folder_path, "/in");
}

// The image measuring service is attached by the GUI toolkit which submits the
// request, so a freshly built one carries none.
TEST(UTEST_ImportUltralyticsObbRequest, sizer_defaults_to_null)
{
  ImportUltralyticsObbRequest e{"/in"};

  EXPECT_EQ(e.sizer, nullptr);
}
