#ifndef IMAGEANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_ANNOTATIONS_DB_CLASS_H
#define IMAGEANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_ANNOTATIONS_DB_CLASS_H

#include <nlohmann/json.hpp>

#include "project-global-decls.h"
#include "src/annotator-business/dbs/AnnotationsDBs/AnnotationsDBTypes.h"
#include "src/annotator-events/events/ImageRecord.h"

namespace iannotator::dbs::annotations
{

/**
 * @brief The annotator annotations dir db controller.
 */
class AnnotationsDirDB : public virtual AnnotationsDBTypes
{
 public:
  virtual ~AnnotationsDirDB() = default;
  AnnotationsDirDB() = default;

  virtual bool load_db(const std::string& fpath);

  virtual ImageRecordsSet get_image_records();

 protected:
  virtual bool serialize();
  virtual ImageRecordsSet load_the_irs();

  nlohmann::json json;
  ImageRecordsSet irdb;
};

}  // namespace iannotator::dbs::annotations

#endif  // IMAGEANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_ANNOTATIONS_DB_CLASS_H
