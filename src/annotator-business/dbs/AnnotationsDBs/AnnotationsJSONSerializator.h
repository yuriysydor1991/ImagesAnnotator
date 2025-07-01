#ifndef IMAGEANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_JSON_DB_SERIALIZATOR_CLASS_H
#define IMAGEANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_JSON_DB_SERIALIZATOR_CLASS_H

#include <nlohmann/json.hpp>

#include "project-global-decls.h"
#include "src/annotator-events/events/ImageRecord.h"

namespace iannotator::dbs::annotations
{

/**
 * @brief The annotator annotations dir db JSON data serializator.
 */
class AnnotationsJSONSerializator
{
 public:
  virtual ~AnnotationsJSONSerializator() = default;
  AnnotationsJSONSerializator() = default;

  virtual bool serialize(const nlohmann::json& json);

  inline static const std::string root_app_name = project_decls::PROJECT_NAME;
  inline static constexpr const char* const annotations_field_name =
      "annotations";
  inline static constexpr const char* const file_version_field_name = "version";
  inline static constexpr const char* const annotation_db_field_name = "db";
  inline static constexpr const char* const annotations_db_path_field_name =
      "path";

  inline static constexpr const char* const annotation_recerod_rel_path_field =
      "rel_path";

  inline static constexpr const char* const annotation_rect_name = "name";
  inline static constexpr const char* const annotation_rect = "rect";
  inline static constexpr const char* const annotation_rect_x = "x";
  inline static constexpr const char* const annotation_rect_y = "y";
  inline static constexpr const char* const annotation_rect_width = "width";
  inline static constexpr const char* const annotation_rect_height = "height";

 protected:
  virtual bool serialize_app_info(const nlohmann::json& json);
  virtual bool serialize_annotations_records(const nlohmann::json& json);
  virtual bool serialize_annotations_record(const nlohmann::json& arecords);
};

}  // namespace iannotator::dbs::annotations

#endif  // IMAGEANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_JSON_DB_SERIALIZATOR_CLASS_H
