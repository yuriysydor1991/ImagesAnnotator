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
#include "src/annotator-events/events/ImportPlainTxt2FolderRequestHandler.h"
#include "src/annotator-events/events/ImportPyTorchVisionRequestHandler.h"
#include "src/annotator-events/events/ImportYolo4FolderRequestHandler.h"

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

/**
 * @brief Records the import request it is handed, so that a submit can be
 * followed all the way to a subscriber.
 */
template <class HandlerT, class EventPtrT>
class FakeImportHandler : public HandlerT
{
 public:
  EventPtrT received;

  void handle(EventPtrT event) override { received = event; }
};

using FakePlainTxtImportHandler =
    FakeImportHandler<events::events::ImportPlainTxt2FolderRequestHandler,
                      events::events::ImportPlainTxt2FolderRequestPtr>;
using FakeYolo4ImportHandler =
    FakeImportHandler<events::events::ImportYolo4FolderRequestHandler,
                      events::events::ImportYolo4FolderRequestPtr>;
using FakeUltralyticsDetectImportHandler =
    FakeImportHandler<events::events::ImportUltralyticsDetectRequestHandler,
                      events::events::ImportUltralyticsDetectRequestPtr>;
using FakeUltralyticsObbImportHandler =
    FakeImportHandler<events::events::ImportUltralyticsObbRequestHandler,
                      events::events::ImportUltralyticsObbRequestPtr>;
using FakeUltralyticsSegmentImportHandler =
    FakeImportHandler<events::events::ImportUltralyticsSegmentRequestHandler,
                      events::events::ImportUltralyticsSegmentRequestPtr>;
using FakeCocoImportHandler =
    FakeImportHandler<events::events::ImportCocoRequestHandler,
                      events::events::ImportCocoRequestPtr>;
using FakePascalVocImportHandler =
    FakeImportHandler<events::events::ImportPascalVocRequestHandler,
                      events::events::ImportPascalVocRequestPtr>;
using FakeCreateMLImportHandler =
    FakeImportHandler<events::events::ImportCreateMLRequestHandler,
                      events::events::ImportCreateMLRequestPtr>;
using FakePyTorchImportHandler =
    FakeImportHandler<events::events::ImportPyTorchVisionRequestHandler,
                      events::events::ImportPyTorchVisionRequestPtr>;

std::shared_ptr<ImagesAnnotatorEventController> make_controller()
{
  // The controller only stores the application context; the dispatch paths
  // exercised here do not touch it, so a null context is enough.
  return std::make_shared<ImagesAnnotatorEventController>(nullptr);
}

}  // namespace

TEST(UTEST_ImagesAnnotatorEventController,
     get_events_factory_returns_the_singleton)
{
  auto eventer = make_controller();

  auto f = eventer->get_events_factory();

  ASSERT_NE(f, nullptr);
  EXPECT_EQ(f.get(), events::events::EventsFactory::instance().get());
}

TEST(UTEST_ImagesAnnotatorEventController,
     submit_dispatches_to_subscribed_handlers)
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

TEST(UTEST_ImagesAnnotatorEventController,
     a_plain_txt_import_request_reaches_its_subscriber)
{
  auto eventer = make_controller();
  auto handler = std::make_shared<FakePlainTxtImportHandler>();

  eventer->subscribe(handler);
  eventer->submit(
      eventer->get_events_factory()->create_plain_txt_import_request("/in"));

  ASSERT_NE(handler->received, nullptr);
  EXPECT_EQ(handler->received->src_folder_path, "/in");
}

TEST(UTEST_ImagesAnnotatorEventController,
     a_yolo4_import_request_reaches_its_subscriber)
{
  auto eventer = make_controller();
  auto handler = std::make_shared<FakeYolo4ImportHandler>();

  eventer->subscribe(handler);
  eventer->submit(
      eventer->get_events_factory()->create_yolo4_import_request("/in"));

  ASSERT_NE(handler->received, nullptr);
  EXPECT_EQ(handler->received->src_folder_path, "/in");
}

TEST(UTEST_ImagesAnnotatorEventController,
     a_ultralytics_detect_import_request_reaches_its_subscriber)
{
  auto eventer = make_controller();
  auto handler = std::make_shared<FakeUltralyticsDetectImportHandler>();

  eventer->subscribe(handler);
  eventer->submit(
      eventer->get_events_factory()->create_ultralytics_detect_import_request(
          "/in"));

  ASSERT_NE(handler->received, nullptr);
  EXPECT_EQ(handler->received->src_folder_path, "/in");
}

TEST(UTEST_ImagesAnnotatorEventController,
     a_ultralytics_obb_import_request_reaches_its_subscriber)
{
  auto eventer = make_controller();
  auto handler = std::make_shared<FakeUltralyticsObbImportHandler>();

  eventer->subscribe(handler);
  eventer->submit(
      eventer->get_events_factory()->create_ultralytics_obb_import_request(
          "/in"));

  ASSERT_NE(handler->received, nullptr);
  EXPECT_EQ(handler->received->src_folder_path, "/in");
}

TEST(UTEST_ImagesAnnotatorEventController,
     a_ultralytics_segment_import_request_reaches_its_subscriber)
{
  auto eventer = make_controller();
  auto handler = std::make_shared<FakeUltralyticsSegmentImportHandler>();

  eventer->subscribe(handler);
  eventer->submit(
      eventer->get_events_factory()->create_ultralytics_segment_import_request(
          "/in"));

  ASSERT_NE(handler->received, nullptr);
  EXPECT_EQ(handler->received->src_folder_path, "/in");
}

TEST(UTEST_ImagesAnnotatorEventController,
     a_coco_import_request_reaches_its_subscriber)
{
  auto eventer = make_controller();
  auto handler = std::make_shared<FakeCocoImportHandler>();

  eventer->subscribe(handler);
  eventer->submit(
      eventer->get_events_factory()->create_coco_import_request("/in"));

  ASSERT_NE(handler->received, nullptr);
  EXPECT_EQ(handler->received->src_folder_path, "/in");
}

TEST(UTEST_ImagesAnnotatorEventController,
     a_pascal_voc_import_request_reaches_its_subscriber)
{
  auto eventer = make_controller();
  auto handler = std::make_shared<FakePascalVocImportHandler>();

  eventer->subscribe(handler);
  eventer->submit(
      eventer->get_events_factory()->create_pascal_voc_import_request("/in"));

  ASSERT_NE(handler->received, nullptr);
  EXPECT_EQ(handler->received->src_folder_path, "/in");
}

TEST(UTEST_ImagesAnnotatorEventController,
     a_createml_import_request_reaches_its_subscriber)
{
  auto eventer = make_controller();
  auto handler = std::make_shared<FakeCreateMLImportHandler>();

  eventer->subscribe(handler);
  eventer->submit(
      eventer->get_events_factory()->create_createml_import_request("/in"));

  ASSERT_NE(handler->received, nullptr);
  EXPECT_EQ(handler->received->src_folder_path, "/in");
}

TEST(UTEST_ImagesAnnotatorEventController,
     a_pytorch_vision_import_request_reaches_its_subscriber)
{
  auto eventer = make_controller();
  auto handler = std::make_shared<FakePyTorchImportHandler>();

  eventer->subscribe(handler);
  eventer->submit(
      eventer->get_events_factory()->create_pytorch_vision_import_request(
          "/in"));

  ASSERT_NE(handler->received, nullptr);
  EXPECT_EQ(handler->received->src_folder_path, "/in");
}

// Each layout keeps its own subscriber queue, so a handler of one import never
// sees the request of another.
TEST(UTEST_ImagesAnnotatorEventController,
     an_import_request_reaches_no_other_layout_subscriber)
{
  auto eventer = make_controller();
  auto plainTxt = std::make_shared<FakePlainTxtImportHandler>();
  auto yolo4 = std::make_shared<FakeYolo4ImportHandler>();

  eventer->subscribe(plainTxt);
  eventer->subscribe(yolo4);

  eventer->submit(
      eventer->get_events_factory()->create_yolo4_import_request("/in"));

  EXPECT_EQ(plainTxt->received, nullptr);
  EXPECT_NE(yolo4->received, nullptr);
}
