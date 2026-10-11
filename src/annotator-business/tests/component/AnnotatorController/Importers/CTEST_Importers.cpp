#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "CTEST_AnnotatorController.h"
#include "ctests_decls.h"
#include "src/annotator-business/AnnotatorController.h"
#include "src/annotator-events/ImagesAnnotatorEventController.h"
#include "src/annotator-events/events/EventsFactory.h"
#include "src/annotator-events/events/ExportDxxwjz2Request.h"
#include "src/annotator-events/events/ExportDxxwjz2pRequest.h"
#include "src/annotator-events/events/ExportDxxwjz3Request.h"
#include "src/annotator-events/events/ExportDxxwjz3pRequest.h"
#include "src/annotator-events/events/IImageSizeFacilityProvider.h"
#include "src/annotator-events/events/ImportCocoRequest.h"
#include "src/annotator-events/events/ImportCreateMLRequest.h"
#include "src/annotator-events/events/ImportDxxwjz2Request.h"
#include "src/annotator-events/events/ImportDxxwjz2pRequest.h"
#include "src/annotator-events/events/ImportDxxwjz3Request.h"
#include "src/annotator-events/events/ImportDxxwjz3pRequest.h"
#include "src/annotator-events/events/ImportPascalVocRequest.h"
#include "src/annotator-events/events/ImportPlainTxt2FolderRequest.h"
#include "src/annotator-events/events/ImportPyTorchVisionRequest.h"
#include "src/annotator-events/events/ImportUltralyticsDetectRequest.h"
#include "src/annotator-events/events/ImportUltralyticsObbRequest.h"
#include "src/annotator-events/events/ImportUltralyticsSegmentRequest.h"
#include "src/annotator-events/events/ImportYolo4FolderRequest.h"
#include "src/app/ApplicationContext.h"
#include "src/log/log.h"

using namespace testing;
using namespace iannotator;
using namespace events::events;

namespace
{

namespace fs = std::filesystem;

/// @brief The GUI toolkit image size readers decode a real picture; this one
/// reports one and the same size for every path, so the suite drives the
/// controller over plain files it writes itself.
class FakeImageSizer : public IImageSizeFacilityProvider
{
 public:
  FakeImageSizer(int nwidth, int nheight) : width{nwidth}, height{nheight} {}

  bool read_image_size(const std::string&, int& owidth, int& oheight) override
  {
    owidth = width;
    oheight = height;
    return true;
  }

  IImageSizeFacilityProviderPtr clone() override
  {
    return std::make_shared<FakeImageSizer>(width, height);
  }

  int width;
  int height;
};

}  // namespace

/**
 * @brief Drives the AnnotatorController import request handlers over dataset
 * directories written by the suite itself.
 *
 * The importers themselves belong to the ImagesAnnotator-DataImporters library
 * and are covered by its own suites. What is checked here is this project's
 * side of that integration: that a request event reaches the library through
 * IADataImportersFacade::create_importer(), that the library context the
 * controller fills in carries the annotations database and the image size
 * reader of the event, and that the records the layout named by that context
 * type yields end up in the database the application is editing.
 */
class CTEST_Importers : public CTEST_AnnotatorController
{
 public:
  /// @brief The size the FakeImageSizer reports for every picture, which is
  /// what the YOLO v4 boxes are un-normalised by.
  inline static constexpr int image_width = 200;
  inline static constexpr int image_height = 100;

  ~CTEST_Importers() override { fs::remove_all(import_dir); }

  CTEST_Importers() : import_dir{get_import_dir()}
  {
    // The controller reports a failing import - and republishes the images
    // provider after a successful one - through events it builds with the
    // events factory of the event controller. The mocked controller hands out
    // none by default, while the real one always does.
    auto eventer =
        std::dynamic_pointer_cast<events::ImagesAnnotatorEventController>(
            actx->eventer);

    EXPECT_NE(eventer, nullptr);

    if (eventer != nullptr) {
      ON_CALL(*eventer, get_events_factory())
          .WillByDefault(Return(std::make_shared<EventsFactory>()));
    }

    // No annotation_db_path on purpose: an import fills an empty project here,
    // so whatever ends up in the database is what the import recovered.
    EXPECT_TRUE(controller->init(actx));
    EXPECT_TRUE(controller->get_images_db().empty());

    fs::remove_all(import_dir);
    fs::create_directories(import_dir);
  }

  /// @brief A source of its own per test case, so that a leftover of one
  /// import cannot be what another one asserts on.
  static fs::path get_import_dir()
  {
    const std::string tname =
        UnitTest::GetInstance()->current_test_info()->name();

    return fs::path{ctests_decls::tmp_data_path_root} /
           ("CTEST_Importers-" + tname);
  }

  void given_file(const std::string& relPath, const std::string& contents)
  {
    const fs::path fpath = import_dir / relPath;

    fs::create_directories(fpath.parent_path());

    std::ofstream f{fpath};
    f << contents;
  }

  static IImageSizeFacilityProviderPtr sizer()
  {
    return std::make_shared<FakeImageSizer>(image_width, image_height);
  }

  /// @brief The one rectangle of the recovered record, or a nullptr.
  ImageRecordRectPtr only_rect()
  {
    const auto& db = controller->get_images_db();

    if (db.size() != 1U || db.front()->rects.size() != 1U) {
      return {};
    }

    return db.front()->rects.front();
  }

  /**
   * @brief Writes the Ultralytics YOLO directory of one image and the one
   * rectangle the given label line names.
   */
  void given_the_ultralytics_dataset(const std::string& labelLine)
  {
    given_file("data.yaml",
               "train: images/train\n"
               "names:\n"
               "  0: 'cat'\n");
    given_file("images/train/a.png", "not-a-real-image");
    given_file("labels/train/a.txt", labelLine);
  }

  /**
   * @brief Expects the database to hold the one image and the one rectangle
   * every dataset of this suite carries: a 100x40 box at 50,20.
   *
   * @param name The annotation name that rectangle is to carry.
   */
  void expect_the_recovered_box(const std::string& name)
  {
    ASSERT_EQ(controller->get_images_db().size(), 1U);

    auto rect = only_rect();

    ASSERT_NE(rect, nullptr);
    EXPECT_EQ(rect->name, name);
    EXPECT_EQ(rect->x, 50);
    EXPECT_EQ(rect->y, 20);
    EXPECT_EQ(rect->width, 100);
    EXPECT_EQ(rect->height, 40);
  }

  /**
   * @brief The darknetxx training project document of the given format, the
   * plain JSON text of which the darknetxx reads as well: the one image of
   * every dataset of this suite and its one cat box.
   */
  static std::string project_of(const std::string& format,
                                const std::string& network)
  {
    return R"({"format": ")" + format + R"(", "version": 1,
"network": )" +
           network + R"(,
"annotations": {"ImagesAnnotator": {"version": "0.12.0"}, "annotations": [
  {"db": {"path": "images"}, "annotations": [
    {"rel_path": "a.png", "width": 200, "height": 100, "annotations": [{"name": "cat", "rect": {"x": 50, "y": 20, "width": 100, "height": 40}}]}
  ]}
]}})";
  }

  /// @brief The YOLO v4 directory of the one image of this suite and of no
  /// class names file, the darknetxx network to train left to the caller
  void given_the_yolo4_directory()
  {
    given_file("data/train.txt", "data/a.png\n");
    given_file("data/a.png", "not-a-real-image");
    given_file("data/a.txt", "0 0.5 0.4 0.5 0.4\n");
  }

  fs::path import_dir;
};

TEST_F(CTEST_Importers, plain_txt_request_fills_the_database)
{
  given_file("cat.txt", "/imgs/a.png 1 50 20 100 40\n");

  auto event =
      std::make_shared<ImportPlainTxt2FolderRequest>(import_dir.string());
  event->sizer = sizer();

  controller->handle(event);

  ASSERT_EQ(controller->get_images_db().size(), 1U);
  EXPECT_EQ(controller->get_images_db().front()->get_full_path(),
            "/imgs/a.png");

  auto rect = only_rect();

  ASSERT_NE(rect, nullptr);
  EXPECT_EQ(rect->name, "cat");
  EXPECT_EQ(rect->x, 50);
  EXPECT_EQ(rect->y, 20);
  EXPECT_EQ(rect->width, 100);
  EXPECT_EQ(rect->height, 40);
}

// The un-normalisation itself is covered by the importers library own suite.
// What this project has to get right is that the image size reader carried by
// the event is the one the library measures with, which is exactly what these
// numbers show: the boxes come back scaled by the FakeImageSizer size.
TEST_F(CTEST_Importers, yolo4_request_fills_the_database)
{
  given_file("data/obj.names", "cat\n");
  given_file("data/train.txt", "data/a.png\n");
  given_file("data/a.png", "not-a-real-image");
  given_file("data/a.txt", "0 0.5 0.4 0.5 0.4\n");

  auto event = std::make_shared<ImportYolo4FolderRequest>(import_dir.string());
  event->sizer = sizer();

  controller->handle(event);

  ASSERT_EQ(controller->get_images_db().size(), 1U);
  EXPECT_EQ(controller->get_images_db().front()->iwidth, image_width);
  EXPECT_EQ(controller->get_images_db().front()->iheight, image_height);

  auto rect = only_rect();

  ASSERT_NE(rect, nullptr);
  EXPECT_EQ(rect->name, "cat");
  EXPECT_EQ(rect->x, 50);
  EXPECT_EQ(rect->y, 20);
  EXPECT_EQ(rect->width, 100);
  EXPECT_EQ(rect->height, 40);
}

TEST_F(CTEST_Importers, pytorch_vision_request_fills_the_database)
{
  given_file("cat/a.png", "cropped");

  auto event =
      std::make_shared<ImportPyTorchVisionRequest>(import_dir.string());
  event->sizer = sizer();

  controller->handle(event);

  ASSERT_EQ(controller->get_images_db().size(), 1U);

  auto rect = only_rect();

  // the crop is the annotation: the rectangle covers the file whole
  ASSERT_NE(rect, nullptr);
  EXPECT_EQ(rect->name, "cat");
  EXPECT_EQ(rect->x, 0);
  EXPECT_EQ(rect->y, 0);
  EXPECT_EQ(rect->width, image_width);
  EXPECT_EQ(rect->height, image_height);
}

// The library merges by the image path, so the very same directory imported
// twice leaves the project with the images it already had.
// The three Ultralytics YOLO layouts differ in the one label file line only,
// which is what these three cases show: one and the same box comes back out of
// the centre and size of the detection line, out of the four corners of the
// oriented one and out of the polygon of the segmentation one.
TEST_F(CTEST_Importers, ultralytics_detect_request_fills_the_database)
{
  given_the_ultralytics_dataset("0 0.5 0.4 0.5 0.4\n");

  auto event =
      std::make_shared<ImportUltralyticsDetectRequest>(import_dir.string());
  event->sizer = sizer();

  controller->handle(event);

  ASSERT_EQ(controller->get_images_db().size(), 1U);
  EXPECT_EQ(controller->get_images_db().front()->iwidth, image_width);

  expect_the_recovered_box("cat");
}

TEST_F(CTEST_Importers, ultralytics_obb_request_fills_the_database)
{
  given_the_ultralytics_dataset("0 0.25 0.2 0.75 0.2 0.75 0.6 0.25 0.6\n");

  auto event =
      std::make_shared<ImportUltralyticsObbRequest>(import_dir.string());
  event->sizer = sizer();

  controller->handle(event);

  expect_the_recovered_box("cat");
}

TEST_F(CTEST_Importers, ultralytics_segment_request_fills_the_database)
{
  given_the_ultralytics_dataset("0 0.25 0.2 0.75 0.2 0.75 0.6 0.25 0.6\n");

  auto event =
      std::make_shared<ImportUltralyticsSegmentRequest>(import_dir.string());
  event->sizer = sizer();

  controller->handle(event);

  expect_the_recovered_box("cat");
}

// The COCO descriptor carries the image size itself, so this is the layout
// which needs nothing of the reader the request hands over.
TEST_F(CTEST_Importers, coco_request_fills_the_database)
{
  given_file("images/a.png", "not-a-real-image");
  given_file("annotations/instances_default.json", R"({
"images": [{"id": 1, "file_name": "a.png", "width": 640, "height": 400}],
"annotations": [{"id": 1, "image_id": 1, "category_id": 1,
                 "bbox": [50, 20, 100, 40]}],
"categories": [{"id": 1, "name": "cat"}]})");

  auto event = std::make_shared<ImportCocoRequest>(import_dir.string());
  event->sizer = sizer();

  controller->handle(event);

  ASSERT_EQ(controller->get_images_db().size(), 1U);
  // the descriptor size wins over the one the reader would have reported
  EXPECT_EQ(controller->get_images_db().front()->iwidth, 640);

  expect_the_recovered_box("cat");
}

TEST_F(CTEST_Importers, pascal_voc_request_fills_the_database)
{
  given_file("JPEGImages/a.png", "not-a-real-image");
  given_file("Annotations/a.xml", R"(<annotation>
  <folder>JPEGImages</folder>
  <filename>a.png</filename>
  <size><width>640</width><height>400</height><depth>3</depth></size>
  <object>
    <name>cat</name>
    <bndbox><xmin>50</xmin><ymin>20</ymin><xmax>150</xmax><ymax>60</ymax></bndbox>
  </object>
</annotation>)");

  auto event = std::make_shared<ImportPascalVocRequest>(import_dir.string());
  event->sizer = sizer();

  controller->handle(event);

  ASSERT_EQ(controller->get_images_db().size(), 1U);
  EXPECT_EQ(controller->get_images_db().front()->iwidth, 640);

  expect_the_recovered_box("cat");
}

// The Create ML descriptor writes the centre of a box, which comes back on the
// corner it was drawn from.
TEST_F(CTEST_Importers, createml_request_fills_the_database)
{
  given_file("a.png", "not-a-real-image");
  given_file("annotations.json", R"([
  {"imagefilename": "a.png", "annotation": [
    {"label": "cat", "coordinates": {"x": 100, "y": 40, "width": 100,
                                     "height": 40}}
  ]}
])");

  auto event = std::make_shared<ImportCreateMLRequest>(import_dir.string());
  event->sizer = sizer();

  controller->handle(event);

  ASSERT_EQ(controller->get_images_db().size(), 1U);
  // the layout carries no size, so the reader of the request filled it in
  EXPECT_EQ(controller->get_images_db().front()->iwidth, image_width);

  expect_the_recovered_box("cat");
}

TEST_F(CTEST_Importers, importing_one_directory_twice_adds_its_images_once)
{
  given_file("cat.txt", "/imgs/a.png 1 50 20 100 40\n");

  auto event =
      std::make_shared<ImportPlainTxt2FolderRequest>(import_dir.string());
  event->sizer = sizer();

  controller->handle(event);
  controller->handle(event);

  EXPECT_EQ(controller->get_images_db().size(), 1U);
}

// An import is a read only pass over its directory: what the application shows
// afterwards points at the files where they already lie.
TEST_F(CTEST_Importers, an_import_writes_nothing_into_its_directory)
{
  given_file("cat/a.png", "cropped");

  auto event =
      std::make_shared<ImportPyTorchVisionRequest>(import_dir.string());
  event->sizer = sizer();

  controller->handle(event);

  int entries{0};

  for ([[maybe_unused]] const auto& e :
       fs::recursive_directory_iterator{import_dir}) {
    ++entries;
  }

  // the cat directory and the one crop inside it, and nothing else
  EXPECT_EQ(entries, 2);
  EXPECT_EQ(controller->get_images_db().front()->get_full_path(),
            (import_dir / "cat" / "a.png").string());
}

TEST_F(CTEST_Importers, an_empty_directory_leaves_the_database_empty)
{
  auto event =
      std::make_shared<ImportPlainTxt2FolderRequest>(import_dir.string());
  event->sizer = sizer();

  controller->handle(event);

  EXPECT_TRUE(controller->get_images_db().empty());
}

TEST_F(CTEST_Importers, a_missing_directory_leaves_the_database_empty)
{
  auto event = std::make_shared<ImportYolo4FolderRequest>(
      (import_dir / "absent").string());
  event->sizer = sizer();

  controller->handle(event);

  EXPECT_TRUE(controller->get_images_db().empty());
}

// The darknetxx training project, the annotations of its images in the
// project structure of this very application: the import path names the
// directory the export writes the project into.
TEST_F(CTEST_Importers, dxxwjz2p_request_fills_the_database)
{
  given_file("images/a.png", "not-a-real-image");
  given_file("yolov4-obj.dxxwjz2p",
             project_of("dxxwjz2p", R"({"cfg": "[net]\nbatch=64\n",
               "classes": [{"index": 0, "name": "cat"}]})"));

  auto event = std::make_shared<ImportDxxwjz2pRequest>(import_dir.string());
  event->sizer = sizer();

  controller->handle(event);

  expect_the_recovered_box("cat");

  EXPECT_EQ(controller->get_images_db().front()->get_full_path(),
            (import_dir / "images" / "a.png").string());
}

// ... or the project file itself, of any name.
TEST_F(CTEST_Importers, dxxwjz3p_request_reads_the_project_file)
{
  given_file("images/a.png", "not-a-real-image");
  given_file("trained.dxxwjz3p",
             project_of("dxxwjz3p", R"({"net": {"batch": 64}, "layers": [],
               "classes": ["cat"]})"));

  auto event = std::make_shared<ImportDxxwjz3pRequest>(
      (import_dir / "trained.dxxwjz3p").string());
  event->sizer = sizer();

  controller->handle(event);

  expect_the_recovered_box("cat");
}

// The YOLO v4 directory of no class names file, the classes of which the
// dxxwjz2 network to train beside the cfg file keeps.
TEST_F(CTEST_Importers, dxxwjz2_request_reads_the_classes_of_the_network)
{
  given_the_yolo4_directory();
  given_file("cfg/yolov4-obj.dxxwjz2", R"({"format": "dxxwjz2", "version": 2,
    "network": {"cfg": "[net]\nbatch=64\n", "classes": [
      {"index": 0, "name": "cat"}]}})");

  auto event = std::make_shared<ImportDxxwjz2Request>(import_dir.string());
  event->sizer = sizer();

  controller->handle(event);

  expect_the_recovered_box("cat");
}

TEST_F(CTEST_Importers, dxxwjz3_request_reads_the_classes_of_the_network)
{
  given_the_yolo4_directory();
  given_file("cfg/yolov4-obj.dxxwjz3", R"({"format": "dxxwjz3", "version": 3,
    "network": {"net": {"batch": 64}, "layers": [], "classes": ["cat"]}})");

  auto event = std::make_shared<ImportDxxwjz3Request>(import_dir.string());
  event->sizer = sizer();

  controller->handle(event);

  expect_the_recovered_box("cat");
}

// A file of another darknetxx format is no project of the requested one.
TEST_F(CTEST_Importers, a_dxxwjz2p_request_of_a_dxxwjz3p_file_fills_nothing)
{
  given_file("images/a.png", "not-a-real-image");
  given_file("yolov4-obj.dxxwjz2p",
             project_of("dxxwjz3p", R"({"net": {"batch": 64}, "layers": [],
               "classes": ["cat"]})"));

  auto event = std::make_shared<ImportDxxwjz2pRequest>(import_dir.string());
  event->sizer = sizer();

  controller->handle(event);

  EXPECT_TRUE(controller->get_images_db().empty());
}

// Every darknetxx layout the application exports reads back into the very
// annotations it was exported of: the gzip compressed documents the exporters
// library writes through the zlib are the ones the importers library reads.
TEST_F(CTEST_Importers, the_darknetxx_exports_import_back)
{
  given_file("data/obj.names", "cat\n");
  given_the_yolo4_directory();

  auto yolo4 = std::make_shared<ImportYolo4FolderRequest>(import_dir.string());
  yolo4->sizer = sizer();

  controller->handle(yolo4);

  expect_the_recovered_box("cat");

  const fs::path exports = import_dir / "exports";

  controller->handle(
      std::make_shared<ExportDxxwjz2pRequest>((exports / "2p").string()));
  controller->handle(
      std::make_shared<ExportDxxwjz3pRequest>((exports / "3p").string()));
  controller->handle(
      std::make_shared<ExportDxxwjz2Request>((exports / "2").string()));
  controller->handle(
      std::make_shared<ExportDxxwjz3Request>((exports / "3").string()));

  auto dxxwjz2p =
      std::make_shared<ImportDxxwjz2pRequest>((exports / "2p").string());
  auto dxxwjz3p =
      std::make_shared<ImportDxxwjz3pRequest>((exports / "3p").string());
  auto dxxwjz2 =
      std::make_shared<ImportDxxwjz2Request>((exports / "2").string());
  auto dxxwjz3 =
      std::make_shared<ImportDxxwjz3Request>((exports / "3").string());

  dxxwjz2p->sizer = sizer();
  dxxwjz3p->sizer = sizer();
  dxxwjz2->sizer = sizer();
  dxxwjz3->sizer = sizer();

  controller->handle(dxxwjz2p);
  controller->handle(dxxwjz3p);
  controller->handle(dxxwjz2);
  controller->handle(dxxwjz3);

  // the source image and its four exported copies
  const auto& db = controller->get_images_db();

  ASSERT_EQ(db.size(), 5U);

  std::vector<std::string> exported;

  for (const auto& ir : db) {
    ASSERT_EQ(ir->rects.size(), 1U) << ir->get_full_path();

    EXPECT_EQ(ir->rects.front()->name, "cat") << ir->get_full_path();
    EXPECT_EQ(ir->rects.front()->x, 50) << ir->get_full_path();
    EXPECT_EQ(ir->rects.front()->y, 20) << ir->get_full_path();
    EXPECT_EQ(ir->rects.front()->width, 100) << ir->get_full_path();
    EXPECT_EQ(ir->rects.front()->height, 40) << ir->get_full_path();

    const std::string full = ir->get_full_path();

    if (full.find(exports.string()) == 0U) {
      exported.push_back(full.substr(exports.string().size() + 1U));
    }
  }

  std::sort(exported.begin(), exported.end());

  EXPECT_EQ(exported,
            (std::vector<std::string>{"2/data/a.png", "2p/images/a.png",
                                      "3/data/a.png", "3p/images/a.png"}));
}
