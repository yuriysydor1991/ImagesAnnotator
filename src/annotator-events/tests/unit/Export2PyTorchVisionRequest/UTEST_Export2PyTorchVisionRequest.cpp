#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "src/annotator-events/events/Export2PyTorchVisionRequest.h"

using namespace testing;
using events::events::Export2PyTorchVisionRequest;

TEST(UTEST_Export2PyTorchVisionRequest, constructor_stores_the_folder_path)
{
  Export2PyTorchVisionRequest e{"/out"};

  EXPECT_EQ(e.dst_folder_path, "/out");
}

TEST(UTEST_Export2PyTorchVisionRequest, cropper_defaults_to_null)
{
  Export2PyTorchVisionRequest e{"/out"};

  EXPECT_EQ(e.cropper, nullptr);
}
