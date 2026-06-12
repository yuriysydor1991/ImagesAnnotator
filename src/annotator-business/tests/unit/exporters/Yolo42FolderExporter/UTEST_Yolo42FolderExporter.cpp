#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>

#include "src/annotator-business/exporters/ExportContext.h"
#include "src/annotator-business/exporters/Yolo42FolderExporter.h"
#include "src/annotator-events/events/ImageRecord.h"
#include "src/annotator-events/events/ImagesPathsDBProvider.h"

using namespace testing;
using events::events::ImageRecordsSet;
using iannotator::exporters::ExportContext;
using iannotator::exporters::Yolo42FolderExporter;

namespace
{

class FakeProvider : public events::events::ImagesPathsDBProvider
{
 public:
  ImageRecordsSet records;
  AnnotationsList annotations;

  ImageRecordsSet& get_images_db() override { return records; }
  std::string get_db_path() override { return {}; }
  AnnotationsList get_available_annotations() override { return annotations; }
  bool changed() override { return false; }
  bool delete_image_record(const std::string&) override { return false; }
};

std::string read_file(const std::filesystem::path& p)
{
  std::ifstream f(p);
  std::stringstream ss;
  ss << f.rdbuf();
  std::string s = ss.str();
  while (!s.empty() && (s.back() == '\n' || s.back() == '\r')) s.pop_back();
  return s;
}

}  // namespace

TEST(UTEST_Yolo42FolderExporter, exports_the_yolo_directory_layout)
{
  const std::filesystem::path dir =
      std::filesystem::path{testing::TempDir()} / "utest_yolo_export";
  std::filesystem::remove_all(dir);

  auto provider = std::make_shared<FakeProvider>();
  provider->annotations = {"cat", "dog"};

  auto ctx = std::make_shared<ExportContext>();
  ctx->export_path = dir.string();
  ctx->dbProvider = provider;

  Yolo42FolderExporter exporter;

  EXPECT_TRUE(exporter.export_db(ctx));

  EXPECT_TRUE(std::filesystem::is_directory(dir / "data"));
  EXPECT_TRUE(std::filesystem::is_directory(dir / "cfg"));
  EXPECT_TRUE(std::filesystem::is_directory(dir / "backup"));

  ASSERT_TRUE(std::filesystem::exists(dir / "data" / "obj.names"));
  EXPECT_EQ(read_file(dir / "data" / "obj.names"), "cat\ndog");

  EXPECT_TRUE(std::filesystem::exists(dir / "data" / "obj.data"));
  EXPECT_TRUE(std::filesystem::exists(dir / "cfg" / "yolov4-obj.cfg"));
  EXPECT_TRUE(std::filesystem::exists(dir / "data" / "train.txt"));
  EXPECT_TRUE(std::filesystem::exists(dir / "data" / "val.txt"));

  std::filesystem::remove_all(dir);
}
