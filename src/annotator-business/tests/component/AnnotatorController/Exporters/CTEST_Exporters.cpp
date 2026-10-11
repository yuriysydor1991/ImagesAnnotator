#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>

#include "CTEST_AnnotatorController.h"
#include "ctests_decls.h"
#include "src/annotator-business/AnnotatorController.h"
#include "src/annotator-events/ImagesAnnotatorEventController.h"
#include "src/annotator-events/events/EventsFactory.h"
#include "src/annotator-events/events/ExportCocoRequest.h"
#include "src/annotator-events/events/ExportCreateMLRequest.h"
#include "src/annotator-events/events/ExportDxxwjz2Request.h"
#include "src/annotator-events/events/ExportDxxwjz2pRequest.h"
#include "src/annotator-events/events/ExportDxxwjz3Request.h"
#include "src/annotator-events/events/ExportDxxwjz3pRequest.h"
#include "src/annotator-events/events/ExportPascalVocRequest.h"
#include "src/annotator-events/events/ExportPlainTxt2FolderRequest.h"
#include "src/annotator-events/events/ExportUltralyticsDetectRequest.h"
#include "src/annotator-events/events/ExportUltralyticsObbRequest.h"
#include "src/annotator-events/events/ExportUltralyticsSegmentRequest.h"
#include "src/annotator-events/events/ExportYolo4FolderRequest.h"
#include "src/app/ApplicationContext.h"
#include "src/log/log.h"

using namespace testing;
using namespace iannotator;
using namespace events::events;

namespace
{
namespace fs = std::filesystem;
}

/**
 * @brief Drives the AnnotatorController export request handlers over a real
 * annotations project file.
 *
 * The exporters themselves belong to the ImagesAnnotator-DataExporters library
 * and are covered by its own suites. What is checked here is this project's
 * side of that integration: that a request event reaches the library through
 * IADataExportersFacade::create_exporter(), that the library context the
 * controller fills in carries the annotations database, and that the layout
 * named by that context type is the one which ends up on the filesystem.
 */
class CTEST_Exporters : public CTEST_AnnotatorController
{
 public:
  inline static const std::string ctest_data_dir = CTEST_DATA_ROOT;
  inline static const std::string ctest_data_json_plan_name =
      "basic-structure-plan.json";

  /// @brief An annotation name of the project file above, which every export
  /// has to carry over into the produced dataset.
  inline static const std::string known_annotation_name =
      "annotation-tag-or-class-name-1";

  ~CTEST_Exporters() override { fs::remove_all(export_dir); }

  CTEST_Exporters() : export_dir{get_export_dir()}
  {
    // A failing export reports itself through a display error event, which the
    // controller builds with the events factory of the event controller. The
    // mocked controller hands out none by default, while the real one always
    // does - the invariant every emit method of the controller asserts.
    auto eventer =
        std::dynamic_pointer_cast<events::ImagesAnnotatorEventController>(
            actx->eventer);

    EXPECT_NE(eventer, nullptr);

    if (eventer != nullptr) {
      ON_CALL(*eventer, get_events_factory())
          .WillByDefault(Return(std::make_shared<EventsFactory>()));
    }

    actx->annotation_db_path = ctest_data_dir + "/" + ctest_data_json_plan_name;

    EXPECT_TRUE(controller->init(actx));
    EXPECT_FALSE(controller->get_images_db().empty());

    fs::remove_all(export_dir);
    fs::create_directories(export_dir);
  }

  /// @brief A destination of its own per test case, so that a leftover of one
  /// export cannot be what another one asserts on.
  static fs::path get_export_dir()
  {
    const std::string tname =
        UnitTest::GetInstance()->current_test_info()->name();

    return fs::path{ctests_decls::tmp_data_path_root} /
           ("CTEST_Exporters-" + tname);
  }

  static std::string read_file(const fs::path& fpath)
  {
    std::ifstream f{fpath};

    if (!f.is_open()) {
      return {};
    }

    std::ostringstream content;
    content << f.rdbuf();

    return content.str();
  }

  fs::path export_dir;
};

TEST_F(CTEST_Exporters, plain_txt_request_writes_a_file_per_annotation_name)
{
  controller->handle(
      std::make_shared<ExportPlainTxt2FolderRequest>(export_dir.string()));

  const fs::path tagFile = export_dir / (known_annotation_name + ".txt");

  ASSERT_TRUE(fs::is_regular_file(tagFile));
  EXPECT_FALSE(read_file(tagFile).empty());
}

TEST_F(CTEST_Exporters, yolo4_request_writes_the_yolo_directory_layout)
{
  controller->handle(
      std::make_shared<ExportYolo4FolderRequest>(export_dir.string()));

  EXPECT_TRUE(fs::is_directory(export_dir / "data"));
  EXPECT_TRUE(fs::is_directory(export_dir / "cfg"));
  EXPECT_TRUE(fs::is_directory(export_dir / "backup"));

  ASSERT_TRUE(fs::is_regular_file(export_dir / "data/obj.names"));

  const std::string names = read_file(export_dir / "data/obj.names");

  EXPECT_NE(names.find(known_annotation_name), std::string::npos);

  // The plain txt layout of the sibling test is not the one that was asked for.
  EXPECT_FALSE(
      fs::is_regular_file(export_dir / (known_annotation_name + ".txt")));

  // nor are the networks to train the darknetxx layouts add to this one
  EXPECT_FALSE(fs::exists(export_dir / "cfg/yolov4-obj.dxxwjz2"));
  EXPECT_FALSE(fs::exists(export_dir / "cfg/yolov4-obj.dxxwjz3"));
}

// The label file lines of each task are covered by the exporters library own
// suite. What this project has to get right is the wiring: that the request
// event reaches the library and that the layout of the context type the
// controller picked for it is the one written. The project file behind this
// fixture names image files which are not on the disk, so the export carries
// over the annotation names and copies no image.
TEST_F(CTEST_Exporters,
       ultralytics_detect_request_writes_the_ultralytics_layout)
{
  controller->handle(
      std::make_shared<ExportUltralyticsDetectRequest>(export_dir.string()));

  EXPECT_TRUE(fs::is_directory(export_dir / "images/train"));
  EXPECT_TRUE(fs::is_directory(export_dir / "labels/train"));

  ASSERT_TRUE(fs::is_regular_file(export_dir / "data.yaml"));

  const std::string yaml = read_file(export_dir / "data.yaml");

  EXPECT_NE(yaml.find("train: images/train"), std::string::npos);
  EXPECT_NE(yaml.find(known_annotation_name), std::string::npos);

  // neither of the two layouts the sibling cases ask for
  EXPECT_FALSE(fs::is_directory(export_dir / "cfg"));
  EXPECT_FALSE(
      fs::is_regular_file(export_dir / (known_annotation_name + ".txt")));
}

// The label file lines of each task are covered by the exporters library own
// suite. What this project has to get right is the wiring: that the request
// event reaches the library and that the layout of the context type the
// controller picked for it is the one written. The project file behind this
// fixture names image files which are not on the disk, so the export carries
// over the annotation names and copies no image.
TEST_F(CTEST_Exporters, ultralytics_obb_request_writes_the_ultralytics_layout)
{
  controller->handle(
      std::make_shared<ExportUltralyticsObbRequest>(export_dir.string()));

  EXPECT_TRUE(fs::is_directory(export_dir / "images/train"));
  EXPECT_TRUE(fs::is_directory(export_dir / "labels/train"));

  ASSERT_TRUE(fs::is_regular_file(export_dir / "data.yaml"));

  const std::string yaml = read_file(export_dir / "data.yaml");

  EXPECT_NE(yaml.find("train: images/train"), std::string::npos);
  EXPECT_NE(yaml.find(known_annotation_name), std::string::npos);

  // neither of the two layouts the sibling cases ask for
  EXPECT_FALSE(fs::is_directory(export_dir / "cfg"));
  EXPECT_FALSE(
      fs::is_regular_file(export_dir / (known_annotation_name + ".txt")));
}

// The label file lines of each task are covered by the exporters library own
// suite. What this project has to get right is the wiring: that the request
// event reaches the library and that the layout of the context type the
// controller picked for it is the one written. The project file behind this
// fixture names image files which are not on the disk, so the export carries
// over the annotation names and copies no image.
TEST_F(CTEST_Exporters,
       ultralytics_segment_request_writes_the_ultralytics_layout)
{
  controller->handle(
      std::make_shared<ExportUltralyticsSegmentRequest>(export_dir.string()));

  EXPECT_TRUE(fs::is_directory(export_dir / "images/train"));
  EXPECT_TRUE(fs::is_directory(export_dir / "labels/train"));

  ASSERT_TRUE(fs::is_regular_file(export_dir / "data.yaml"));

  const std::string yaml = read_file(export_dir / "data.yaml");

  EXPECT_NE(yaml.find("train: images/train"), std::string::npos);
  EXPECT_NE(yaml.find(known_annotation_name), std::string::npos);

  // neither of the two layouts the sibling cases ask for
  EXPECT_FALSE(fs::is_directory(export_dir / "cfg"));
  EXPECT_FALSE(
      fs::is_regular_file(export_dir / (known_annotation_name + ".txt")));
}

// The descriptor contents are covered by the exporters library own suite. What
// this project has to get right is the wiring: that the request event reaches
// the library and that the layout of the context type the controller picked for
// it is the one written. The project file behind this fixture names image files
// which are not on the disk, so the export carries over the annotation names
// and copies no image.
TEST_F(CTEST_Exporters, coco_request_writes_the_coco_layout)
{
  controller->handle(std::make_shared<ExportCocoRequest>(export_dir.string()));

  EXPECT_TRUE(fs::is_directory(export_dir / "images"));

  ASSERT_TRUE(
      fs::is_regular_file(export_dir / "annotations/instances_default.json"));

  const std::string json =
      read_file(export_dir / "annotations/instances_default.json");

  EXPECT_NE(json.find("\"categories\": ["), std::string::npos);
  EXPECT_NE(json.find(known_annotation_name), std::string::npos);

  // neither of the two layouts the sibling cases ask for
  EXPECT_FALSE(fs::is_regular_file(export_dir / "data.yaml"));
  EXPECT_FALSE(
      fs::is_regular_file(export_dir / (known_annotation_name + ".txt")));
}

// The descriptors themselves are covered by the exporters library own suite,
// which is where an image really is on the disk. Here the image files the
// project file names are not, so what this case can assert on is the directory
// shape and the two lists, which no other layout of the library writes.
TEST_F(CTEST_Exporters, pascal_voc_request_writes_the_pascal_voc_layout)
{
  controller->handle(
      std::make_shared<ExportPascalVocRequest>(export_dir.string()));

  EXPECT_TRUE(fs::is_directory(export_dir / "JPEGImages"));
  EXPECT_TRUE(fs::is_directory(export_dir / "Annotations"));

  EXPECT_TRUE(fs::is_regular_file(export_dir / "ImageSets/Main/train.txt"));
  EXPECT_TRUE(fs::is_regular_file(export_dir / "ImageSets/Main/val.txt"));

  // neither of the two layouts the sibling cases ask for
  EXPECT_FALSE(fs::is_regular_file(export_dir / "data.yaml"));
  EXPECT_FALSE(
      fs::is_regular_file(export_dir / (known_annotation_name + ".txt")));
}

// The descriptor itself is covered by the exporters library own suite. Here
// the image files the project file names are not on the disk, so what this
// case asserts on is the one flat directory the layout is: the descriptor
// beside the images, with no subdirectory of any sibling layout below it.
TEST_F(CTEST_Exporters, createml_request_writes_the_createml_layout)
{
  controller->handle(
      std::make_shared<ExportCreateMLRequest>(export_dir.string()));

  EXPECT_TRUE(fs::is_regular_file(export_dir / "annotations.json"));

  EXPECT_FALSE(fs::exists(export_dir / "images"));
  EXPECT_FALSE(fs::exists(export_dir / "JPEGImages"));

  // neither of the two layouts the sibling cases ask for
  EXPECT_FALSE(fs::is_regular_file(export_dir / "data.yaml"));
  EXPECT_FALSE(
      fs::is_regular_file(export_dir / (known_annotation_name + ".txt")));
}

// The network document itself is covered by the exporters library own suite.
// What is asserted on here is the layout: the YOLO v4 directory of the sibling
// case with the gzip compressed network to train beside its cfg file.
TEST_F(CTEST_Exporters, dxxwjz2_request_writes_the_darknetxx_layout)
{
  controller->handle(
      std::make_shared<ExportDxxwjz2Request>(export_dir.string()));

  EXPECT_TRUE(fs::is_directory(export_dir / "backup"));
  EXPECT_TRUE(fs::is_regular_file(export_dir / "cfg/yolov4-obj.cfg"));

  const std::string names = read_file(export_dir / "data/obj.names");

  EXPECT_NE(names.find(known_annotation_name), std::string::npos);

  const fs::path network = export_dir / "cfg/yolov4-obj.dxxwjz2";

  ASSERT_TRUE(fs::is_regular_file(network));

  // the gzip magic the darknetxx tells a dxxwjz2 file from a plain JSON one by
  EXPECT_EQ(read_file(network).substr(0, 2), "\x1F\x8B");

  // neither of the two layouts the sibling cases ask for
  EXPECT_FALSE(fs::is_regular_file(export_dir / "data.yaml"));
  EXPECT_FALSE(
      fs::is_regular_file(export_dir / (known_annotation_name + ".txt")));
}

// The dxxwjz3 sibling of the case above: the network to train of the JSON
// data alone beside the cfg file, the dxxwjz2 one not
TEST_F(CTEST_Exporters, dxxwjz3_request_writes_the_darknetxx_layout)
{
  controller->handle(
      std::make_shared<ExportDxxwjz3Request>(export_dir.string()));

  EXPECT_TRUE(fs::is_directory(export_dir / "backup"));
  EXPECT_TRUE(fs::is_regular_file(export_dir / "cfg/yolov4-obj.cfg"));

  const fs::path network = export_dir / "cfg/yolov4-obj.dxxwjz3";

  ASSERT_TRUE(fs::is_regular_file(network));
  EXPECT_EQ(read_file(network).substr(0, 2), "\x1F\x8B");
  EXPECT_FALSE(fs::exists(export_dir / "cfg/yolov4-obj.dxxwjz2"));
}

// The darknetxx training projects: the project file beside the copies of the
// images, none of the YOLO v4 directory
TEST_F(CTEST_Exporters, darknetxx_project_requests_write_the_project_layouts)
{
  const fs::path dxxwjz2p = export_dir / "dxxwjz2p";
  const fs::path dxxwjz3p = export_dir / "dxxwjz3p";

  controller->handle(
      std::make_shared<ExportDxxwjz2pRequest>(dxxwjz2p.string()));
  controller->handle(
      std::make_shared<ExportDxxwjz3pRequest>(dxxwjz3p.string()));

  for (const auto& [dir, project] :
       {std::make_pair(dxxwjz2p, std::string{"yolov4-obj.dxxwjz2p"}),
        std::make_pair(dxxwjz3p, std::string{"yolov4-obj.dxxwjz3p"})}) {
    EXPECT_TRUE(fs::is_directory(dir / "backup")) << project;
    EXPECT_TRUE(fs::is_directory(dir / "images")) << project;
    EXPECT_FALSE(fs::exists(dir / "data")) << project;
    EXPECT_FALSE(fs::exists(dir / "cfg")) << project;

    ASSERT_TRUE(fs::is_regular_file(dir / project)) << project;
    EXPECT_EQ(read_file(dir / project).substr(0, 2), "\x1F\x8B") << project;
  }

  EXPECT_FALSE(fs::exists(dxxwjz2p / "yolov4-obj.dxxwjz3p"));
  EXPECT_FALSE(fs::exists(dxxwjz3p / "yolov4-obj.dxxwjz2p"));
}

TEST_F(CTEST_Exporters, an_empty_destination_path_is_rejected)
{
  controller->handle(std::make_shared<ExportPlainTxt2FolderRequest>(""));

  EXPECT_TRUE(fs::is_empty(export_dir));
}
