#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "src/annotator-events/events/ImageRecord.h"
#include "src/helpers/ImageRecordUrlAndPathHelper.h"

using namespace testing;
using events::events::ImageRecord;
using events::events::ImageRecordPtr;
using helpers::ImageRecordUrlAndPathHelper;

namespace
{

ImageRecordPtr ir(const std::string& path, const std::string& absdir)
{
  return ImageRecord::create(path, absdir);
}

}  // namespace

TEST(UTEST_ImageRecordUrlAndPathHelper, has_urls_true_when_abs_dir_is_url)
{
  EXPECT_TRUE(
      ImageRecordUrlAndPathHelper::has_urls(ir("img.png", "http://h/dir/")));
}

TEST(UTEST_ImageRecordUrlAndPathHelper, has_urls_true_when_path_is_url)
{
  EXPECT_TRUE(
      ImageRecordUrlAndPathHelper::has_urls(ir("http://h/img.png", "/local")));
}

TEST(UTEST_ImageRecordUrlAndPathHelper, has_urls_false_for_local_paths)
{
  EXPECT_FALSE(
      ImageRecordUrlAndPathHelper::has_urls(ir("img.png", "/local/dir")));
}

TEST(UTEST_ImageRecordUrlAndPathHelper, has_urls_false_for_null_record)
{
  EXPECT_FALSE(ImageRecordUrlAndPathHelper::has_urls(nullptr));
}

TEST(UTEST_ImageRecordUrlAndPathHelper, get_ir_path_resolves_url_dir_and_rel)
{
  EXPECT_EQ(
      ImageRecordUrlAndPathHelper::get_ir_path(ir("img.png", "http://h/dir/")),
      "http://h/dir/img.png");
}

TEST(UTEST_ImageRecordUrlAndPathHelper, get_ir_path_returns_path_when_both_urls)
{
  EXPECT_EQ(ImageRecordUrlAndPathHelper::get_ir_path(
                ir("http://other/z.png", "http://h/dir/")),
            "http://other/z.png");
}

TEST(UTEST_ImageRecordUrlAndPathHelper, get_ir_path_returns_full_path_for_local)
{
  EXPECT_EQ(ImageRecordUrlAndPathHelper::get_ir_path(ir("a.png", "/root")),
            "/root/a.png");
}

TEST(UTEST_ImageRecordUrlAndPathHelper, get_ir_path_empty_for_null_record)
{
  EXPECT_TRUE(ImageRecordUrlAndPathHelper::get_ir_path(nullptr).empty());
}
