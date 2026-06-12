#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <filesystem>
#include <memory>
#include <string>

#include "src/annotator-business/exporters/ExportContext.h"
#include "src/annotator-business/exporters/PyTorchVisionFolderExporter.h"
#include "src/annotator-events/events/IImageCropperFacilityProvider.h"
#include "src/annotator-events/events/ImageRecord.h"
#include "src/annotator-events/events/ImageRecordRect.h"
#include "src/annotator-events/events/ImagesPathsDBProvider.h"

using namespace testing;
using events::events::ImageRecordPtr;
using events::events::ImageRecordRectPtr;
using events::events::ImageRecordsSet;
using iannotator::exporters::ExportContext;
using iannotator::exporters::PyTorchVisionFolderExporter;

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

class FakeCropper : public events::events::IImageCropperFacilityProvider
{
 public:
  bool crop_out_2_fs(ImageRecordPtr, ImageRecordRectPtr,
                     std::string& tofpath) override
  {
    tofpath = "/tmp/crop.png";
    return true;
  }

  IImageCropperFacilityProviderPtr clone() override
  {
    return std::make_shared<FakeCropper>();
  }
};

}  // namespace

TEST(UTEST_PyTorchVisionFolderExporter, fails_when_cropper_is_missing)
{
  auto ctx = std::make_shared<ExportContext>();
  ctx->export_path = "/some/dir";
  ctx->dbProvider = std::make_shared<FakeProvider>();
  ctx->cropper = nullptr;

  PyTorchVisionFolderExporter exporter;

  EXPECT_FALSE(exporter.export_db(ctx));
}

TEST(UTEST_PyTorchVisionFolderExporter, fails_when_export_path_is_empty)
{
  auto ctx = std::make_shared<ExportContext>();
  ctx->export_path = "";
  ctx->dbProvider = std::make_shared<FakeProvider>();
  ctx->cropper = std::make_shared<FakeCropper>();

  PyTorchVisionFolderExporter exporter;

  EXPECT_FALSE(exporter.export_db(ctx));
}

TEST(UTEST_PyTorchVisionFolderExporter, succeeds_for_a_db_without_records)
{
  const std::filesystem::path dir =
      std::filesystem::path{testing::TempDir()} / "utest_pytorch_export";
  std::filesystem::remove_all(dir);
  std::filesystem::create_directories(dir);

  auto ctx = std::make_shared<ExportContext>();
  ctx->export_path = dir.string();
  ctx->dbProvider = std::make_shared<FakeProvider>();
  ctx->cropper = std::make_shared<FakeCropper>();

  PyTorchVisionFolderExporter exporter;

  EXPECT_TRUE(exporter.export_db(ctx));

  std::filesystem::remove_all(dir);
}
