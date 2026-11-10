#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

#include "QtInitFixture.h"
#include "src/annotator-events/events/ImageRecord.h"
#include "src/annotator-events/events/ImageRecordRect.h"
#include "src/annotator-events/events/ImageRecordSegment.h"
#include "src/gui/qt6/main-window/custom-widgets/CentralWorkingCanvas.h"
#include "src/gui/qt6/main-window/custom-widgets/ImagePathLabel.h"
#include "src/gui/qt6/qt_includes.h"

using namespace testing;
using events::events::ImageRecord;
using events::events::ImageRecordRect;
using events::events::ImageRecordSegment;
using events::events::ImageRecordSegmentPtr;
namespace cw = imagesAnnotatorQt6::window::custom_widgets;

namespace
{

/// @brief A 100x100 rectangle at (10,10) on an unscaled image, the fixture the
/// hit-testing cases below probe.
constexpr int rectX = 10;
constexpr int rectY = 10;
constexpr int rectSide = 100;

/// @brief Matches CentralWorkingCanvas::cornersPixelSize, which is private.
constexpr int corner = 10;

std::shared_ptr<cw::ImagePathLabel> make_annotated_label()
{
  auto ir = ImageRecord::create("a.png", "/d");
  ir->imageScale = 1.0;
  ir->rects.emplace_back(std::make_shared<ImageRecordRect>(
      "annotation", rectX, rectY, rectSide, rectSide));

  return std::make_shared<cw::ImagePathLabel>(ir);
}

/// @brief A triangle segment of the (20,20), (80,20) and (50,70) points.
ImageRecordSegmentPtr make_triangle()
{
  auto segment = std::make_shared<ImageRecordSegment>();

  segment->add_point(20.0, 20.0);
  segment->add_point(80.0, 20.0);
  segment->add_point(50.0, 70.0);

  return segment;
}

std::shared_ptr<cw::ImagePathLabel> make_segmented_label(
    const ImageRecordSegmentPtr& segment)
{
  auto ir = ImageRecord::create("a.png", "/d");
  ir->imageScale = 1.0;
  ir->rects.emplace_back(segment);

  return std::make_shared<cw::ImagePathLabel>(ir);
}

}  // namespace

class CTEST_Qt6CentralWorkingCanvas : public QtInitFixture
{
};

TEST_F(CTEST_Qt6CentralWorkingCanvas, set_pixmap_installs_and_sizes_the_image)
{
  std::shared_ptr<cw::ImagePathLabel> current_image;
  QWidget parent;

  auto* canvas = new cw::CentralWorkingCanvas(&parent, current_image);

  QPixmap pm{20, 10};
  pm.fill(Qt::black);

  EXPECT_TRUE(canvas->set_pixmap(pm));
  EXPECT_FALSE(canvas->get_pixmap().isNull());
  EXPECT_EQ(canvas->get_pixmap().width(), 20);
  EXPECT_EQ(canvas->get_pixmap().height(), 10);

  // The canvas sizes itself to the image so the enclosing QScrollArea scrolls
  // it and its widget coordinates stay equal to the image ones.
  EXPECT_EQ(canvas->size(), QSize(20, 10));
}

TEST_F(CTEST_Qt6CentralWorkingCanvas, rejects_a_null_pixmap)
{
  std::shared_ptr<cw::ImagePathLabel> current_image;
  QWidget parent;

  auto* canvas = new cw::CentralWorkingCanvas(&parent, current_image);

  EXPECT_FALSE(canvas->set_pixmap(QPixmap{}));
  EXPECT_TRUE(canvas->get_pixmap().isNull());
}

TEST_F(CTEST_Qt6CentralWorkingCanvas, clear_drops_the_image)
{
  std::shared_ptr<cw::ImagePathLabel> current_image;
  QWidget parent;

  auto* canvas = new cw::CentralWorkingCanvas(&parent, current_image);

  QPixmap pm{20, 10};
  pm.fill(Qt::black);

  ASSERT_TRUE(canvas->set_pixmap(pm));

  canvas->clear();

  EXPECT_TRUE(canvas->get_pixmap().isNull());
}

TEST_F(CTEST_Qt6CentralWorkingCanvas, reports_no_resize_without_an_image)
{
  std::shared_ptr<cw::ImagePathLabel> current_image;
  QWidget parent;

  auto* canvas = new cw::CentralWorkingCanvas(&parent, current_image);

  EXPECT_FALSE(canvas->mouse_is_over_resize(5, 5));
  EXPECT_EQ(canvas->get_mouse_over_rect(), nullptr);
}

TEST_F(CTEST_Qt6CentralWorkingCanvas, detects_the_upper_left_resize_corner)
{
  auto current_image = make_annotated_label();
  QWidget parent;

  auto* canvas = new cw::CentralWorkingCanvas(&parent, current_image);

  EXPECT_TRUE(canvas->mouse_is_over_resize(rectX + 1, rectY + 1));
  EXPECT_TRUE(canvas->is_over_up_left());
  EXPECT_FALSE(canvas->is_over_down_right());
  EXPECT_EQ(canvas->get_mouse_over_rect(),
            current_image->get_image_rec()->rects.front());
}

TEST_F(CTEST_Qt6CentralWorkingCanvas, detects_the_lower_right_resize_corner)
{
  auto current_image = make_annotated_label();
  QWidget parent;

  auto* canvas = new cw::CentralWorkingCanvas(&parent, current_image);

  EXPECT_TRUE(
      canvas->mouse_is_over_resize(rectX + rectSide - 1, rectY + rectSide - 1));
  EXPECT_TRUE(canvas->is_over_down_right());
  EXPECT_FALSE(canvas->is_over_up_left());
}

TEST_F(CTEST_Qt6CentralWorkingCanvas, the_rectangle_middle_is_not_a_resize_area)
{
  auto current_image = make_annotated_label();
  QWidget parent;

  auto* canvas = new cw::CentralWorkingCanvas(&parent, current_image);

  // Well inside the rectangle but away from every corner handle.
  EXPECT_FALSE(
      canvas->mouse_is_over_resize(rectX + rectSide / 2, rectY + rectSide / 2));
}

TEST_F(CTEST_Qt6CentralWorkingCanvas, honours_the_image_scale_when_hit_testing)
{
  auto current_image = make_annotated_label();
  current_image->get_image_rec()->imageScale = 2.0;

  QWidget parent;

  auto* canvas = new cw::CentralWorkingCanvas(&parent, current_image);

  // At the unscaled coordinates the corner has moved away...
  EXPECT_FALSE(canvas->mouse_is_over_resize(rectX + 1, rectY + 1));

  // ...and now sits at twice the distance from the origin.
  EXPECT_TRUE(canvas->mouse_is_over_resize(rectX * 2 + corner / 2,
                                           rectY * 2 + corner / 2));
  EXPECT_TRUE(canvas->is_over_up_left());
}

TEST_F(CTEST_Qt6CentralWorkingCanvas, reports_no_point_without_an_image)
{
  std::shared_ptr<cw::ImagePathLabel> current_image;
  QWidget parent;

  auto* canvas = new cw::CentralWorkingCanvas(&parent, current_image);

  EXPECT_FALSE(canvas->mouse_is_over_point(20, 20));
  EXPECT_EQ(canvas->get_mouse_over_segment(), nullptr);
}

TEST_F(CTEST_Qt6CentralWorkingCanvas, a_plain_rectangle_has_no_points)
{
  auto current_image = make_annotated_label();
  QWidget parent;

  auto* canvas = new cw::CentralWorkingCanvas(&parent, current_image);

  EXPECT_FALSE(canvas->mouse_is_over_point(rectX, rectY));
}

TEST_F(CTEST_Qt6CentralWorkingCanvas, detects_the_segment_point_under_the_mouse)
{
  auto segment = make_triangle();
  auto current_image = make_segmented_label(segment);
  QWidget parent;

  auto* canvas = new cw::CentralWorkingCanvas(&parent, current_image);

  EXPECT_TRUE(canvas->mouse_is_over_point(81, 22));
  EXPECT_EQ(canvas->get_mouse_over_segment(), segment);
  EXPECT_EQ(canvas->get_mouse_over_point(), 1U);

  // Inside the polygon but away from every point of it.
  EXPECT_FALSE(canvas->mouse_is_over_point(50, 35));
  EXPECT_EQ(canvas->get_mouse_over_segment(), nullptr);
}

TEST_F(CTEST_Qt6CentralWorkingCanvas, honours_the_image_scale_for_the_points)
{
  auto segment = make_triangle();
  auto current_image = make_segmented_label(segment);
  current_image->get_image_rec()->imageScale = 2.0;

  QWidget parent;

  auto* canvas = new cw::CentralWorkingCanvas(&parent, current_image);

  EXPECT_FALSE(canvas->mouse_is_over_point(50, 70));
  EXPECT_TRUE(canvas->mouse_is_over_point(100, 140));
  EXPECT_EQ(canvas->get_mouse_over_point(), 2U);
}

TEST_F(CTEST_Qt6CentralWorkingCanvas, prefers_the_point_of_the_current_segment)
{
  auto below = make_triangle();
  auto above = make_triangle();
  auto current_image = make_segmented_label(below);
  auto ir = current_image->get_image_rec();

  ir->rects.emplace_back(above);

  QWidget parent;

  auto* canvas = new cw::CentralWorkingCanvas(&parent, current_image);

  // The topmost drawn segment wins without a current annotation...
  EXPECT_TRUE(canvas->mouse_is_over_point(20, 20));
  EXPECT_EQ(canvas->get_mouse_over_segment(), above);

  // ...and the current annotation wins over it.
  ir->current_rect = below;

  EXPECT_TRUE(canvas->mouse_is_over_point(20, 20));
  EXPECT_EQ(canvas->get_mouse_over_segment(), below);
}

TEST_F(CTEST_Qt6CentralWorkingCanvas, the_drawn_segment_points_are_not_dragged)
{
  auto segment = make_triangle();
  auto current_image = make_segmented_label(segment);
  QWidget parent;

  auto* canvas = new cw::CentralWorkingCanvas(&parent, current_image);

  canvas->set_drawn_segment(segment);

  EXPECT_EQ(canvas->get_drawn_segment(), segment);
  EXPECT_FALSE(canvas->mouse_is_over_point(20, 20));

  canvas->set_drawn_segment(nullptr);

  EXPECT_TRUE(canvas->mouse_is_over_point(20, 20));
}

TEST_F(CTEST_Qt6CentralWorkingCanvas, paints_the_segments_being_drawn_and_done)
{
  auto drawn = std::make_shared<ImageRecordSegment>();
  drawn->add_point(5.0, 5.0);
  drawn->add_point(15.0, 5.0);

  auto current_image = make_segmented_label(make_triangle());
  current_image->get_image_rec()->rects.emplace_back(drawn);

  QWidget parent;

  auto* canvas = new cw::CentralWorkingCanvas(&parent, current_image);

  QPixmap pm{100, 100};
  pm.fill(Qt::white);

  ASSERT_TRUE(canvas->set_pixmap(pm));

  canvas->set_drawn_segment(drawn);
  ASSERT_TRUE(canvas->mouse_is_over_point(20, 20));

  // Renders the open path of the drawn segment, the closed polygon and the
  // point handles without a display to show them on.
  QPixmap target{canvas->size()};
  canvas->render(&target);

  EXPECT_FALSE(target.isNull());
}
