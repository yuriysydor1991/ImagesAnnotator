#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "src/annotator-events/events/ImportPlainTxt2FolderRequest.h"

using namespace testing;
using events::events::ImportPlainTxt2FolderRequest;

TEST(UTEST_ImportPlainTxt2FolderRequest, constructor_stores_the_folder_path)
{
  ImportPlainTxt2FolderRequest e{"/in"};

  EXPECT_EQ(e.src_folder_path, "/in");
}

// The image measuring service is attached by the GUI toolkit which submits the
// request, so a freshly built one carries none.
TEST(UTEST_ImportPlainTxt2FolderRequest, sizer_defaults_to_null)
{
  ImportPlainTxt2FolderRequest e{"/in"};

  EXPECT_EQ(e.sizer, nullptr);
}
