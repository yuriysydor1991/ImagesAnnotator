#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>

#include "src/annotator-business/exporters/ExportContext.h"
#include "src/annotator-business/exporters/PlainTxt2FolderExporter.h"
#include "src/annotator-events/events/ImageRecord.h"
#include "src/annotator-events/events/ImageRecordRect.h"
#include "src/annotator-events/events/ImagesPathsDBProvider.h"

using namespace testing;
using events::events::ImageRecord;
using events::events::ImageRecordRect;
using events::events::ImageRecordsSet;
using iannotator::exporters::ExportContext;
using iannotator::exporters::PlainTxt2FolderExporter;

namespace
{

class FakeProvider : public events::events::ImagesPathsDBProvider
{
 public:
  ImageRecordsSet records;

  ImageRecordsSet& get_images_db() override { return records; }
  std::string get_db_path() override { return {}; }
  AnnotationsList get_available_annotations() override { return {}; }
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

TEST(UTEST_PlainTxt2FolderExporter, exports_one_txt_file_per_tag)
{
  const std::filesystem::path dir =
      std::filesystem::path{testing::TempDir()} / "utest_plaintxt_export";
  std::filesystem::remove_all(dir);
  std::filesystem::create_directories(dir);

  auto provider = std::make_shared<FakeProvider>();
  auto ir = ImageRecord::create("a.png", "/imgs");
  ir->rects.emplace_back(std::make_shared<ImageRecordRect>("cat", 1, 2, 3, 4));
  ir->rects.emplace_back(std::make_shared<ImageRecordRect>("cat", 5, 6, 7, 8));
  ir->rects.emplace_back(
      std::make_shared<ImageRecordRect>("dog", 9, 10, 11, 12));
  provider->records.emplace_back(ir);

  auto ctx = std::make_shared<ExportContext>();
  ctx->export_path = dir.string();
  ctx->dbProvider = provider;

  PlainTxt2FolderExporter exporter;

  EXPECT_TRUE(exporter.export_db(ctx));

  ASSERT_TRUE(std::filesystem::exists(dir / "cat.txt"));
  ASSERT_TRUE(std::filesystem::exists(dir / "dog.txt"));
  EXPECT_EQ(read_file(dir / "cat.txt"), "/imgs/a.png 2 1 2 3 4 5 6 7 8");
  EXPECT_EQ(read_file(dir / "dog.txt"), "/imgs/a.png 1 9 10 11 12");

  std::filesystem::remove_all(dir);
}

TEST(UTEST_PlainTxt2FolderExporter, skips_records_without_annotations)
{
  const std::filesystem::path dir =
      std::filesystem::path{testing::TempDir()} / "utest_plaintxt_export_empty";
  std::filesystem::remove_all(dir);
  std::filesystem::create_directories(dir);

  auto provider = std::make_shared<FakeProvider>();
  provider->records.emplace_back(ImageRecord::create("noann.png", "/imgs"));

  auto ctx = std::make_shared<ExportContext>();
  ctx->export_path = dir.string();
  ctx->dbProvider = provider;

  PlainTxt2FolderExporter exporter;

  EXPECT_TRUE(exporter.export_db(ctx));
  EXPECT_TRUE(std::filesystem::is_empty(dir));

  std::filesystem::remove_all(dir);
}

TEST(UTEST_PlainTxt2FolderExporter, fails_when_export_path_is_empty)
{
  auto provider = std::make_shared<FakeProvider>();

  auto ctx = std::make_shared<ExportContext>();
  ctx->export_path = "";
  ctx->dbProvider = provider;

  PlainTxt2FolderExporter exporter;

  EXPECT_FALSE(exporter.export_db(ctx));
}
