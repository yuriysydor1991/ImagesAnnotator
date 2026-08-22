#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "src/annotator-events/events/ImportPascalVocRequest.h"

using namespace testing;
using events::events::ImportPascalVocRequest;

TEST(UTEST_ImportPascalVocRequest, constructor_stores_the_folder_path)
{
  ImportPascalVocRequest e{"/in"};

  EXPECT_EQ(e.src_folder_path, "/in");
}

// The image measuring service is attached by the GUI toolkit which submits the
// request, so a freshly built one carries none.
TEST(UTEST_ImportPascalVocRequest, sizer_defaults_to_null)
{
  ImportPascalVocRequest e{"/in"};

  EXPECT_EQ(e.sizer, nullptr);
}
