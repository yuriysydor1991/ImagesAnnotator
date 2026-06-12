#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "src/annotator-events/events/ExportYolo4FolderRequest.h"

using namespace testing;
using events::events::ExportYolo4FolderRequest;

TEST(UTEST_ExportYolo4FolderRequest, constructor_stores_the_value)
{
  ExportYolo4FolderRequest e{"/out"};

  EXPECT_EQ(e.dst_folder_path, "/out");
}
