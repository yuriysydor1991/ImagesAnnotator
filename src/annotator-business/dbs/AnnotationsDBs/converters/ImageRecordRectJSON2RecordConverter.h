#ifndef IMAGEANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_RECT_JSON_2_IMAGERECORDRECT_CONVERTER_CLASS_H
#define IMAGEANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_RECT_JSON_2_IMAGERECORDRECT_CONVERTER_CLASS_H

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
 * @brief The image record JSON data to ImageRecordsSet converter.
 */
class ImageRecordRectJSON2RecordConverter : public virtual AnnotationsDBTypes,
                                            virtual public ConvertersAliases
{
 public:
  using ImageRecordRectPtr = events::events::ImageRecordRectPtr;

  virtual ~ImageRecordRectJSON2RecordConverter() = default;
  ImageRecordRectJSON2RecordConverter();
  explicit ImageRecordRectJSON2RecordConverter(
      std::shared_ptr<EventsFactory> factory);

  virtual ImageRecordRectPtr convert(const nlohmann::json& rect);

 private:
  std::shared_ptr<EventsFactory> efactory;
};

}  // namespace iannotator::dbs::annotations::converters

#endif  // IMAGEANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_RECT_JSON_2_IMAGERECORDRECT_CONVERTER_CLASS_H
