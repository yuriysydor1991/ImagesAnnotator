#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_set>

#include "CTEST_AnnotatorController.h"
#include "ctests_decls.h"
#include "src/annotator-business/AnnotatorController.h"
#include "src/annotator-events/ImagesAnnotatorEventController.h"
#include "src/annotator-events/events/ImagesDirChanged.h"
#include "src/app/ApplicationContext.h"
#include "src/log/log.h"

namespace fs = std::filesystem;

class CTEST_ImagesDirLoader : public CTEST_AnnotatorController
{
 public:
  inline static const std::string tmp_data_dir =
      ctests_decls::tmp_data_path_root + "/CTEST_ImagesDirLoader";

  inline static std::unordered_set<std::string> default_junk_names{
      "default.junkfile.name.1",           "default.junkfile.name.2",
      "default.junkfile.name.3",           "default.junkfile.name.qwe",
      "default.junkfile.name.unknown_ext", "default-junkfile"};

  inline static std::unordered_set<std::string> default_valid_image_names{
      "default.image.stub.name.1.png",  "default.image.stub.name.2.png",
      "default.image.stub.name.3.jpeg", "default.image.stub.name.4.jpg",
      "default.image.stub.name.5.gif",  "default.image.stub.name.6.tif",
      "default.image.stub.name.7.tiff", "default.image.stub.name.8.bmp",
      "default.image.stub.name.8.webp"};

  inline static const std::unordered_set<std::string> valid_extsDB{
      ".png", ".jpg",  ".jpeg", ".webp", ".jpe", ".jp2", ".avif", ".bmp",
      ".dib", ".pbm",  ".pgm",  ".ppm",  ".pxm", ".pnm", ".pfm",  ".sr",
      ".ras", ".tiff", ".tif",  ".exr",  ".hdr", ".pic", ".gif"};

  inline static constexpr const unsigned int SUBSC = 100U;

  ~CTEST_ImagesDirLoader() = default;

  CTEST_ImagesDirLoader()
  {
    EXPECT_NE(controller, nullptr);
    EXPECT_NE(actx, nullptr);

    EXPECT_TRUE(create_tmp_dir(ctests_decls::tmp_data_path_root));
    EXPECT_TRUE(create_tmp_dir(tmp_data_dir));
  }

  inline static bool create_tmp_dir(const std::string& path)
  {
    if (!fs::exists(path)) {
      EXPECT_TRUE(fs::create_directory(path));
    }

    return true;
  }

  inline static bool create_tmp_dir_junkfile(const std::string& dirpath,
                                             const std::string& fname)
  {
    if (!fs::exists(dirpath)) {
      EXPECT_TRUE(fs::create_directory(dirpath));
    }

    const std::string ffpath = dirpath + "/" + fname;

    std::fstream junkfile(ffpath.c_str(), std::fstream::out);

    if (!junkfile.is_open()) {
      LOGE("Fail to create the junkfile at the path " << ffpath);
      return false;
    }

    junkfile << "Junk file! Erase when seen!";

    junkfile.close();

    return true;
  }

  inline static bool create_tmp_dir_default_junkfiles(
      const std::string& dirpath)
  {
    for (const auto& junkname : default_junk_names) {
      EXPECT_TRUE(create_tmp_dir_junkfile(dirpath, junkname));
    }

    return true;
  }

  inline static bool create_tmp_dir_default_valid_files(
      const std::string& dirpath)
  {
    for (const auto& fname : default_valid_image_names) {
      EXPECT_TRUE(create_tmp_dir_junkfile(dirpath, fname));
    }

    return true;
  }
};

TEST_F(CTEST_ImagesDirLoader, no_context_init_failure)
{
  EXPECT_FALSE(controller->init({}));
}

TEST_F(CTEST_ImagesDirLoader, empty_context_init_success)
{
  EXPECT_TRUE(controller->init(actx));
}

TEST_F(CTEST_ImagesDirLoader, images_empty_dir_context_init_success)
{
  static const std::string tmpdir =
      tmp_data_dir + "/images_dir_context_init_success";

  EXPECT_TRUE(create_tmp_dir(tmpdir));

  actx->images_db_path = tmpdir;

  EXPECT_TRUE(controller->init(actx));

  EXPECT_TRUE(controller->get_images_db().empty());
}

TEST_F(CTEST_ImagesDirLoader, images_junk_dir_context_init_success)
{
  static const std::string tmpdir =
      tmp_data_dir + "/images_dir_context_init_success";

  EXPECT_TRUE(create_tmp_dir(tmpdir));
  EXPECT_TRUE(create_tmp_dir_default_junkfiles(tmpdir));

  actx->images_db_path = tmpdir;

  EXPECT_TRUE(controller->init(actx));

  EXPECT_TRUE(controller->get_images_db().empty());
}

TEST_F(CTEST_ImagesDirLoader, images_valid_fnames_dir_context_init_success)
{
  static const std::string tmpdir =
      tmp_data_dir + "/images_valid_fnames_dir_context_init_success";

  EXPECT_TRUE(create_tmp_dir(tmpdir));
  EXPECT_TRUE(create_tmp_dir_default_valid_files(tmpdir));

  actx->images_db_path = tmpdir;

  EXPECT_TRUE(controller->init(actx));

  auto& db = controller->get_images_db();

  EXPECT_FALSE(db.empty());

  EXPECT_EQ(db.size(), default_valid_image_names.size());

  for (const auto& dbentry : db) {
    fs::path fspath{dbentry->path};
    const std::string ext = fspath.extension();

    EXPECT_NE(valid_extsDB.find(ext), valid_extsDB.end());
  }
}

TEST_F(CTEST_ImagesDirLoader,
       images_valid_fnames_dir_with_subdir_context_init_success)
{
  static const std::string tmpdir =
      tmp_data_dir +
      "/images_valid_fnames_dir_with_subdir_context_init_success";
  static const std::string tmpsubdir = tmpdir + "/sub";

  EXPECT_TRUE(create_tmp_dir(tmpdir));
  EXPECT_TRUE(create_tmp_dir(tmpsubdir));
  EXPECT_TRUE(create_tmp_dir_default_valid_files(tmpdir));
  EXPECT_TRUE(create_tmp_dir_default_valid_files(tmpsubdir));

  actx->images_db_path = tmpdir;

  EXPECT_TRUE(controller->init(actx));

  auto& db = controller->get_images_db();

  EXPECT_FALSE(db.empty());

  EXPECT_EQ(db.size(), default_valid_image_names.size() * 2U);

  for (const auto& dbentry : db) {
    fs::path fspath{dbentry->path};
    const std::string ext = fspath.extension();

    EXPECT_NE(valid_extsDB.find(ext), valid_extsDB.end());
  }
}

TEST_F(CTEST_ImagesDirLoader,
       images_junk_and_valid_fnames_dir_context_init_success)
{
  static const std::string tmpdir =
      tmp_data_dir + "/images_junk_and_valid_fnames_dir_context_init_success";

  EXPECT_TRUE(create_tmp_dir(tmpdir));
  EXPECT_TRUE(create_tmp_dir_default_junkfiles(tmpdir));
  EXPECT_TRUE(create_tmp_dir_default_valid_files(tmpdir));

  actx->images_db_path = tmpdir;

  EXPECT_TRUE(controller->init(actx));

  auto& db = controller->get_images_db();

  EXPECT_FALSE(db.empty());

  EXPECT_EQ(db.size(), default_valid_image_names.size());

  for (const auto& dbentry : db) {
    fs::path fspath{dbentry->path};
    const std::string ext = fspath.extension();

    EXPECT_NE(valid_extsDB.find(ext), valid_extsDB.end());
  }
}

TEST_F(CTEST_ImagesDirLoader,
       images_junk_and_valid_fnames_dir_with_sub_context_init_success)
{
  static const std::string tmpdir =
      tmp_data_dir +
      "/images_junk_and_valid_fnames_dir_with_sub_context_init_success";
  static const std::string tmpsubdir = tmpdir + "/sub";

  EXPECT_TRUE(create_tmp_dir(tmpdir));
  EXPECT_TRUE(create_tmp_dir(tmpsubdir));
  EXPECT_TRUE(create_tmp_dir_default_valid_files(tmpdir));
  EXPECT_TRUE(create_tmp_dir_default_valid_files(tmpsubdir));
  EXPECT_TRUE(create_tmp_dir_default_junkfiles(tmpdir));
  EXPECT_TRUE(create_tmp_dir_default_junkfiles(tmpsubdir));

  actx->images_db_path = tmpdir;

  EXPECT_TRUE(controller->init(actx));

  auto& db = controller->get_images_db();

  EXPECT_FALSE(db.empty());

  EXPECT_EQ(db.size(), default_valid_image_names.size() * 2U);

  for (const auto& dbentry : db) {
    fs::path fspath{dbentry->path};
    const std::string ext = fspath.extension();

    EXPECT_NE(valid_extsDB.find(ext), valid_extsDB.end());
  }
}

TEST_F(CTEST_ImagesDirLoader,
       images_junk_and_valid_fnames_dir_with_multisub_context_init_success)
{
  static const std::string tmpdir =
      tmp_data_dir +
      "/images_junk_and_valid_fnames_dir_with_multisub_context_init_success";
  static std::string tmpsubdir = tmpdir;

  EXPECT_TRUE(create_tmp_dir(tmpdir));
  EXPECT_TRUE(create_tmp_dir_default_valid_files(tmpdir));
  EXPECT_TRUE(create_tmp_dir_default_junkfiles(tmpdir));

  for (auto iter = 0U; iter < SUBSC; ++iter) {
    tmpsubdir += "/" + std::to_string(iter);

    EXPECT_TRUE(create_tmp_dir(tmpsubdir));
    EXPECT_TRUE(create_tmp_dir_default_valid_files(tmpsubdir));
    EXPECT_TRUE(create_tmp_dir_default_junkfiles(tmpsubdir));
  }

  actx->images_db_path = tmpdir;

  EXPECT_TRUE(controller->init(actx));

  auto& db = controller->get_images_db();

  EXPECT_FALSE(db.empty());

  EXPECT_EQ(db.size(), default_valid_image_names.size() * SUBSC +
                           default_valid_image_names.size());

  for (const auto& dbentry : db) {
    fs::path fspath{dbentry->path};
    const std::string ext = fspath.extension();

    EXPECT_NE(valid_extsDB.find(ext), valid_extsDB.end());
  }
}

TEST_F(CTEST_ImagesDirLoader, no_images_event_handle_failure)
{
  controller->handle(std::shared_ptr<ImagesDirChanged>{});

  auto& db = controller->get_images_db();

  EXPECT_TRUE(db.empty());
}

TEST_F(CTEST_ImagesDirLoader, images_empty_dirname_handle_failure)
{
  auto ievent = std::make_shared<ImagesDirChanged>("");

  controller->handle(ievent);

  auto& db = controller->get_images_db();

  EXPECT_TRUE(db.empty());
}

TEST_F(CTEST_ImagesDirLoader, images_filled_dir_changed_handle_success)
{
  static const std::string tmpdir =
      tmp_data_dir + "/images_dir_changed_handle_success";
  static const std::string tmpsubdir = tmpdir + "/sub";

  EXPECT_TRUE(create_tmp_dir(tmpdir));
  EXPECT_TRUE(create_tmp_dir(tmpsubdir));
  EXPECT_TRUE(create_tmp_dir_default_valid_files(tmpdir));
  EXPECT_TRUE(create_tmp_dir_default_valid_files(tmpsubdir));
  EXPECT_TRUE(create_tmp_dir_default_junkfiles(tmpdir));
  EXPECT_TRUE(create_tmp_dir_default_junkfiles(tmpsubdir));

  auto ievent = std::make_shared<ImagesDirChanged>(tmpdir);

  controller->handle(ievent);

  auto& db = controller->get_images_db();

  EXPECT_FALSE(db.empty());

  EXPECT_EQ(db.size(), default_valid_image_names.size() * 2U);

  for (const auto& dbentry : db) {
    fs::path fspath{dbentry->path};
    const std::string ext = fspath.extension();

    EXPECT_NE(valid_extsDB.find(ext), valid_extsDB.end());
  }
}
