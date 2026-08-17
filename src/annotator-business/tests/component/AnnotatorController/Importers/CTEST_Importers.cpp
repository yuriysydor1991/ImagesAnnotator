#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <memory>
#include <string>

#include "CTEST_AnnotatorController.h"
#include "ctests_decls.h"
#include "src/annotator-business/AnnotatorController.h"
#include "src/annotator-events/ImagesAnnotatorEventController.h"
#include "src/annotator-events/events/EventsFactory.h"
#include "src/annotator-events/events/IImageSizeFacilityProvider.h"
#include "src/annotator-events/events/ImportPlainTxt2FolderRequest.h"
#include "src/annotator-events/events/ImportPyTorchVisionRequest.h"
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
 * LibraryFacade::create_importer(), that the library context the controller
 * fills in carries the annotations database and the image size reader of the
 * event, and that the records the layout named by that context type yields end
 * up in the database the application is editing.
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
