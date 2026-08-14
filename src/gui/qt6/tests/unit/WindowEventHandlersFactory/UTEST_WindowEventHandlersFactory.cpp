#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

#include "src/gui/qt6/MainWindowContext.h"
#include "src/gui/qt6/main-window/event-handlers/WindowEventHandlersFactory.h"
#include "src/gui/qt6/main-window/event-handlers/WindowOps.h"

using namespace testing;
using imagesAnnotatorQt6::MainWindowContext;
namespace eh = imagesAnnotatorQt6::window::event_handlers;

namespace
{

// Building the handlers never touches a Qt widget (the base ctor only stores
// the shared infrastructure pointer), so the factory can be exercised without a
// QApplication. A WindowOps wrapping an empty MainWindowContext is enough.
std::shared_ptr<eh::WindowOps> make_ops()
{
  return std::make_shared<eh::WindowOps>(std::make_shared<MainWindowContext>());
}

}  // namespace

TEST(UTEST_Qt6WindowEventHandlersFactory, creates_every_handler_non_null)
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

TEST(UTEST_Qt6WindowEventHandlersFactory, produces_independent_instances)
{
  eh::WindowEventHandlersFactory factory;
  auto ops = make_ops();

  auto first = factory.create_about_events_handler(ops);
  auto second = factory.create_about_events_handler(ops);

  EXPECT_NE(first, nullptr);
  EXPECT_NE(second, nullptr);
  EXPECT_NE(first, second);
}

TEST(UTEST_Qt6WindowEventHandlersFactory, tolerates_a_null_ops)
{
  eh::WindowEventHandlersFactory factory;

  // The handlers accept a null infrastructure object (the abstract base just
  // leaves its context null), so the factory must not crash on a null ops.
  EXPECT_NE(factory.create_keyboard_events_handler(nullptr), nullptr);
  EXPECT_NE(factory.create_about_events_handler(nullptr), nullptr);
  EXPECT_NE(factory.create_window_system_events_handler(nullptr), nullptr);
}
