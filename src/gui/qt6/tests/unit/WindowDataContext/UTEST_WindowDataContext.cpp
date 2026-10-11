#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "src/gui/qt6/main-window/WindowDataContext.h"

using namespace testing;
using imagesAnnotatorQt6::window::WindowDataContext;

TEST(UTEST_Qt6WindowDataContext, get_ids_is_not_empty)
{
  EXPECT_FALSE(WindowDataContext::get_ids().empty());
}

TEST(UTEST_Qt6WindowDataContext, get_ids_contains_the_main_window)
{
  const auto& ids = WindowDataContext::get_ids();

  EXPECT_NE(ids.find(WindowDataContext::window_id), ids.cend());
}

TEST(UTEST_Qt6WindowDataContext, every_id_is_a_usable_object_name)
{
  // The ids double as QObject::objectName() values, which the window loader
  // resolves through findChild(); an empty one would silently match nothing.
  for (const auto& id : WindowDataContext::get_ids()) {
    EXPECT_FALSE(id.empty());
  }
}

TEST(UTEST_Qt6WindowDataContext, get_ids_returns_a_stable_reference)
{
  EXPECT_EQ(&WindowDataContext::get_ids(), &WindowDataContext::get_ids());
}

TEST(UTEST_Qt6WindowDataContext, embedded_resource_paths_use_the_qt_scheme)
{
  // ":" prefixed paths are what QFile / QPixmap resolve against the compiled-in
  // Qt resource tree of src/gui/qt6/ui/template.qrc.
  const std::string logo{WindowDataContext::logo_res_path};
  const std::string qss{WindowDataContext::main_window_qss};

  EXPECT_EQ(logo.find(':'), 0U);
  EXPECT_EQ(qss.find(':'), 0U);

  EXPECT_NE(logo.find("/qt6/"), std::string::npos);
  EXPECT_NE(qss.find("/qt6/"), std::string::npos);
}

// The menu actions of the darknetxx exports are looked up by these object
// names, so the window loader resolves them only once listed here.
TEST(UTEST_Qt6WindowDataContext, get_ids_contains_the_darknetxx_export_ids)
{
  const auto& ids = WindowDataContext::get_ids();

  EXPECT_EQ(
      ids.count(WindowDataContext::annotations_export_dxxwjz2_2_folder_id), 1U);
  EXPECT_EQ(
      ids.count(WindowDataContext::annotations_export_dxxwjz2p_2_folder_id),
      1U);
  EXPECT_EQ(
      ids.count(WindowDataContext::annotations_export_dxxwjz3_2_folder_id), 1U);
  EXPECT_EQ(
      ids.count(WindowDataContext::annotations_export_dxxwjz3p_2_folder_id),
      1U);
}

TEST(UTEST_Qt6WindowDataContext, get_ids_contains_every_import_id)
{
  const auto& ids = WindowDataContext::get_ids();

  EXPECT_EQ(ids.count(WindowDataContext::annotations_import_txt_2_folder_id),
            1U);
  EXPECT_EQ(ids.count(WindowDataContext::annotations_import_yolo4_2_folder_id),
            1U);
  EXPECT_EQ(
      ids.count(
          WindowDataContext::annotations_import_ultralytics_detect_2_folder_id),
      1U);
  EXPECT_EQ(
      ids.count(
          WindowDataContext::annotations_import_ultralytics_obb_2_folder_id),
      1U);
  EXPECT_EQ(ids.count(WindowDataContext::
                          annotations_import_ultralytics_segment_2_folder_id),
            1U);
  EXPECT_EQ(ids.count(WindowDataContext::annotations_import_coco_2_folder_id),
            1U);
  EXPECT_EQ(
      ids.count(WindowDataContext::annotations_import_pascal_voc_2_folder_id),
      1U);
  EXPECT_EQ(
      ids.count(WindowDataContext::annotations_import_createml_2_folder_id),
      1U);
  EXPECT_EQ(
      ids.count(
          WindowDataContext::annotations_import_pytorchvision_2_folder_id),
      1U);
}

// The menu actions of the darknetxx imports are looked up by these object
// names, so the window loader resolves them only once listed here.
TEST(UTEST_Qt6WindowDataContext, get_ids_contains_the_darknetxx_import_ids)
{
  const auto& ids = WindowDataContext::get_ids();

  EXPECT_EQ(
      ids.count(WindowDataContext::annotations_import_dxxwjz2_2_folder_id), 1U);
  EXPECT_EQ(
      ids.count(WindowDataContext::annotations_import_dxxwjz2p_project_id), 1U);
  EXPECT_EQ(
      ids.count(WindowDataContext::annotations_import_dxxwjz3_2_folder_id), 1U);
  EXPECT_EQ(
      ids.count(WindowDataContext::annotations_import_dxxwjz3p_project_id), 1U);
}
