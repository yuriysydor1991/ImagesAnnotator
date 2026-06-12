#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <nlohmann/json.hpp>

#include <string>

#include "src/annotator-business/dbs/AnnotationsDBs/AnnotationsJSONSerializator.h"

using namespace testing;
using S = iannotator::dbs::annotations::AnnotationsJSONSerializator;

namespace
{

// Exposes the protected validation helpers for focused unit testing.
class TestableSerializer : public iannotator::dbs::annotations::
                               AnnotationsJSONSerializator
{
 public:
  using AnnotationsJSONSerializator::serialize_annotations_record;
  using AnnotationsJSONSerializator::serialize_app_info;
};

nlohmann::json valid_record(const std::string& absdir)
{
  nlohmann::json rec;
  rec[S::annotation_db_field_name][S::annotations_db_path_field_name] = absdir;
  rec[S::annotations_field_name] = nlohmann::json::array();
  return rec;
}

nlohmann::json valid_db()
{
  nlohmann::json j;
  j[S::root_app_name][S::file_version_field_name] = "1.0.0";
  j[S::annotations_field_name] = nlohmann::json::array();
  j[S::annotations_field_name].push_back(valid_record("/abs/dir"));
  return j;
}

}  // namespace

TEST(UTEST_AnnotationsJSONSerializator, serialize_accepts_a_valid_db)
{
  TestableSerializer s;

  EXPECT_TRUE(s.serialize(valid_db()));
}

TEST(UTEST_AnnotationsJSONSerializator, app_info_requires_the_app_root_element)
{
  TestableSerializer s;

  nlohmann::json j;
  j[S::annotations_field_name] = nlohmann::json::array();

  EXPECT_FALSE(s.serialize_app_info(j));
}

TEST(UTEST_AnnotationsJSONSerializator, app_info_requires_the_annotations_field)
{
  TestableSerializer s;

  nlohmann::json j;
  j[S::root_app_name][S::file_version_field_name] = "1.0.0";

  EXPECT_FALSE(s.serialize_app_info(j));
}

TEST(UTEST_AnnotationsJSONSerializator, record_requires_the_db_element)
{
  TestableSerializer s;

  nlohmann::json rec;
  rec[S::annotations_field_name] = nlohmann::json::array();

  EXPECT_FALSE(s.serialize_annotations_record(rec));
}

TEST(UTEST_AnnotationsJSONSerializator, record_rejects_an_empty_db_path)
{
  TestableSerializer s;

  EXPECT_FALSE(s.serialize_annotations_record(valid_record("")));
}

TEST(UTEST_AnnotationsJSONSerializator, record_accepts_a_valid_record)
{
  TestableSerializer s;

  EXPECT_TRUE(s.serialize_annotations_record(valid_record("/abs/dir")));
}

TEST(UTEST_AnnotationsJSONSerializator, serialize_rejects_an_invalid_inner_record)
{
  TestableSerializer s;

  auto j = valid_db();
  j[S::annotations_field_name][0] = nlohmann::json::object();

  EXPECT_FALSE(s.serialize(j));
}
