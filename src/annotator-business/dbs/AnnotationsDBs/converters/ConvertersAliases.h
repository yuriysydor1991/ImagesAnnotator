#ifndef IMAGEANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_CONVERTERS_ALIASES_CLASS_H
#define IMAGEANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_CONVERTERS_ALIASES_CLASS_H

#include <nlohmann/json.hpp>

#include "project-global-decls.h"
#include "src/annotator-business/dbs/AnnotationsDBs/AnnotationsDBTypes.h"
#include "src/annotator-business/dbs/AnnotationsDBs/AnnotationsJSONSerializator.h"
#include "src/annotator-events/events/EventsFactory.h"
#include "src/annotator-events/events/ImageRecord.h"
#include "src/annotator-events/events/ImageRecordRect.h"

namespace iannotator::dbs::annotations::converters
{

/**
 * @brief The converters classes types holder.
 */
class ConvertersAliases
{
 public:
  using EventsFactory = events::events::EventsFactory;
  using ImageRecordRect = events::events::ImageRecordRect;
  using ImageRecord = events::events::ImageRecord;
  using ImageRecordPtr = events::events::ImageRecordPtr;
  using ImageRecordRectSet = events::events::ImageRecordRectSet;

  inline static const auto& fdb =
      AnnotationsJSONSerializator::annotation_db_field_name;
  inline static const auto& fpath =
      AnnotationsJSONSerializator::annotations_db_path_field_name;
  inline static const auto& fann =
      AnnotationsJSONSerializator::annotations_field_name;
  inline static const auto& frel_path =
      AnnotationsJSONSerializator::annotation_recerod_rel_path_field;
  inline static const auto& fname =
      AnnotationsJSONSerializator::annotation_rect_name;
  inline static const auto& frect =
      AnnotationsJSONSerializator::annotation_rect;
  inline static const auto& fx = AnnotationsJSONSerializator::annotation_rect_x;
  inline static const auto& fy = AnnotationsJSONSerializator::annotation_rect_y;
  inline static const auto& fwidth =
      AnnotationsJSONSerializator::annotation_rect_width;
  inline static const auto& fheight =
      AnnotationsJSONSerializator::annotation_rect_height;
  inline static const auto& fannIScale =
      AnnotationsJSONSerializator::annotation_image_scale_factor;
  inline static const auto& fversion =
      AnnotationsJSONSerializator::file_version_field_name;

  virtual ~ConvertersAliases() = default;
  ConvertersAliases() = default;
};

}  // namespace iannotator::dbs::annotations::converters

#endif  // IMAGEANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_CONVERTERS_ALIASES_CLASS_H
