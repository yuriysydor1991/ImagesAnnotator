#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "src/annotator-events/events/ImportPyTorchVisionRequest.h"

using namespace testing;
using events::events::ImportPyTorchVisionRequest;

TEST(UTEST_ImportPyTorchVisionRequest, constructor_stores_the_folder_path)
{
  ImportPyTorchVisionRequest e{"/in"};

  EXPECT_EQ(e.src_folder_path, "/in");
}

// The image measuring service is attached by the GUI toolkit which submits the
// request, so a freshly built one carries none.
TEST(UTEST_ImportPyTorchVisionRequest, sizer_defaults_to_null)
{
  ImportPyTorchVisionRequest e{"/in"};

  EXPECT_EQ(e.sizer, nullptr);
}
