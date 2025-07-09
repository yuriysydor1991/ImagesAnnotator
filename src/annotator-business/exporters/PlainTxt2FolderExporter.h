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

#ifndef IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_PLAINTXT2FOLDEREXPORTER_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_PLAINTXT2FOLDEREXPORTER_CLASS_H

#include <fstream>
#include <memory>
#include <unordered_map>

#include "src/annotator-business/exporters/IExporter.h"

namespace iannotator::exporters
{

/**
 * @brief The annotations to plain txt format exporter class.
 */
class PlainTxt2FolderExporter : virtual public IExporter
{
 public:
  virtual ~PlainTxt2FolderExporter();
  PlainTxt2FolderExporter() = default;

  virtual bool export_db(ExportContextPtr) override;

 private:
  using ImageRecordPtr = events::events::ImageRecordPtr;
  using FilePtr = std::shared_ptr<std::fstream>;
  using tag2file = std::unordered_map<std::string, FilePtr>;
  using irDataTmpDB =
      std::unordered_map<std::string, std::pair<unsigned int, std::string>>;

  void clear();

  FilePtr get_file(const std::string& dirPath, const std::string& tagName);

  void export_ir(const ImageRecordPtr& ir, const std::string& exportDir);
  irDataTmpDB gather_ir_data(const ImageRecordPtr& ir);
  void write_ir_data(const irDataTmpDB& tag2data, const std::string& exportDir,
                     const std::string& imagePath);

  tag2file exFiles;
};

}  // namespace iannotator::exporters

#endif  // IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_PLAINTXT2FOLDEREXPORTER_CLASS_H
