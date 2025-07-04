#ifndef IMAGEANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_ANNOTATIONS_DB_CLASS_H
#define IMAGEANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_ANNOTATIONS_DB_CLASS_H

#include <nlohmann/json.hpp>

#include "project-global-decls.h"
#include "src/annotator-business/dbs/AnnotationsDBs/AnnotationsDBTypes.h"
#include "src/annotator-events/events/ImageRecord.h"
#include "src/annotator-events/events/ImagesPathsDBProvider.h"

namespace iannotator::dbs::annotations
{

/**
 * @brief The annotator annotations dir db controller.
 */
class AnnotationsDirDB : public virtual AnnotationsDBTypes,
                         virtual public events::events::ImagesPathsDBProvider
{
 public:
  using AnnotationsList =
      events::events::ImagesPathsDBProvider::AnnotationsList;

  virtual ~AnnotationsDirDB() = default;
  AnnotationsDirDB() = default;

  virtual bool load_db(const std::string& fpath);

  virtual ImageRecordsSet& get_images_db() override;
  virtual std::string get_db_path() override;
  virtual AnnotationsList get_available_annotations() override;

  virtual void add_images_db(const ImageRecordsSet& andb);

  virtual bool store_db();
  virtual bool store_db(const std::string& fpath);

 protected:
  virtual bool serialize();
  virtual ImageRecordsSet load_the_irs();

  nlohmann::json json;
  ImageRecordsSet irdb;
  std::string current_db_path;
};

}  // namespace iannotator::dbs::annotations

#endif  // IMAGEANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_ANNOTATIONS_DB_CLASS_H
