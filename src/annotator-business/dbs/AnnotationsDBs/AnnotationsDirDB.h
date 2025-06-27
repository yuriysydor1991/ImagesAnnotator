#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_ANNOTATIONS_DB_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_ANNOTATIONS_DB_CLASS_H

#include <nlohmann/json.hpp>

namespace iannotator::dbs::annotations
{

/**
 * @brief The annotator annotations dir db controller.
 */
class AnnotationsDirDB
{
 public:
  virtual ~AnnotationsDirDB() = default;
  AnnotationsDirDB() = default;

  virtual bool load_db(const std::string& fpath);

  virtual 

 private:
  nlohmann::json json;
};

}  // namespace iannotator::dbs::annotations

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_ANNOTATIONS_DB_CLASS_H
