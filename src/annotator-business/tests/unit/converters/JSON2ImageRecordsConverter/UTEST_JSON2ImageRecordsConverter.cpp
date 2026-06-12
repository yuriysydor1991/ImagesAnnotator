#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <nlohmann/json.hpp>

#include <string>

#include "src/annotator-business/dbs/AnnotationsDBs/AnnotationsJSONSerializator.h"
#include "src/annotator-business/dbs/AnnotationsDBs/converters/JSON2ImageRecordsConverter.h"
#include "src/annotator-events/events/ImageRecord.h"

using namespace testing;
using events::events::ImageRecordsSet;
using iannotator::dbs::annotations::converters::JSON2ImageRecordsConverter;
using S = iannotator::dbs::annotations::AnnotationsJSONSerializator;

namespace
{

// Exposes the protected conversion methods for focused unit testing.
class TestableConverter : public JSON2ImageRecordsConverter
{
 public:
  using JSON2ImageRecordsConverter::convert_annotation;
  using JSON2ImageRecordsConverter::convert_folder;
};

nlohmann::json annotation_json(const std::string& relPath)
{
  nlohmann::json fan;
  fan[S::annotation_recerod_rel_path_field] = relPath;
  fan[S::annotation_image_scale_factor] = 1.0;
  fan[S::annotation_image_width] = 4;
  fan[S::annotation_image_height] = 5;
  fan[S::annotations_field_name] = nlohmann::json::array();
  return fan;
}

nlohmann::json folder_json(const std::string& absdir)
{
  nlohmann::json folder;
  folder[S::annotation_db_field_name][S::annotations_db_path_field_name] =
      absdir;
  folder[S::annotations_field_name] = nlohmann::json::array();
  return folder;
}

}  // namespace

TEST(UTEST_JSON2ImageRecordsConverter, convert_folder_fills_the_records_set)
{
  TestableConverter conv;

  auto folder = folder_json("/abs/dir");
  folder[S::annotations_field_name].push_back(annotation_json("a.png"));
  folder[S::annotations_field_name].push_back(annotation_json("b.png"));

  ImageRecordsSet rset;

  EXPECT_TRUE(conv.convert_folder(folder, rset));

  ASSERT_EQ(rset.size(), 2U);
  EXPECT_EQ(rset[0]->path, "a.png");
  EXPECT_EQ(rset[0]->abs_dir_path, "/abs/dir");
  EXPECT_EQ(rset[1]->path, "b.png");
}

TEST(UTEST_JSON2ImageRecordsConverter, convert_folder_appends_to_existing_set)
{
  TestableConverter conv;

  auto folder = folder_json("/abs/dir");
  folder[S::annotations_field_name].push_back(annotation_json("a.png"));

  ImageRecordsSet rset{events::events::ImageRecord::create("seed.png", "/s")};

  EXPECT_TRUE(conv.convert_folder(folder, rset));

  ASSERT_EQ(rset.size(), 2U);
  EXPECT_EQ(rset[0]->path, "seed.png");
  EXPECT_EQ(rset[1]->path, "a.png");
}

TEST(UTEST_JSON2ImageRecordsConverter, convert_folder_fails_on_empty_db_path)
{
  TestableConverter conv;

  auto folder = folder_json("");

  ImageRecordsSet rset;

  EXPECT_FALSE(conv.convert_folder(folder, rset));
  EXPECT_TRUE(rset.empty());
}

TEST(UTEST_JSON2ImageRecordsConverter, convert_annotation_builds_a_single_record)
{
  TestableConverter conv;

  auto ir = conv.convert_annotation(annotation_json("x.png"), "/d");

  ASSERT_NE(ir, nullptr);
  EXPECT_EQ(ir->path, "x.png");
  EXPECT_EQ(ir->abs_dir_path, "/d");
}
