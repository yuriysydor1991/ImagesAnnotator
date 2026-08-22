#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "src/annotator-events/events/ImportUltralyticsSegmentRequest.h"

using namespace testing;
using events::events::ImportUltralyticsSegmentRequest;

TEST(UTEST_ImportUltralyticsSegmentRequest, constructor_stores_the_folder_path)
{
  ImportUltralyticsSegmentRequest e{"/in"};

  EXPECT_EQ(e.src_folder_path, "/in");
}

// The image measuring service is attached by the GUI toolkit which submits the
// request, so a freshly built one carries none.
TEST(UTEST_ImportUltralyticsSegmentRequest, sizer_defaults_to_null)
{
  ImportUltralyticsSegmentRequest e{"/in"};

  EXPECT_EQ(e.sizer, nullptr);
}
