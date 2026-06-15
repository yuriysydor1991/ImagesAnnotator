#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

#include "FakeWindowLoader.h"
#include "GtkInitFixture.h"
#include "src/gtkmm3/MainWindowContext.h"
#include "src/gtkmm3/main-window/custom-widgets/CentralWorkingCanvas.h"
#include "src/gtkmm3/main-window/custom-widgets/ImagePathLabel.h"
#include "src/gtkmm3/main-window/event-handlers/AboutEventsHandler.h"
#include "src/gtkmm3/main-window/event-handlers/AnnotationsEventsHandler.h"
#include "src/gtkmm3/main-window/event-handlers/ExportEventsHandler.h"
#include "src/gtkmm3/main-window/event-handlers/ImageListEventsHandler.h"
#include "src/gtkmm3/main-window/event-handlers/ImageNavigationEventsHandler.h"
#include "src/gtkmm3/main-window/event-handlers/KeyboardEventsHandler.h"
#include "src/gtkmm3/main-window/event-handlers/MouseDrawEventsHandler.h"
#include "src/gtkmm3/main-window/event-handlers/ProjectFileEventsHandler.h"
#include "src/gtkmm3/main-window/event-handlers/WebPageEventsHandler.h"
#include "src/gtkmm3/main-window/event-handlers/WindowOps.h"
#include "src/gtkmm3/main-window/event-handlers/WindowSystemEventsHandler.h"
#include "src/gtkmm3/main-window/event-handlers/ZoomEventsHandler.h"

using namespace testing;
using templateGtkmm3::MainWindowContext;
using templateGtkmm3::window::custom_widgets::CentralWorkingCanvas;
using templateGtkmm3::window::custom_widgets::ImagePathLabel;
namespace eh = templateGtkmm3::window::event_handlers;

/**
 * @brief Component tests for the decomposed window event handlers.
 *
 * Each handler's subscribe() connects its GTK signals to the widgets exposed by
 * the WindowLoader. The fixture provides a MainWindowContext backed by the
 * FakeWindowLoader test double (real widgets, no UI resource loading) so the
 * wiring can be exercised against genuine GTK signals. Self-skips headlessly.
 */
class CTEST_WindowEventHandlers : public GtkInitFixture
{
 protected:
  std::shared_ptr<MainWindowContext> mwctx;
  std::shared_ptr<eh::WindowOps> ops;

  void SetUp() override
  {
    GtkInitFixture::SetUp();  // GTEST_SKIP()s when no display is available

    // GTEST_SKIP() in the base SetUp only returns from the base method, so bail
    // out here too before touching any GTK widget on a headless agent.
    if (IsSkipped()) {
      return;
    }

    mwctx = std::make_shared<MainWindowContext>();
    mwctx->wloader = std::make_shared<FakeWindowLoader>();
    ops = std::make_shared<eh::WindowOps>(mwctx);
  }
};

TEST_F(CTEST_WindowEventHandlers, about_subscribe_returns_true)
{
  eh::AboutEventsHandler handler(ops);

  EXPECT_TRUE(handler.subscribe());
}

TEST_F(CTEST_WindowEventHandlers, keyboard_subscribe_returns_true)
{
  eh::KeyboardEventsHandler handler(ops);

  EXPECT_TRUE(handler.subscribe());
}

TEST_F(CTEST_WindowEventHandlers, zoom_subscribe_returns_true)
{
  eh::ZoomEventsHandler handler(ops);

  EXPECT_TRUE(handler.subscribe());
}

TEST_F(CTEST_WindowEventHandlers, image_navigation_subscribe_returns_true)
{
  eh::ImageNavigationEventsHandler handler(ops);

  EXPECT_TRUE(handler.subscribe());
}

TEST_F(CTEST_WindowEventHandlers, image_list_subscribe_returns_true)
{
  eh::ImageListEventsHandler handler(ops);

  EXPECT_TRUE(handler.subscribe());
}

TEST_F(CTEST_WindowEventHandlers, annotations_subscribe_returns_true)
{
  eh::AnnotationsEventsHandler handler(ops);

  EXPECT_TRUE(handler.subscribe());
}

TEST_F(CTEST_WindowEventHandlers, project_file_subscribe_returns_true)
{
  eh::ProjectFileEventsHandler handler(ops);

  EXPECT_TRUE(handler.subscribe());
}

TEST_F(CTEST_WindowEventHandlers, export_subscribe_returns_true)
{
  eh::ExportEventsHandler handler(ops);

  EXPECT_TRUE(handler.subscribe());
}

TEST_F(CTEST_WindowEventHandlers, web_page_subscribe_returns_true)
{
  eh::WebPageEventsHandler handler(ops);

  EXPECT_TRUE(handler.subscribe());
}

TEST_F(CTEST_WindowEventHandlers, window_system_subscribe_returns_true)
{
  eh::WindowSystemEventsHandler handler(ops);

  EXPECT_TRUE(handler.subscribe());
}

TEST_F(CTEST_WindowEventHandlers, mouse_draw_subscribe_returns_true)
{
  std::shared_ptr<ImagePathLabel> ci;
  mwctx->centralCanvas = std::make_shared<CentralWorkingCanvas>(ci);

  eh::MouseDrawEventsHandler handler(ops);

  EXPECT_TRUE(handler.subscribe());
}
