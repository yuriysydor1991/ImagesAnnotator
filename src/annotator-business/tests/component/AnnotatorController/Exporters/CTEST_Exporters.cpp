#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

#include "CTEST_AnnotatorController.h"
#include "ctests_decls.h"
#include "src/annotator-business/AnnotatorController.h"
#include "src/annotator-events/ImagesAnnotatorEventController.h"
#include "src/annotator-events/events/EventsFactory.h"
#include "src/annotator-events/events/ExportPlainTxt2FolderRequest.h"
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
 * LibraryFacade::create_exporter(), that the export context the controller
 * fills in carries the annotations database, and that the requested
 * ExportFormat is the layout that ends up on the filesystem.
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
}

TEST_F(CTEST_Exporters, an_empty_destination_path_is_rejected)
{
  controller->handle(std::make_shared<ExportPlainTxt2FolderRequest>(""));

  EXPECT_TRUE(fs::is_empty(export_dir));
}
