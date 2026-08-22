#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "src/annotator-events/events/ImportUltralyticsDetectRequest.h"

using namespace testing;
using events::events::ImportUltralyticsDetectRequest;

TEST(UTEST_ImportUltralyticsDetectRequest, constructor_stores_the_folder_path)
{
  ImportUltralyticsDetectRequest e{"/in"};

  EXPECT_EQ(e.src_folder_path, "/in");
}

// The image measuring service is attached by the GUI toolkit which submits the
// request, so a freshly built one carries none.
TEST(UTEST_ImportUltralyticsDetectRequest, sizer_defaults_to_null)
{
  ImportUltralyticsDetectRequest e{"/in"};

  EXPECT_EQ(e.sizer, nullptr);
}
