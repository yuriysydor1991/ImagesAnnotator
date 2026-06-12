#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "src/annotator-events/events/ExportPlainTxt2FolderRequest.h"

using namespace testing;
using events::events::ExportPlainTxt2FolderRequest;

TEST(UTEST_ExportPlainTxt2FolderRequest, constructor_stores_the_value)
{
  ExportPlainTxt2FolderRequest e{"/out"};

  EXPECT_EQ(e.dst_folder_path, "/out");
}
