#ifndef IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_PLAINTXT2FOLDEREXPORTER_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_PLAINTXT2FOLDEREXPORTER_CLASS_H

#include <fstream>
#include <memory>
#include <unordered_map>

#include "src/annotator-business/exporters/IExporter.h"

namespace iannotator::exporters
{

/**
 * @brief The annotator business controller class.
 */
class PlainTxt2FolderExporter : virtual public IExporter
{
 public:
  virtual ~PlainTxt2FolderExporter() = default;
  PlainTxt2FolderExporter() = default;

  virtual bool export_db(ExportContextPtr) override;

 private:
  using FilePtr = std::shared_ptr<std::fstream>;
  using tag2file = std::unordered_map<std::string, FilePtr>;
  using irDataTmpDB =
      std::unordered_map<std::string, std::pair<unsigned int, std::string>>;

  void clear();

  FilePtr get_file(const std::string& dirPath, const std::string& tagName);

  tag2file exFiles;
};

}  // namespace iannotator::exporters

#endif  // IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_PLAINTXT2FOLDEREXPORTER_CLASS_H
