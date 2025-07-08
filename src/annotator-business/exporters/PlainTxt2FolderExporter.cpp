#include "src/annotator-business/exporters/PlainTxt2FolderExporter.h"

#include <cassert>
#include <memory>
#include <sstream>

#include "src/annotator-business/exporters/IExporter.h"
#include "src/log/log.h"

namespace iannotator::exporters
{

PlainTxt2FolderExporter::~PlainTxt2FolderExporter() { clear(); }

bool PlainTxt2FolderExporter::export_db(ExportContextPtr ectx)
{
  assert(ectx != nullptr);
  assert(ectx->dbProvider != nullptr);

  clear();

  if (ectx == nullptr) {
    LOGE("Invalid export context pointer provided");
    return false;
  }

  if (ectx->dbProvider == nullptr) {
    LOGE("Export context with no db provider");
    return false;
  }

  if (ectx->export_path.empty()) {
    LOGE("Export context with no export path provider");
    return false;
  }

  const auto& exportDir = ectx->export_path;

  LOGD("Will be trying to export some data to " << exportDir);

  auto dbset = ectx->dbProvider->get_images_db();

  for (const auto& ir : dbset) {
    assert(ir != nullptr);

    if (ir == nullptr) {
      LOGE("Located invalid pointer in the image records queue");
      continue;
    }

    export_ir(ir, exportDir);
  }

  clear();

  return false;
}

PlainTxt2FolderExporter::irDataTmpDB PlainTxt2FolderExporter::gather_ir_data(
    const ImageRecordPtr& ir)
{
  irDataTmpDB tag2data;

  for (const auto& irr : ir->rects) {
    auto& dp = tag2data[irr->name];

    std::stringstream oss;

    oss << irr->x << " " << irr->y << " " << irr->width << " " << irr->height;

    dp.first++;
    dp.second +=
        (dp.second.empty() ? std::string{} : std::string{" "}) + oss.str();
  }

  return tag2data;
}

void PlainTxt2FolderExporter::export_ir(const ImageRecordPtr& ir,
                                        const std::string& exportDir)
{
  const auto& imagePath = ir->get_full_path();

  LOGT("Trying to export image: " << imagePath);

  const auto tag2data = gather_ir_data(ir);

  write_ir_data(tag2data, exportDir, imagePath);
}

void PlainTxt2FolderExporter::write_ir_data(const irDataTmpDB& tag2data,
                                            const std::string& exportDir,
                                            const std::string& imagePath)
{
  for (const auto& p : tag2data) {
    auto f = get_file(exportDir, p.first);

    assert(f != nullptr);

    if (f == nullptr) {
      LOGE("Invalid file pointer obtained for " << p.first << " in the file "
                                                << imagePath);
      continue;
    }

    if (p.second.first == 0) {
      continue;
    }

    *f << imagePath << " " << p.second.first << " " << p.second.second
       << std::endl;
  }
}

void PlainTxt2FolderExporter::clear()
{
  for (auto& p : exFiles) {
    assert(p.second != nullptr);

    p.second->close();
  }

  exFiles.clear();
}

PlainTxt2FolderExporter::FilePtr PlainTxt2FolderExporter::get_file(
    const std::string& dirPath, const std::string& tagName)
{
  static const std::string txtExt = ".txt";

  const std::string filename = tagName + txtExt;
  const std::string filepath = dirPath + "/" + filename;

  auto miter = exFiles.find(filepath);

  if (miter != exFiles.end()) {
    LOGT("cache hit");
    return miter->second;
  }

  auto nfptr = std::make_shared<std::fstream>();

  nfptr->open(filepath.c_str(), std::fstream::out | std::fstream::trunc);

  if (!nfptr->is_open()) {
    LOGE("Fail to create/open file: " << filepath);
    return {};
  }

  exFiles[filepath] = nfptr;

  return nfptr;
}

}  // namespace iannotator::exporters
