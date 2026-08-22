#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <filesystem>
#include <memory>
#include <string>

#include "src/annotator-events/ImagesAnnotatorEventController.h"
#include "src/annotator-events/events/EventsFactory.h"
#include "src/app/ApplicationContext.h"
#include "src/app/converter/ApplicationFormatsConverter.h"

using namespace app;
using namespace app::converter;
using namespace events::events;
using namespace testing;

/**
 * @brief Drives the converter over a mocked event controller.
 *
 * What belongs here is this class' own share of a conversion: that the source
 * format spelling picks the import request event of that very layout, that the
 * destination one picks the matching export request, that both carry the paths
 * of the context, that the writing half runs only after the reading one
 * succeeded and that an error event of either half fails the conversion. The
 * reading and the writing themselves belong to the annotator controller and to
 * the two libraries behind it, and are covered by their own suites.
 */
class UTEST_ApplicationFormatsConverter : public Test
{
 public:
  /// @brief Never touched here: the mocked event controller hands the import
  /// request to nobody who would read it.
  inline static const std::string src_path{"/datasets/source"};

  /// @brief A directory which is already there, since the converter creates a
  /// destination that is not - and this suite is to write nothing.
  inline static const std::string dst_path{
      std::filesystem::current_path().string()};

  UTEST_ApplicationFormatsConverter() { reset_pipeline(); }

  /// @brief A converter of its own with an event controller of its own, so
  /// that a dispatch checked for one format cannot be what another one
  /// satisfies.
  void reset_pipeline()
  {
    converter = std::make_shared<ApplicationFormatsConverter>();
    appctx = std::make_shared<ApplicationContext>(argc, argv);
    eventer =
        std::make_shared<NiceMock<events::ImagesAnnotatorEventController>>(
            appctx);
    appctx->eventer = eventer;

    ON_CALL(*eventer, get_events_factory())
        .WillByDefault(Return(std::make_shared<EventsFactory>()));
  }

  void given_the_request(const std::string& from, const std::string& into)
  {
    appctx->convert_from = from;
    appctx->convert_src = src_path;
    appctx->convert_to = into;
    appctx->convert_dst = dst_path;
  }

  /**
   * @brief Expects the given source format spelling to submit the RequestT
   * import request event over the source path of the context.
   */
  template <class RequestT>
  void expect_import_of(const std::string& from)
  {
    reset_pipeline();
    given_the_request(from, "plain-txt");

    EXPECT_CALL(*eventer, submit(A<std::shared_ptr<RequestT>>())).Times(1);
    EXPECT_CALL(*eventer, submit(A<ExportPlainTxt2FolderRequestPtr>()))
        .Times(1);

    EXPECT_TRUE(converter->convert(appctx)) << from;
  }

  /**
   * @brief Expects the given destination format spelling to submit the
   * RequestT export request event over the destination path of the context.
   */
  template <class RequestT>
  void expect_export_of(const std::string& into)
  {
    reset_pipeline();
    given_the_request("plain-txt", into);

    EXPECT_CALL(*eventer, submit(A<ImportPlainTxt2FolderRequestPtr>()))
        .Times(1);
    EXPECT_CALL(*eventer, submit(A<std::shared_ptr<RequestT>>())).Times(1);

    EXPECT_TRUE(converter->convert(appctx)) << into;
  }

  int argc{0};
  char** argv{nullptr};

  std::shared_ptr<ApplicationFormatsConverter> converter;
  std::shared_ptr<ApplicationContext> appctx;
  std::shared_ptr<NiceMock<events::ImagesAnnotatorEventController>> eventer;
};

TEST_F(UTEST_ApplicationFormatsConverter, no_context_is_no_conversion)
{
  EXPECT_FALSE(converter->convert({}));
}

TEST_F(UTEST_ApplicationFormatsConverter, no_eventer_is_no_conversion)
{
  given_the_request("plain-txt", "coco");

  appctx->eventer.reset();

  EXPECT_FALSE(converter->convert(appctx));
}

TEST_F(UTEST_ApplicationFormatsConverter, the_errors_of_both_halves_are_heard)
{
  given_the_request("plain-txt", "coco");

  EXPECT_CALL(*eventer, subscribe(A<DisplayErrorEventHandlerPtr>())).Times(1);

  EXPECT_TRUE(converter->convert(appctx));
}

TEST_F(UTEST_ApplicationFormatsConverter, an_unknown_source_format_is_rejected)
{
  given_the_request("nosuch", "coco");

  EXPECT_CALL(*eventer, submit(A<ExportCocoRequestPtr>())).Times(0);

  EXPECT_FALSE(converter->convert(appctx));
}

TEST_F(UTEST_ApplicationFormatsConverter,
       an_unknown_destination_format_is_rejected)
{
  given_the_request("plain-txt", "nosuch");

  EXPECT_CALL(*eventer, submit(A<ImportPlainTxt2FolderRequestPtr>())).Times(0);

  EXPECT_FALSE(converter->convert(appctx));
}

TEST_F(UTEST_ApplicationFormatsConverter, both_halves_carry_the_context_paths)
{
  given_the_request("plain-txt", "coco");

  InSequence requestsOrder;

  EXPECT_CALL(*eventer, submit(A<ImportPlainTxt2FolderRequestPtr>()))
      .Times(1)
      .WillOnce(Invoke([](ImportPlainTxt2FolderRequestPtr event) {
        EXPECT_EQ(event->src_folder_path, src_path);
      }));

  EXPECT_CALL(*eventer, submit(A<ExportCocoRequestPtr>()))
      .Times(1)
      .WillOnce(Invoke([](ExportCocoRequestPtr event) {
        EXPECT_EQ(event->dst_folder_path, dst_path);
      }));

  EXPECT_TRUE(converter->convert(appctx));
}

TEST_F(UTEST_ApplicationFormatsConverter,
       an_error_of_the_reading_half_stops_the_conversion)
{
  given_the_request("plain-txt", "coco");

  EXPECT_CALL(*eventer, submit(A<ImportPlainTxt2FolderRequestPtr>()))
      .Times(1)
      .WillOnce(Invoke([&](ImportPlainTxt2FolderRequestPtr) {
        converter->handle(DisplayErrorEvent::create("no such directory"));
      }));

  EXPECT_CALL(*eventer, submit(A<ExportCocoRequestPtr>())).Times(0);

  EXPECT_FALSE(converter->convert(appctx));
}

TEST_F(UTEST_ApplicationFormatsConverter,
       an_error_of_the_writing_half_fails_the_conversion)
{
  given_the_request("plain-txt", "coco");

  EXPECT_CALL(*eventer, submit(A<ExportCocoRequestPtr>()))
      .Times(1)
      .WillOnce(Invoke([&](ExportCocoRequestPtr) {
        converter->handle(DisplayErrorEvent::create("read only destination"));
      }));

  EXPECT_FALSE(converter->convert(appctx));
}

TEST_F(UTEST_ApplicationFormatsConverter,
       an_error_of_a_previous_half_is_not_carried_over)
{
  given_the_request("plain-txt", "coco");

  converter->handle(DisplayErrorEvent::create("of something else"));

  EXPECT_TRUE(converter->convert(appctx));
}

TEST_F(UTEST_ApplicationFormatsConverter,
       every_source_format_submits_its_own_request)
{
  expect_import_of<AnnotationsDirChanged>("imagesannotator");
  expect_import_of<ImportPlainTxt2FolderRequest>("plain-txt");
  expect_import_of<ImportYolo4FolderRequest>("yolo4");
  expect_import_of<ImportUltralyticsDetectRequest>("ultralytics-detect");
  expect_import_of<ImportUltralyticsObbRequest>("ultralytics-obb");
  expect_import_of<ImportUltralyticsSegmentRequest>("ultralytics-segment");
  expect_import_of<ImportCocoRequest>("coco");
  expect_import_of<ImportPascalVocRequest>("pascal-voc");
  expect_import_of<ImportCreateMLRequest>("createml");
  expect_import_of<ImportPyTorchVisionRequest>("pytorch");
}

TEST_F(UTEST_ApplicationFormatsConverter,
       every_destination_format_submits_its_own_request)
{
  expect_export_of<StoreRequest>("imagesannotator");
  expect_export_of<ExportPlainTxt2FolderRequest>("plain-txt");
  expect_export_of<ExportYolo4FolderRequest>("yolo4");
  expect_export_of<ExportUltralyticsDetectRequest>("ultralytics-detect");
  expect_export_of<ExportUltralyticsObbRequest>("ultralytics-obb");
  expect_export_of<ExportUltralyticsSegmentRequest>("ultralytics-segment");
  expect_export_of<ExportCocoRequest>("coco");
  expect_export_of<ExportPascalVocRequest>("pascal-voc");
  expect_export_of<ExportCreateMLRequest>("createml");
}

TEST_F(UTEST_ApplicationFormatsConverter,
       a_destination_which_can_not_be_created_fails_the_conversion)
{
  given_the_request("plain-txt", "coco");

  // A child of something which is no directory, so that the destination can
  // not be made whatever the suite is run as.
  appctx->convert_dst = "/dev/null/destination";

  EXPECT_CALL(*eventer, submit(A<ExportCocoRequestPtr>())).Times(0);

  EXPECT_FALSE(converter->convert(appctx));
}

TEST_F(UTEST_ApplicationFormatsConverter,
       the_pytorch_vision_layout_is_no_destination)
{
  given_the_request("plain-txt", "pytorch");

  EXPECT_CALL(*eventer, submit(A<Export2PyTorchVisionRequestPtr>())).Times(0);

  EXPECT_FALSE(converter->convert(appctx));
}
