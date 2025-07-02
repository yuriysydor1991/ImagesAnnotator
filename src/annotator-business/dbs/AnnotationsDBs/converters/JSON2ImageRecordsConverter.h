#ifndef IMAGEANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_JSON_2_IMAGERECORD_CONVERTER_CLASS_H
#define IMAGEANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_JSON_2_IMAGERECORD_CONVERTER_CLASS_H

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
class JSON2ImageRecordsConverter : public virtual AnnotationsDBTypes,
                                   virtual public ConvertersAliases
{
 public:
  virtual ~JSON2ImageRecordsConverter() = default;
  JSON2ImageRecordsConverter();

  virtual ImageRecordsSet fetch_records(const nlohmann::json& allAJSon);

 private:
  std::shared_ptr<EventsFactory> efactory;
};

}  // namespace iannotator::dbs::annotations::converters

#endif  // IMAGEANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_JSON_2_IMAGERECORD_CONVERTER_CLASS_H
