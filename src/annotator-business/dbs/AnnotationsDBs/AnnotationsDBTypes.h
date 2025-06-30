#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_ANNOTATIONS_DB_COMPONENT_TYPES_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_ANNOTATIONS_DB_COMPONENT_TYPES_CLASS_H

#include <nlohmann/json.hpp>

#include "project-global-decls.h"
#include "src/annotator-events/events/ImageRecord.h"

namespace iannotator::dbs::annotations
{

/**
 * @brief The annotator annotations db types aliases.
 */
class AnnotationsDBTypes
{
 public:
  using ImageRecordsSet = events::events::ImageRecordsSet;

  AnnotationsDBTypes() = default;
  virtual ~AnnotationsDBTypes() = default;
};

}  // namespace iannotator::dbs::annotations

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_ANNOTATIONS_DB_COMPONENT_TYPES_CLASS_H
