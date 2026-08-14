#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

#include "WxInitFixture.h"
#include "src/gui/wxwidgets/main-window/custom-widgets/CentralWorkingCanvas.h"
#include "src/gui/wxwidgets/main-window/custom-widgets/ImagePathLabel.h"
#include "src/gui/wxwidgets/wx_includes.h"

using namespace testing;
namespace cw = imagesAnnotatorWx::window::custom_widgets;

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
