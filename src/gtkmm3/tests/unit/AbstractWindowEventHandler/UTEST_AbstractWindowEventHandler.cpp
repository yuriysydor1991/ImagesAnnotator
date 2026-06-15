#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

#include "src/gtkmm3/MainWindowContext.h"
#include "src/gtkmm3/main-window/event-handlers/AbstractWindowEventHandler.h"
#include "src/gtkmm3/main-window/event-handlers/WindowOps.h"

using namespace testing;
using templateGtkmm3::MainWindowContext;
namespace eh = templateGtkmm3::window::event_handlers;

namespace
{

// Minimal concrete handler that exposes the protected infrastructure pointers
// captured by the abstract base constructor so they can be asserted.
class ProbeHandler : public eh::AbstractWindowEventHandler
{
 public:
  using eh::AbstractWindowEventHandler::AbstractWindowEventHandler;

  bool subscribe() override { return true; }

  WindowOpsPtr exposed_ops() const { return ops; }
  std::shared_ptr<MainWindowContext> exposed_mwctx() const { return mwctx; }
};

}  // namespace

TEST(UTEST_AbstractWindowEventHandler, null_ops_leaves_context_null)
{
  ProbeHandler handler{nullptr};

  EXPECT_EQ(handler.exposed_ops(), nullptr);
  EXPECT_EQ(handler.exposed_mwctx(), nullptr);
}

TEST(UTEST_AbstractWindowEventHandler, ops_with_null_context_leaves_context_null)
{
  auto ops = std::make_shared<eh::WindowOps>(nullptr);

  ProbeHandler handler{ops};

  EXPECT_EQ(handler.exposed_ops(), ops);
  EXPECT_EQ(handler.exposed_mwctx(), nullptr);
}

TEST(UTEST_AbstractWindowEventHandler, propagates_the_ops_context)
{
  auto mwctx = std::make_shared<MainWindowContext>();
  auto ops = std::make_shared<eh::WindowOps>(mwctx);

  ProbeHandler handler{ops};

  EXPECT_EQ(handler.exposed_ops(), ops);
  EXPECT_EQ(handler.exposed_mwctx(), mwctx);
}
