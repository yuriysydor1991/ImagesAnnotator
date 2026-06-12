#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "src/annotator-business/dbs/AnnotationsDBs/mergers/DefaultDBMerger.h"
#include "src/annotator-events/events/ImageRecord.h"

using namespace testing;
using events::events::ImageRecord;
using events::events::ImageRecordPtr;
using events::events::ImageRecordsSet;
using iannotator::dbs::annotations::merger::DefaultDBMerger;

namespace
{

ImageRecordPtr ir(const std::string& path, const std::string& absdir)
{
  return ImageRecord::create(path, absdir);
}

}  // namespace

TEST(UTEST_DefaultDBMerger, appends_new_records_and_skips_duplicates)
{
  DefaultDBMerger merger;

  ImageRecordsSet dst{ir("a.png", "/d"), ir("b.png", "/d")};
  ImageRecordsSet append{ir("b.png", "/d"), ir("c.png", "/d")};

  merger.merge(dst, append);

  ASSERT_EQ(dst.size(), 3U);
  EXPECT_EQ(dst[0]->get_full_path(), "/d/a.png");
  EXPECT_EQ(dst[1]->get_full_path(), "/d/b.png");
  EXPECT_EQ(dst[2]->get_full_path(), "/d/c.png");
}

TEST(UTEST_DefaultDBMerger, sorts_the_destination_by_full_path)
{
  DefaultDBMerger merger;

  ImageRecordsSet dst;
  ImageRecordsSet append{ir("c.png", "/d"), ir("a.png", "/d"),
                         ir("b.png", "/d")};

  merger.merge(dst, append);

  ASSERT_EQ(dst.size(), 3U);
  EXPECT_EQ(dst[0]->get_full_path(), "/d/a.png");
  EXPECT_EQ(dst[1]->get_full_path(), "/d/b.png");
  EXPECT_EQ(dst[2]->get_full_path(), "/d/c.png");
}

TEST(UTEST_DefaultDBMerger, merging_an_empty_append_keeps_destination)
{
  DefaultDBMerger merger;

  ImageRecordsSet dst{ir("a.png", "/d")};
  ImageRecordsSet append;

  merger.merge(dst, append);

  ASSERT_EQ(dst.size(), 1U);
  EXPECT_EQ(dst[0]->get_full_path(), "/d/a.png");
}
