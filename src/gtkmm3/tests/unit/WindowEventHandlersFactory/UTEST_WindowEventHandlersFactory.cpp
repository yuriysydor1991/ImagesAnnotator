#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

#include "src/gtkmm3/MainWindowContext.h"
#include "src/gtkmm3/main-window/event-handlers/AboutEventsHandler.h"
#include "src/gtkmm3/main-window/event-handlers/AnnotationsEventsHandler.h"
#include "src/gtkmm3/main-window/event-handlers/ExportEventsHandler.h"
#include "src/gtkmm3/main-window/event-handlers/ImageListEventsHandler.h"
#include "src/gtkmm3/main-window/event-handlers/ImageNavigationEventsHandler.h"
#include "src/gtkmm3/main-window/event-handlers/KeyboardEventsHandler.h"
#include "src/gtkmm3/main-window/event-handlers/MouseDrawEventsHandler.h"
#include "src/gtkmm3/main-window/event-handlers/ProjectFileEventsHandler.h"
#include "src/gtkmm3/main-window/event-handlers/WebPageEventsHandler.h"
#include "src/gtkmm3/main-window/event-handlers/WindowEventHandlersFactory.h"
#include "src/gtkmm3/main-window/event-handlers/WindowOps.h"
#include "src/gtkmm3/main-window/event-handlers/WindowSystemEventsHandler.h"
#include "src/gtkmm3/main-window/event-handlers/ZoomEventsHandler.h"

using namespace testing;
using templateGtkmm3::MainWindowContext;
namespace eh = templateGtkmm3::window::event_handlers;

namespace
{

// Building the handlers never touches GTK widgets (the base ctor only stores
// the shared infrastructure pointer), so the factory can be exercised without
// a display. A WindowOps wrapping an empty MainWindowContext is enough.
std::shared_ptr<eh::WindowOps> make_ops()
{
  return std::make_shared<eh::WindowOps>(std::make_shared<MainWindowContext>());
}

}  // namespace

TEST(UTEST_WindowEventHandlersFactory, creates_every_handler_non_null)
{
  eh::WindowEventHandlersFactory factory;
  auto ops = make_ops();

  EXPECT_NE(factory.create_mouse_events_handler(ops), nullptr);
  EXPECT_NE(factory.create_keyboard_events_handler(ops), nullptr);
  EXPECT_NE(factory.create_zoom_events_handler(ops), nullptr);
  EXPECT_NE(factory.create_image_navigation_events_handler(ops), nullptr);
  EXPECT_NE(factory.create_image_list_events_handler(ops), nullptr);
  EXPECT_NE(factory.create_annotations_events_handler(ops), nullptr);
  EXPECT_NE(factory.create_project_file_events_handler(ops), nullptr);
  EXPECT_NE(factory.create_export_events_handler(ops), nullptr);
  EXPECT_NE(factory.create_web_page_events_handler(ops), nullptr);
  EXPECT_NE(factory.create_about_events_handler(ops), nullptr);
  EXPECT_NE(factory.create_window_system_events_handler(ops), nullptr);
}

TEST(UTEST_WindowEventHandlersFactory, produces_independent_instances)
{
  eh::WindowEventHandlersFactory factory;
  auto ops = make_ops();

  auto first = factory.create_about_events_handler(ops);
  auto second = factory.create_about_events_handler(ops);

  EXPECT_NE(first, nullptr);
  EXPECT_NE(second, nullptr);
  EXPECT_NE(first, second);
}

TEST(UTEST_WindowEventHandlersFactory, tolerates_a_null_ops)
{
  eh::WindowEventHandlersFactory factory;

  // The handlers accept a null infrastructure object (the abstract base just
  // leaves its context null), so the factory must not crash on a null ops.
  EXPECT_NE(factory.create_keyboard_events_handler(nullptr), nullptr);
  EXPECT_NE(factory.create_about_events_handler(nullptr), nullptr);
  EXPECT_NE(factory.create_window_system_events_handler(nullptr), nullptr);
}
