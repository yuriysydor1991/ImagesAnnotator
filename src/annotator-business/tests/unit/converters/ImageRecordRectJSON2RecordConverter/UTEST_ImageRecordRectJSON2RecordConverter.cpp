#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <nlohmann/json.hpp>

#include <memory>
#include <string>

#include "src/annotator-business/dbs/AnnotationsDBs/AnnotationsJSONSerializator.h"
#include "src/annotator-business/dbs/AnnotationsDBs/converters/ImageRecordRectJSON2RecordConverter.h"

using namespace testing;
using iannotator::dbs::annotations::converters::ImageRecordRectJSON2RecordConverter;
using S = iannotator::dbs::annotations::AnnotationsJSONSerializator;

namespace
{

nlohmann::json rect_json(const std::string& name, int x, int y, int w, int h)
{
  nlohmann::json rect;
  rect[S::annotation_rect_name] = name;
  rect[S::annotation_rect][S::annotation_rect_x] = x;
  rect[S::annotation_rect][S::annotation_rect_y] = y;
  rect[S::annotation_rect][S::annotation_rect_width] = w;
  rect[S::annotation_rect][S::annotation_rect_height] = h;
  return rect;
}

}  // namespace

TEST(UTEST_ImageRecordRectJSON2RecordConverter, converts_a_rect_json_to_a_record)
{
  ImageRecordRectJSON2RecordConverter conv;

  auto r = conv.convert(rect_json("cat", 1, 2, 3, 4));

  ASSERT_NE(r, nullptr);
  EXPECT_EQ(r->name, "cat");
  EXPECT_EQ(r->x, 1);
  EXPECT_EQ(r->y, 2);
  EXPECT_EQ(r->width, 3);
  EXPECT_EQ(r->height, 4);
}

TEST(UTEST_ImageRecordRectJSON2RecordConverter, preserves_negative_geometry)
{
  ImageRecordRectJSON2RecordConverter conv;

  auto r = conv.convert(rect_json("dog", -5, -6, 7, 8));

  ASSERT_NE(r, nullptr);
  EXPECT_EQ(r->name, "dog");
  EXPECT_EQ(r->x, -5);
  EXPECT_EQ(r->y, -6);
  EXPECT_EQ(r->width, 7);
  EXPECT_EQ(r->height, 8);
}
