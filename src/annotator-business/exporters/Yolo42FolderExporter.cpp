/**
 * The "Simplified BSD License"
 *
 * Copyright (c) 2025, Yurii Sydor (yuriysydor1991@gmail.com)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   - Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "src/annotator-business/exporters/Yolo42FolderExporter.h"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <memory>
#include <sstream>
#include <utility>

#include "src/annotator-business/exporters/IExporter.h"
#include "src/log/log.h"

namespace iannotator::exporters
{

namespace
{
namespace fs = std::filesystem;
}

bool Yolo42FolderExporter::export_db(ExportContextPtr ectx)
{
  assert(ectx != nullptr);
  assert(!ectx->export_path.empty());
  assert(ectx->dbProvider != nullptr);

  LOGI("Exporting to " << ectx->export_path);

  if (!create_subdirs(ectx)) {
    LOGE("Failure while creating necessary directories");
    return false;
  }

  if (!express_obj_names(ectx)) {
    LOGE("Failure while expressing class names");
    return false;
  }

  if (!express_obj_data(ectx)) {
    LOGE("Failure to express obj.data file");
    return false;
  }

  if (!express_yolocfg(ectx)) {
    LOGE("Failure to express yolov4-obj.cfg file");
    return false;
  }

  if (!express_train_and_val(ectx)) {
    LOGE("Failure to copy and express rectangles");
    return false;
  }

  return true;
}

bool Yolo42FolderExporter::create_subdirs(ExportContextPtr ectx)
{
  assert(ectx != nullptr);
  assert(!ectx->export_path.empty());
  assert(ectx->dbProvider != nullptr);

  fs::path dirPath = ectx->export_path;

  if (!fs::is_directory(dirPath)) {
    LOGE("Directory does not exists: " << ectx->export_path);
    return false;
  }

  fs::path dataPath = dirPath / dataRel;

  if (!fs::create_directory(dataPath)) {
    LOGE("Failure while creating the directory: " << dataPath.string());
    return false;
  }

  fs::path cfgPath = dirPath / cfgRel;

  if (!fs::create_directory(cfgPath)) {
    LOGE("Failure while creating the directory: " << cfgPath.string());
    return false;
  }

  return true;
}

bool Yolo42FolderExporter::express_obj_names(ExportContextPtr ectx)
{
  assert(ectx != nullptr);
  assert(!ectx->export_path.empty());
  assert(ectx->dbProvider != nullptr);

  const std::string fpath = ectx->export_path + "/" + objNamesRel;

  std::fstream objNFile(fpath.c_str(), std::fstream::out | std::fstream::trunc);

  if (!objNFile.is_open()) {
    LOGE("Failure during file opening " << fpath);
    return false;
  }

  aList = ectx->dbProvider->get_available_annotations();

  for (const auto& name : aList) {
    objNFile << name << std::endl;
  }

  objNFile.close();

  return true;
}

bool Yolo42FolderExporter::express_obj_data(ExportContextPtr ectx)
{
  assert(ectx != nullptr);
  assert(!ectx->export_path.empty());
  assert(ectx->dbProvider != nullptr);

  const std::string fpath = ectx->export_path + "/" + objDataRel;

  std::fstream objDFile(fpath.c_str(), std::fstream::out | std::fstream::trunc);

  if (!objDFile.is_open()) {
    LOGE("Failure during file opening " << fpath);
    return false;
  }

  objDFile << "classes: " << aList.size() << std::endl;
  objDFile << "train = " << trainTxtRel << std::endl;
  objDFile << "valid = " << valTxtRel << std::endl;
  objDFile << "names = " << objNamesRel << std::endl;
  objDFile << "backup = backup/" << std::endl;

  objDFile.close();

  return true;
}

bool Yolo42FolderExporter::express_yolocfg(ExportContextPtr ectx)
{
  assert(ectx != nullptr);
  assert(!ectx->export_path.empty());
  assert(ectx->dbProvider != nullptr);

  const std::string fpath = ectx->export_path + "/" + yolov4CfgRel;

  std::fstream yoloCfgFile(fpath.c_str(),
                           std::fstream::out | std::fstream::trunc);

  if (!yoloCfgFile.is_open()) {
    LOGE("Failure during file opening " << fpath);
    return false;
  }

  yoloCfgFile << "filters = " << ((aList.size() + 5) * 3) << std::endl;

  yoloCfgFile.close();

  return true;
}

bool Yolo42FolderExporter::express_train_and_val(ExportContextPtr ectx)
{
  assert(ectx != nullptr);
  assert(!ectx->export_path.empty());
  assert(ectx->dbProvider != nullptr);

  const std::string trainpath = ectx->export_path + "/" + trainTxtRel;
  const std::string valpath = ectx->export_path + "/" + valTxtRel;

  std::fstream trainf(trainpath.c_str(),
                      std::fstream::out | std::fstream::trunc);
  std::fstream valf(valpath.c_str(), std::fstream::out | std::fstream::trunc);

  if (!trainf.is_open()) {
    LOGE("Failure during file opening " << trainpath);
    return false;
  }

  if (!valf.is_open()) {
    LOGE("Failure during file opening " << valpath);
    return false;
  }

  for (auto& ir : ectx->dbProvider->get_images_db()) {
    assert(ir != nullptr);

    if (ir->rects.empty()) {
      LOGT("Skipping image without annotations: " << ir->get_full_path());
      continue;
    }

    if (ir->iwidth == 0 || ir->iheight == 0) {
      LOGT("Skipping image record with 0 width or height: "
           << ir->get_full_path());
      continue;
    }

    const auto p = prepare_image(ectx, ir);

    if (p.first.empty() || p.second.empty()) {
      LOGE("skipping image: " << ir->get_full_path());
      continue;
    }

    const fs::path fullTxt = fs::path{ectx->export_path} / p.second;

    if (!express_image_annotations(ectx, ir, fullTxt.string())) {
      LOGE("Fail to express annotations for " << ir->get_full_path());
      continue;
    }

    trainf << p.first << std::endl;
    valf << p.first << std::endl;
  }

  trainf.close();
  valf.close();

  return true;
}

Yolo42FolderExporter::DataImage2TxtRec Yolo42FolderExporter::prepare_image(
    ExportContextPtr ectx, ImageRecordPtr& ir)
{
  assert(ir != nullptr);

  if (ir == nullptr) {
    LOGE("Image record pointer is invalid");
    return {};
  }

  fs::path origPath = ir->get_full_path();

  if (!fs::is_regular_file(origPath)) {
    LOGE("No file found under the path: " << origPath.string());
    return {};
  }

  const fs::path newpath = get_new_filepath(ectx, ir);

  assert(!newpath.string().empty());

  if (newpath.string().empty()) {
    LOGE("Failure during new file path obtaining");
    return {};
  }

  LOGT("Copying file " << origPath.string() << " to " << newpath.string());

  try {
    if (!fs::copy_file(origPath, newpath)) {
      LOGE("Fail to copy file " << origPath.string());
      return {};
    }
  }
  catch (std::exception& e) {
    LOGE("failure during file copying " << origPath << " reason: " << e.what());
    return {};
  }

  return std::make_pair<std::string, std::string>(
      (fs::path{dataRel} / newpath.filename()).string(),
      (fs::path{dataRel} / (newpath.stem().string() + ".txt")).string());
}

fs::path Yolo42FolderExporter::get_new_filepath(ExportContextPtr ectx,
                                                ImageRecordPtr& ir)
{
  const fs::path origPath = ir->get_full_path();

  fs::path newpath =
      fs::path{ectx->export_path} / dataRel / origPath.filename();

  if (!fs::is_regular_file(newpath)) {
    return newpath;
  }

  LOGT("File with the same name already exists, creating the new one");

  unsigned long long index{1};
  do {
    newpath = fs::path{ectx->export_path} / dataRel /
              (origPath.stem().string() + "-" + std::to_string(index) +
               origPath.extension().string());
  } while (fs::is_regular_file(newpath));

  return newpath;
}

bool Yolo42FolderExporter::express_image_annotations(
    [[maybe_unused]] ExportContextPtr ectx, ImageRecordPtr& ir,
    const std::string& irtxtpath)
{
  assert(ectx != nullptr);
  assert(!ectx->export_path.empty());
  assert(ectx->dbProvider != nullptr);
  assert(!irtxtpath.empty());

  if (irtxtpath.empty()) {
    LOGE("Empty filepath provided");
    return false;
  }

  std::fstream ftxt(irtxtpath.c_str(), std::fstream::out | std::fstream::trunc);

  if (!ftxt.is_open()) {
    LOGE("Fail to open file " << irtxtpath);
    return false;
  }

  for (auto& irr : ir->rects) {
    assert(!irr->name.empty());

    auto classIter = aList.find(irr->name);

    assert(classIter != aList.end());

    if (classIter == aList.end()) {
      LOGE("Didn't find class name in the list: " << irr->name);
      continue;
    }

    const IndexType classIndex = std::distance(aList.begin(), classIter);

    if (!express_rectangle_data(ftxt, ir, classIndex, irr)) {
      LOGE("Failure while expressing rectangle name: " << irr->name);
      continue;
    }
  }

  ftxt.close();

  return true;
}

bool Yolo42FolderExporter::express_rectangle_data(std::fstream& ftxt,
                                                  ImageRecordPtr& ir,
                                                  const IndexType& index,
                                                  ImageRecordRectPtr& irr)
{
  assert(ftxt.is_open());
  assert(irr != nullptr);

  if (ir->iwidth == 0) {
    LOGE("Image record contains 0 width " << ir->get_full_path());
    return false;
  }

  if (ir->iheight == 0) {
    LOGE("Image record contains 0 height " << ir->get_full_path());
    return false;
  }

  const double cx = toD(irr->x) + (toD(irr->width) / 2.0);
  const double cy = toD(irr->y) + (toD(irr->height) / 2.0);

  const double ncx = cx / toD(ir->iwidth);
  const double ncy = cy / toD(ir->iheight);
  const double nwidth = toD(irr->width) / toD(ir->iwidth);
  const double nheight = toD(irr->height) / toD(ir->iheight);

  ftxt << index << " " << ncx << " " << ncy << " " << nwidth << " " << nheight
       << std::endl;

  return true;
}

}  // namespace iannotator::exporters
