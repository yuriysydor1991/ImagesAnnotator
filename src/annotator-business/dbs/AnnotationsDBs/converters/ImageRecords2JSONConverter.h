#ifndef IMAGEANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_IMAGERECORD_2_JSON_CONVERTER_CLASS_H
#define IMAGEANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_IMAGERECORD_2_JSON_CONVERTER_CLASS_H

#include <nlohmann/json.hpp>

#include "project-global-decls.h"
#include "src/annotator-business/dbs/AnnotationsDBs/AnnotationsDBTypes.h"
#include "src/annotator-business/dbs/AnnotationsDBs/AnnotationsJSONSerializator.h"
#include "src/annotator-business/dbs/AnnotationsDBs/converters/ConvertersAliases.h"
#include "src/annotator-events/events/EventsFactory.h"
#include "src/annotator-events/events/ImageRecord.h"
#include "src/annotator-events/events/ImageRecordRect.h"

namespace iannotator::dbs::annotations::converters
{

/**
 * @brief The annotator annotations JSON data to ImageRecordsSet converter.
 */
class ImageRecords2JSONConverter : public virtual AnnotationsDBTypes,
                                   virtual public ConvertersAliases
{
 public:
  virtual ~ImageRecords2JSONConverter() = default;
  ImageRecords2JSONConverter() = default;

  virtual bool store(const ImageRecordsSet& irs, const std::string& filepath);

 protected:
  virtual nlohmann::json convert(const ImageRecordsSet& irs);
  virtual nlohmann::json convert(const ImageRecordPtr& ir);
  virtual nlohmann::json convert(const ImageRecordRectSet& rects);
};

}  // namespace iannotator::dbs::annotations::converters

#endif  // IMAGEANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_IMAGERECORD_2_JSON_CONVERTER_CLASS_H
