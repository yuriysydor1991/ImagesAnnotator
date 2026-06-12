#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "src/annotator-events/ImagesAnnotatorEventController.h"
#include "src/annotator-events/events/DisplayErrorEvent.h"
#include "src/annotator-events/events/DisplayErrorEventHandler.h"
#include "src/annotator-events/events/EventsFactory.h"
#include "src/annotator-events/events/ImagesDirChanged.h"
#include "src/annotator-events/events/ImagesDirChangedIHandler.h"

using namespace testing;
using events::ImagesAnnotatorEventController;

namespace
{

class FakeErrorHandler : public events::events::DisplayErrorEventHandler
{
 public:
  int calls{0};
  std::string last;

  void handle(events::events::DisplayErrorEventPtr event) override
  {
    ++calls;
    last = event->description;
  }
};

class FakeImagesDirHandler : public events::events::ImagesDirChangedIHandler
{
 public:
  events::events::ImagesDirChangedPtr received;

  void handle(events::events::ImagesDirChangedPtr event) override
  {
    received = event;
  }
};

std::shared_ptr<ImagesAnnotatorEventController> make_controller()
{
  // The controller only stores the application context; the dispatch paths
  // exercised here do not touch it, so a null context is enough.
  return std::make_shared<ImagesAnnotatorEventController>(nullptr);
}

}  // namespace

TEST(UTEST_ImagesAnnotatorEventController, get_events_factory_returns_the_singleton)
{
  auto eventer = make_controller();

  auto f = eventer->get_events_factory();

  ASSERT_NE(f, nullptr);
  EXPECT_EQ(f.get(), events::events::EventsFactory::instance().get());
}

TEST(UTEST_ImagesAnnotatorEventController, submit_dispatches_to_subscribed_handlers)
{
  auto eventer = make_controller();
  auto handler = std::make_shared<FakeErrorHandler>();

  eventer->subscribe(handler);
  eventer->submit(events::events::DisplayErrorEvent::create("boom"));

  EXPECT_EQ(handler->calls, 1);
  EXPECT_EQ(handler->last, "boom");
}

TEST(UTEST_ImagesAnnotatorEventController, submit_without_subscribers_is_a_noop)
{
  auto eventer = make_controller();

  eventer->submit(events::events::DisplayErrorEvent::create("nobody listens"));

  SUCCEED();
}

TEST(UTEST_ImagesAnnotatorEventController,
     on_images_dir_changed_dispatches_an_event_with_the_path)
{
  auto eventer = make_controller();
  auto handler = std::make_shared<FakeImagesDirHandler>();

  eventer->subscribe(handler);
  eventer->onImagesDirChanged("/imgs");

  ASSERT_NE(handler->received, nullptr);
  EXPECT_EQ(handler->received->images_dir, "/imgs");
}
