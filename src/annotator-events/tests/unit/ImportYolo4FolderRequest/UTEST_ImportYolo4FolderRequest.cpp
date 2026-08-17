#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "src/annotator-events/events/ImportYolo4FolderRequest.h"

using namespace testing;
using events::events::ImportYolo4FolderRequest;

TEST(UTEST_ImportYolo4FolderRequest, constructor_stores_the_folder_path)
{
  ImportYolo4FolderRequest e{"/in"};

  EXPECT_EQ(e.src_folder_path, "/in");
}

// The image measuring service is attached by the GUI toolkit which submits the
// request, so a freshly built one carries none.
TEST(UTEST_ImportYolo4FolderRequest, sizer_defaults_to_null)
{
  ImportYolo4FolderRequest e{"/in"};

  EXPECT_EQ(e.sizer, nullptr);
}
