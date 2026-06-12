#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <nlohmann/json.hpp>

#include <memory>
#include <string>

#include "src/annotator-business/dbs/AnnotationsDBs/AnnotationsJSONSerializator.h"
#include "src/annotator-business/dbs/AnnotationsDBs/converters/ImageRecordJSON2RecordConverter.h"

using namespace testing;
using iannotator::dbs::annotations::converters::ImageRecordJSON2RecordConverter;
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

nlohmann::json annotation_json(const std::string& relPath)
{
  nlohmann::json fan;
  fan[S::annotation_recerod_rel_path_field] = relPath;
  fan[S::annotation_image_scale_factor] = 1.5;
  fan[S::annotation_image_width] = 10;
  fan[S::annotation_image_height] = 20;
  fan[S::annotations_field_name] = nlohmann::json::array();
  return fan;
}

}  // namespace

TEST(UTEST_ImageRecordJSON2RecordConverter, converts_record_metadata)
{
  ImageRecordJSON2RecordConverter conv;

  auto ir = conv.convert(annotation_json("img.png"), "/abs/dir");

  ASSERT_NE(ir, nullptr);
  EXPECT_EQ(ir->path, "img.png");
  EXPECT_EQ(ir->abs_dir_path, "/abs/dir");
  EXPECT_DOUBLE_EQ(ir->imageScale, 1.5);
  EXPECT_EQ(ir->iwidth, 10);
  EXPECT_EQ(ir->iheight, 20);
  EXPECT_TRUE(ir->rects.empty());
}

TEST(UTEST_ImageRecordJSON2RecordConverter, converts_nested_rectangles)
{
  ImageRecordJSON2RecordConverter conv;

  auto fan = annotation_json("img.png");
  fan[S::annotations_field_name].push_back(rect_json("cat", 1, 2, 3, 4));
  fan[S::annotations_field_name].push_back(rect_json("dog", 5, 6, 7, 8));

  auto ir = conv.convert(fan, "/abs/dir");

  ASSERT_NE(ir, nullptr);
  ASSERT_EQ(ir->rects.size(), 2U);
  EXPECT_EQ(ir->rects[0]->name, "cat");
  EXPECT_EQ(ir->rects[0]->x, 1);
  EXPECT_EQ(ir->rects[1]->name, "dog");
  EXPECT_EQ(ir->rects[1]->height, 8);
}
