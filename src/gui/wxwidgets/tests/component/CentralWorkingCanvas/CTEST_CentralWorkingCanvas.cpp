#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

#include "WxInitFixture.h"
#include "src/annotator-events/events/ImageRecord.h"
#include "src/annotator-events/events/ImageRecordRect.h"
#include "src/annotator-events/events/ImageRecordSegment.h"
#include "src/gui/wxwidgets/main-window/custom-widgets/CentralWorkingCanvas.h"
#include "src/gui/wxwidgets/main-window/custom-widgets/ImagePathLabel.h"
#include "src/gui/wxwidgets/wx_includes.h"

using namespace testing;
using events::events::ImageRecord;
using events::events::ImageRecordRect;
using events::events::ImageRecordSegment;
using events::events::ImageRecordSegmentPtr;
namespace cw = imagesAnnotatorWx::window::custom_widgets;

namespace
{

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

class CTEST_WxCentralWorkingCanvas : public WxInitFixture
{
};

TEST_F(CTEST_WxCentralWorkingCanvas, starts_without_a_bitmap)
{
  auto* frame = new wxFrame(nullptr, wxID_ANY, "t");
  std::shared_ptr<cw::ImagePathLabel> current_image;

  auto* canvas = new cw::CentralWorkingCanvas(frame, current_image);

  EXPECT_FALSE(canvas->get_bitmap().IsOk());

  frame->Destroy();
}

TEST_F(CTEST_WxCentralWorkingCanvas, set_bitmap_installs_the_image)
{
  auto* frame = new wxFrame(nullptr, wxID_ANY, "t");
  std::shared_ptr<cw::ImagePathLabel> current_image;

  auto* canvas = new cw::CentralWorkingCanvas(frame, current_image);

  wxImage img(20, 10);
  wxBitmap bmp(img);

  EXPECT_TRUE(canvas->set_bitmap(bmp));
  EXPECT_TRUE(canvas->get_bitmap().IsOk());
  EXPECT_EQ(canvas->get_bitmap().GetWidth(), 20);
  EXPECT_EQ(canvas->get_bitmap().GetHeight(), 10);

  frame->Destroy();
}

TEST_F(CTEST_WxCentralWorkingCanvas, reports_no_point_without_an_image)
{
  auto* frame = new wxFrame(nullptr, wxID_ANY, "t");
  std::shared_ptr<cw::ImagePathLabel> current_image;

  auto* canvas = new cw::CentralWorkingCanvas(frame, current_image);

  EXPECT_FALSE(canvas->mouse_is_over_point(20, 20));
  EXPECT_EQ(canvas->get_mouse_over_segment(), nullptr);

  frame->Destroy();
}

TEST_F(CTEST_WxCentralWorkingCanvas, a_plain_rectangle_has_no_points)
{
  auto* frame = new wxFrame(nullptr, wxID_ANY, "t");
  auto ir = ImageRecord::create("a.png", "/d");
  ir->imageScale = 1.0;
  ir->rects.emplace_back(
      std::make_shared<ImageRecordRect>("annotation", 10, 10, 100, 100));
  auto current_image = std::make_shared<cw::ImagePathLabel>(ir);

  auto* canvas = new cw::CentralWorkingCanvas(frame, current_image);

  EXPECT_FALSE(canvas->mouse_is_over_point(10, 10));

  frame->Destroy();
}

TEST_F(CTEST_WxCentralWorkingCanvas, detects_the_segment_point_under_the_mouse)
{
  auto* frame = new wxFrame(nullptr, wxID_ANY, "t");
  auto segment = make_triangle();
  auto current_image = make_segmented_label(segment);

  auto* canvas = new cw::CentralWorkingCanvas(frame, current_image);

  EXPECT_TRUE(canvas->mouse_is_over_point(81, 22));
  EXPECT_EQ(canvas->get_mouse_over_segment(), segment);
  EXPECT_EQ(canvas->get_mouse_over_point(), 1U);

  // Inside the polygon but away from every point of it.
  EXPECT_FALSE(canvas->mouse_is_over_point(50, 35));
  EXPECT_EQ(canvas->get_mouse_over_segment(), nullptr);

  frame->Destroy();
}

TEST_F(CTEST_WxCentralWorkingCanvas, honours_the_image_scale_for_the_points)
{
  auto* frame = new wxFrame(nullptr, wxID_ANY, "t");
  auto segment = make_triangle();
  auto current_image = make_segmented_label(segment);
  current_image->get_image_rec()->imageScale = 2.0;

  auto* canvas = new cw::CentralWorkingCanvas(frame, current_image);

  EXPECT_FALSE(canvas->mouse_is_over_point(50, 70));
  EXPECT_TRUE(canvas->mouse_is_over_point(100, 140));
  EXPECT_EQ(canvas->get_mouse_over_point(), 2U);

  frame->Destroy();
}

TEST_F(CTEST_WxCentralWorkingCanvas, prefers_the_point_of_the_current_segment)
{
  auto* frame = new wxFrame(nullptr, wxID_ANY, "t");
  auto below = make_triangle();
  auto above = make_triangle();
  auto current_image = make_segmented_label(below);
  auto ir = current_image->get_image_rec();

  ir->rects.emplace_back(above);

  auto* canvas = new cw::CentralWorkingCanvas(frame, current_image);

  // The topmost drawn segment wins without a current annotation...
  EXPECT_TRUE(canvas->mouse_is_over_point(20, 20));
  EXPECT_EQ(canvas->get_mouse_over_segment(), above);

  // ...and the current annotation wins over it.
  ir->current_rect = below;

  EXPECT_TRUE(canvas->mouse_is_over_point(20, 20));
  EXPECT_EQ(canvas->get_mouse_over_segment(), below);

  frame->Destroy();
}

TEST_F(CTEST_WxCentralWorkingCanvas, the_drawn_segment_points_are_not_dragged)
{
  auto* frame = new wxFrame(nullptr, wxID_ANY, "t");
  auto segment = make_triangle();
  auto current_image = make_segmented_label(segment);

  auto* canvas = new cw::CentralWorkingCanvas(frame, current_image);

  canvas->set_drawn_segment(segment);

  EXPECT_EQ(canvas->get_drawn_segment(), segment);
  EXPECT_FALSE(canvas->mouse_is_over_point(20, 20));

  canvas->set_drawn_segment(nullptr);

  EXPECT_TRUE(canvas->mouse_is_over_point(20, 20));

  frame->Destroy();
}
