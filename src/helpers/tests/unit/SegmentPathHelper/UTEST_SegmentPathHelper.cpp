#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstddef>
#include <memory>
#include <tuple>

#include "src/annotator-events/events/ImageRecord.h"
#include "src/annotator-events/events/ImageRecordRect.h"
#include "src/annotator-events/events/ImageRecordSegment.h"
#include "src/helpers/SegmentPathHelper.h"

using namespace testing;
using events::events::ImageRecord;
using events::events::ImageRecordRect;
using events::events::ImageRecordRectPtr;
using events::events::ImageRecordSegment;
using events::events::ImageRecordSegmentPtr;
using helpers::SegmentPathHelper;

namespace
{

using Point = SegmentPathHelper::Point;
using Points = SegmentPathHelper::Points;

constexpr double tolerance = 1e-9;

/// @brief A triangle drawn by three clicks: (10,10), (50,10) and (30,40).
ImageRecordSegmentPtr triangle()
{
  auto segment = std::make_shared<ImageRecordSegment>();

  segment->add_point(10.0, 10.0);
  segment->add_point(50.0, 10.0);
  segment->add_point(30.0, 40.0);

  return segment;
}

void expect_point(const Point& actual, const double& x, const double& y)
{
  EXPECT_NEAR(std::get<0>(actual), x, tolerance);
  EXPECT_NEAR(std::get<1>(actual), y, tolerance);
}

}  // namespace

TEST(UTEST_SegmentPathHelper, as_segment_tells_a_segment_from_a_rectangle)
{
  const ImageRecordRectPtr segment = std::make_shared<ImageRecordSegment>();
  const ImageRecordRectPtr rect =
      std::make_shared<ImageRecordRect>("rect", 1, 2, 3, 4);

  EXPECT_EQ(SegmentPathHelper::as_segment(segment), segment);
  EXPECT_EQ(SegmentPathHelper::as_segment(rect), nullptr);
  EXPECT_EQ(SegmentPathHelper::as_segment(nullptr), nullptr);
}

TEST(UTEST_SegmentPathHelper, image_points_of_an_empty_segment_are_empty)
{
  ImageRecordSegment segment;

  EXPECT_TRUE(SegmentPathHelper::image_points(segment).empty());
}

TEST(UTEST_SegmentPathHelper, image_points_report_the_open_path_too)
{
  ImageRecordSegment segment;

  segment.add_point(10.0, 10.0);
  segment.add_point(50.0, 30.0);

  // The outline() of the record reports the four rectangle corners for a path
  // this short, while the helper reports the very two clicked points.
  const auto image = SegmentPathHelper::image_points(segment);

  ASSERT_EQ(image.size(), 2U);
  expect_point(image[0], 10.0, 10.0);
  expect_point(image[1], 50.0, 30.0);
}

TEST(UTEST_SegmentPathHelper, image_points_follow_the_polygon_outline)
{
  auto segment = triangle();

  EXPECT_EQ(SegmentPathHelper::image_points(*segment), segment->outline());
}

TEST(UTEST_SegmentPathHelper, set_image_points_fits_the_bounding_box)
{
  ImageRecordSegment segment;

  SegmentPathHelper::set_image_points(
      segment, Points{{10.5, 20.0}, {30.0, 5.25}, {15.0, 40.0}});

  EXPECT_EQ(segment.x, 10);
  EXPECT_EQ(segment.y, 5);
  EXPECT_EQ(segment.width, 20);
  EXPECT_EQ(segment.height, 35);

  const auto image = SegmentPathHelper::image_points(segment);

  ASSERT_EQ(image.size(), 3U);
  expect_point(image[0], 10.5, 20.0);
  expect_point(image[1], 30.0, 5.25);
  expect_point(image[2], 15.0, 40.0);
}

TEST(UTEST_SegmentPathHelper, set_image_points_keeps_the_points_of_one_line)
{
  ImageRecordSegment segment;

  // A vertical line has no width to share its x of.
  SegmentPathHelper::set_image_points(segment, Points{{7.0, 3.0}, {7.0, 9.0}});

  EXPECT_EQ(segment.x, 7);
  EXPECT_EQ(segment.width, 0);

  const auto image = SegmentPathHelper::image_points(segment);

  ASSERT_EQ(image.size(), 2U);
  expect_point(image[0], 7.0, 3.0);
  expect_point(image[1], 7.0, 9.0);
}

TEST(UTEST_SegmentPathHelper, set_image_points_of_no_points_clears_the_path)
{
  auto segment = triangle();

  SegmentPathHelper::set_image_points(*segment, Points{});

  EXPECT_TRUE(segment->points.empty());
}

TEST(UTEST_SegmentPathHelper, move_point_moves_one_point_and_refits)
{
  auto segment = triangle();

  ASSERT_TRUE(SegmentPathHelper::move_point(*segment, 2U, 70.0, 60.0));

  const auto image = SegmentPathHelper::image_points(*segment);

  ASSERT_EQ(image.size(), 3U);
  expect_point(image[0], 10.0, 10.0);
  expect_point(image[1], 50.0, 10.0);
  expect_point(image[2], 70.0, 60.0);

  EXPECT_EQ(segment->x, 10);
  EXPECT_EQ(segment->y, 10);
  EXPECT_EQ(segment->width, 60);
  EXPECT_EQ(segment->height, 50);
}

TEST(UTEST_SegmentPathHelper, move_point_shrinks_the_box_too)
{
  auto segment = triangle();

  ASSERT_TRUE(SegmentPathHelper::move_point(*segment, 2U, 30.0, 20.0));

  EXPECT_EQ(segment->y, 10);
  EXPECT_EQ(segment->height, 10);
}

TEST(UTEST_SegmentPathHelper, move_point_out_of_the_path_changes_nothing)
{
  auto segment = triangle();
  const auto before = *segment;

  EXPECT_FALSE(SegmentPathHelper::move_point(*segment, 3U, 1.0, 1.0));

  EXPECT_EQ(segment->points, before.points);
  EXPECT_EQ(segment->x, before.x);
  EXPECT_EQ(segment->width, before.width);
}

TEST(UTEST_SegmentPathHelper, find_point_finds_the_point_near_the_position)
{
  auto segment = triangle();

  EXPECT_EQ(SegmentPathHelper::find_point(*segment, 51.0, 12.0, 1.0), 1U);
  EXPECT_EQ(SegmentPathHelper::find_point(*segment, 30.0, 40.0, 1.0), 2U);
}

TEST(UTEST_SegmentPathHelper, find_point_finds_nothing_far_away)
{
  auto segment = triangle();

  EXPECT_EQ(SegmentPathHelper::find_point(*segment, 30.0, 20.0, 1.0),
            SegmentPathHelper::npos);
  EXPECT_EQ(SegmentPathHelper::find_point(ImageRecordSegment{}, 0.0, 0.0, 1.0),
            SegmentPathHelper::npos);
}

TEST(UTEST_SegmentPathHelper, find_point_honours_the_image_scale)
{
  auto segment = triangle();

  // The (50,10) point is drawn at (100,20) on a twice enlarged image.
  EXPECT_EQ(SegmentPathHelper::find_point(*segment, 50.0, 10.0, 2.0),
            SegmentPathHelper::npos);
  EXPECT_EQ(SegmentPathHelper::find_point(*segment, 102.0, 21.0, 2.0), 1U);
}

TEST(UTEST_SegmentPathHelper, find_point_prefers_the_nearest_point)
{
  ImageRecordSegment segment;

  SegmentPathHelper::set_image_points(
      segment, Points{{10.0, 10.0}, {14.0, 10.0}, {12.0, 30.0}});

  EXPECT_EQ(SegmentPathHelper::find_point(segment, 11.0, 10.0, 1.0), 0U);
  EXPECT_EQ(SegmentPathHelper::find_point(segment, 13.5, 10.0, 1.0), 1U);
}

TEST(UTEST_SegmentPathHelper, find_point_honours_the_distance)
{
  auto segment = triangle();

  EXPECT_EQ(SegmentPathHelper::find_point(*segment, 20.0, 10.0, 1.0, 9.0),
            SegmentPathHelper::npos);
  EXPECT_EQ(SegmentPathHelper::find_point(*segment, 20.0, 10.0, 1.0, 10.0), 0U);
}

TEST(UTEST_SegmentPathHelper, to_image_point_unscales_the_canvas_position)
{
  expect_point(SegmentPathHelper::to_image_point(40.0, 30.0, 200.0, 100.0, 2.0),
               20.0, 15.0);
}

TEST(UTEST_SegmentPathHelper, to_image_point_clamps_to_the_image)
{
  expect_point(
      SegmentPathHelper::to_image_point(-5.0, 150.0, 200.0, 100.0, 2.0), 0.0,
      50.0);
  expect_point(
      SegmentPathHelper::to_image_point(250.0, -1.0, 200.0, 100.0, 0.5), 400.0,
      0.0);
}

TEST(UTEST_SegmentPathHelper, to_image_point_refuses_an_invalid_scale)
{
  expect_point(SegmentPathHelper::to_image_point(40.0, 30.0, 200.0, 100.0, 0.0),
               0.0, 0.0);
}

TEST(UTEST_SegmentPathHelper, is_closable_demands_the_least_points)
{
  ImageRecordSegment segment;

  segment.add_point(10.0, 10.0);
  segment.add_point(50.0, 10.0);

  EXPECT_FALSE(SegmentPathHelper::is_closable(segment));

  segment.add_point(30.0, 40.0);

  EXPECT_TRUE(SegmentPathHelper::is_closable(segment));
}

TEST(UTEST_SegmentPathHelper, finish_keeps_the_closable_segment)
{
  auto ir = ImageRecord::create("a.png", "/d");
  auto segment = triangle();

  ir->rects.emplace_back(segment);
  ir->current_rect = segment;

  EXPECT_TRUE(SegmentPathHelper::finish(ir, segment));

  ASSERT_EQ(ir->rects.size(), 1U);
  EXPECT_EQ(ir->rects.front(), segment);
  EXPECT_EQ(ir->current_rect, segment);
}

TEST(UTEST_SegmentPathHelper, finish_drops_the_too_short_path)
{
  auto ir = ImageRecord::create("a.png", "/d");
  auto rect = std::make_shared<ImageRecordRect>("rect", 1, 2, 3, 4);
  auto segment = std::make_shared<ImageRecordSegment>();

  segment->add_point(10.0, 10.0);
  segment->add_point(50.0, 10.0);

  ir->rects.emplace_back(rect);
  ir->rects.emplace_back(segment);
  ir->current_rect = segment;

  EXPECT_FALSE(SegmentPathHelper::finish(ir, segment));

  ASSERT_EQ(ir->rects.size(), 1U);
  EXPECT_EQ(ir->rects.front(), rect);
  EXPECT_EQ(ir->current_rect, nullptr);
}

TEST(UTEST_SegmentPathHelper, finish_keeps_another_current_annotation)
{
  auto ir = ImageRecord::create("a.png", "/d");
  auto rect = std::make_shared<ImageRecordRect>("rect", 1, 2, 3, 4);
  auto segment = std::make_shared<ImageRecordSegment>();

  segment->add_point(10.0, 10.0);

  ir->rects.emplace_back(segment);
  ir->rects.emplace_back(rect);
  ir->current_rect = rect;

  EXPECT_FALSE(SegmentPathHelper::finish(ir, segment));

  ASSERT_EQ(ir->rects.size(), 1U);
  EXPECT_EQ(ir->current_rect, rect);
}

TEST(UTEST_SegmentPathHelper, finish_of_no_segment_keeps_nothing)
{
  auto ir = ImageRecord::create("a.png", "/d");

  EXPECT_FALSE(SegmentPathHelper::finish(ir, nullptr));
}

TEST(UTEST_SegmentPathHelper, finish_without_an_image_record_tells_the_verdict)
{
  auto segment = std::make_shared<ImageRecordSegment>();

  segment->add_point(10.0, 10.0);

  EXPECT_TRUE(SegmentPathHelper::finish(nullptr, triangle()));
  EXPECT_FALSE(SegmentPathHelper::finish(nullptr, segment));
}
