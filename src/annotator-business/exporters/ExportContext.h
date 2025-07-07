#ifndef IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_EXPORT_CONTEXT_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_EXPORT_CONTEXT_CLASS_H

#include <memory>
#include <string>

#include "src/annotator-events/events/ImagesPathsDBProvider.h"

namespace iannotator::exporters
{

/**
 * @brief Class to hold the export configuration data.
 */
class ExportContext
{
 public:
  using ImagesPathsDBProviderPtr = events::events::ImagesPathsDBProviderPtr;

  virtual ~ExportContext() = default;
  ExportContext() = default;

  /// @brief File or directory dst export path
  std::string export_path;

  ImagesPathsDBProviderPtr dbProvider;
};

using ExportContextPtr = std::shared_ptr<ExportContext>;

}  // namespace iannotator::exporters

#endif  // IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_EXPORT_CONTEXT_CLASS_H
