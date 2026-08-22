#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>

#include "ctests_decls.h"
#include "src/app/ApplicationContext.h"
#include "src/app/converter/ApplicationConverterComponent.h"
#include "src/app/converter/ApplicationFormatsConverter.h"

using namespace app;
using namespace app::converter;
using namespace testing;

namespace
{
namespace fs = std::filesystem;
}  // namespace

/**
 * @brief Runs whole conversions over the annotations the suite writes itself.
 *
 * Nothing is mocked here: the component brings up the real event controller and
 * the real annotator business controller, and the request events reach the
 * ImagesAnnotator-DataImporters and the ImagesAnnotator-DataExporters libraries
 * through them, so what a case asserts on is the dataset that ended up on the
 * disk. The readers and the writers themselves are covered by the suites of
 * those two libraries; what is checked here is that a command line request
 * really travels the whole pipeline with no graphical interface behind it.
 *
 * The native project database is the side of every conversion which carries the
 * image sizes itself, so no case here needs an image to be decoded.
 */
class CTEST_FormatsConverter : public Test
{
 public:
  ~CTEST_FormatsConverter() override { fs::remove_all(work_dir); }

  CTEST_FormatsConverter() : work_dir{get_work_dir()}
  {
    fs::remove_all(work_dir);
    fs::create_directories(work_dir);
  }

  /// @brief A directory of its own per test case, so that what one conversion
  /// left behind cannot be what another one asserts on.
  static fs::path get_work_dir()
  {
    const std::string tname =
        UnitTest::GetInstance()->current_test_info()->name();

    return fs::path{ctests_decls::tmp_data_path_root} /
           ("CTEST_FormatsConverter-" + tname);
  }

  void given_file(const std::string& relPath, const std::string& contents)
  {
    const fs::path fpath = work_dir / relPath;

    fs::create_directories(fpath.parent_path());

    std::ofstream f{fpath};
    f << contents;
  }

  /**
   * @brief Writes the annotations project of one 200x100 image carrying one
   * 100x40 `cat` rectangle at 50,20, and the image file it names.
   */
  void given_the_native_project()
  {
    given_file("imgs/a.png", "the-pixels-are-never-decoded-here");
    given_file("project.json",
               "{\"ImagesAnnotator\":{\"version\":\"0.12.0\"},"
               "\"annotations\":[{\"annotations\":[{\"annotations\":["
               "{\"name\":\"cat\",\"rect\":"
               "{\"height\":40,\"width\":100,\"x\":50,\"y\":20}}],"
               "\"height\":100,\"rel_path\":\"a.png\",\"scale_factor\":0.0,"
               "\"width\":200}],\"db\":{\"path\":\"" +
                   (work_dir / "imgs").string() + "\"}}]}");
  }

  /// @brief Writes the plain text dataset of the very same rectangle.
  void given_the_plain_txt_dataset()
  {
    given_file("imgs/a.png", "the-pixels-are-never-decoded-here");
    given_file("plain/cat.txt",
               (work_dir / "imgs" / "a.png").string() + " 1 50 20 100 40\n");
  }

  /// @brief Runs a whole conversion the way the command line asks for one.
  int run_conversion(const std::string& from, const fs::path& src,
                     const std::string& into, const fs::path& dst)
  {
    auto ctx = std::make_shared<ApplicationContext>(argc, argv);

    ctx->convert_from = from;
    ctx->convert_src = src.string();
    ctx->convert_to = into;
    ctx->convert_dst = dst.string();

    auto component = std::make_shared<ApplicationConverterComponent>(
        std::make_shared<ApplicationFormatsConverter>());

    return component->run(ctx);
  }

  static std::string read_file(const fs::path& fpath)
  {
    std::ifstream f{fpath};
    std::stringstream contents;

    contents << f.rdbuf();

    return contents.str();
  }

  int argc{0};
  char** argv{nullptr};

  fs::path work_dir;
};

TEST_F(CTEST_FormatsConverter, a_native_project_converts_into_a_coco_dataset)
{
  given_the_native_project();

  EXPECT_EQ(run_conversion("imagesannotator", work_dir / "project.json", "coco",
                           work_dir / "coco"),
            0);

  const std::string produced =
      read_file(work_dir / "coco" / "annotations" / "instances_default.json");

  EXPECT_THAT(produced, HasSubstr("\"bbox\": [50, 20, 100, 40]"));
  EXPECT_THAT(produced, HasSubstr("\"name\": \"cat\""));
  EXPECT_THAT(produced, HasSubstr("\"width\": 200"));

  EXPECT_TRUE(fs::exists(work_dir / "coco" / "images" / "a.png"));
}

TEST_F(CTEST_FormatsConverter,
       a_native_project_converts_into_a_plain_txt_dataset)
{
  given_the_native_project();

  // The plain text layout is the one which writes into the directory it was
  // given instead of creating one, so this case is also what shows that the
  // converter makes a destination that does not exist yet.
  EXPECT_EQ(run_conversion("imagesannotator", work_dir / "project.json",
                           "plain-txt", work_dir / "plain-out"),
            0);

  EXPECT_THAT(read_file(work_dir / "plain-out" / "cat.txt"),
              HasSubstr(" 1 50 20 100 40"));
}

TEST_F(CTEST_FormatsConverter,
       a_plain_txt_dataset_converts_into_a_native_project)
{
  given_the_plain_txt_dataset();

  EXPECT_EQ(run_conversion("plain-txt", work_dir / "plain", "imagesannotator",
                           work_dir / "recovered" / "project.json"),
            0);

  const std::string produced =
      read_file(work_dir / "recovered" / "project.json");

  EXPECT_THAT(produced, HasSubstr("\"name\": \"cat\""));
  EXPECT_THAT(produced, HasSubstr("\"x\": 50"));
  EXPECT_THAT(produced, HasSubstr("\"rel_path\": \"a.png\""));
}

TEST_F(CTEST_FormatsConverter, a_missing_source_is_a_failed_conversion)
{
  EXPECT_NE(run_conversion("coco", work_dir / "nothing-here", "yolo4",
                           work_dir / "out"),
            0);
}

TEST_F(CTEST_FormatsConverter, an_unknown_format_is_a_failed_conversion)
{
  given_the_native_project();

  EXPECT_NE(run_conversion("imagesannotator", work_dir / "project.json",
                           "nosuch", work_dir / "out"),
            0);

  EXPECT_FALSE(fs::exists(work_dir / "out"));
}

TEST_F(CTEST_FormatsConverter, the_pytorch_vision_layout_is_no_destination)
{
  given_the_native_project();

  EXPECT_NE(run_conversion("imagesannotator", work_dir / "project.json",
                           "pytorch", work_dir / "out"),
            0);
}
