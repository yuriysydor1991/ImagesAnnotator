#ifndef IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_IEXPORTER_ABSTRACT_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_IEXPORTER_ABSTRACT_CLASS_H

#include <memory>

#include "src/annotator-business/exporters/ExportContext.h"

/**
 * @brief The images annotator exporter namespace that holds all annotator
 * related exporters classes.
 */
namespace iannotator::exporters
{

/**
 * @brief The abstract class to define the interface for the all available
 * annotation data exporters.
 */
class IExporter
{
 public:
  virtual ~IExporter() = default;
  IExporter() = default;

  virtual bool export_db(ExportContextPtr) = 0;
};

}  // namespace iannotator::exporters

#endif  // IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_IEXPORTER_ABSTRACT_CLASS_H
