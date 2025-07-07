#include "src/annotator-business/dbs/AnnotationsDBs/converters/ImageRecordRectJSON2RecordConverter.h"

#include <cassert>
#include <nlohmann/json.hpp>

#include "project-global-decls.h"
#include "src/annotator-business/dbs/AnnotationsDBs/AnnotationsDBTypes.h"
#include "src/annotator-business/dbs/AnnotationsDBs/AnnotationsJSONSerializator.h"
#include "src/annotator-business/dbs/AnnotationsDBs/converters/ConvertersAliases.h"
#include "src/annotator-events/events/EventsFactory.h"
#include "src/annotator-events/events/ImageRecord.h"
#include "src/annotator-events/events/ImageRecordRect.h"
#include "src/log/log.h"

namespace iannotator::dbs::annotations::converters
{

ImageRecordRectJSON2RecordConverter::ImageRecordRectJSON2RecordConverter()
    : efactory{std::make_shared<EventsFactory>()}
{
  assert(efactory != nullptr);
}

ImageRecordRectJSON2RecordConverter::ImageRecordRectJSON2RecordConverter(
    std::shared_ptr<EventsFactory> factory)
    : efactory{factory}
{
  assert(efactory != nullptr);
}

ImageRecordRectJSON2RecordConverter::ImageRecordRectPtr
ImageRecordRectJSON2RecordConverter::convert(const nlohmann::json& rect)
{
  assert(efactory != nullptr);

  LOGT("The json rect: " << rect.dump(2));

  assert(rect.contains(fname));
  assert(rect.contains(frect));

  const auto& rd = rect[frect];

  assert(rd.contains(fx));
  assert(rd.contains(fy));
  assert(rd.contains(fwidth));
  assert(rd.contains(fheight));

  LOGT("Adding rect name: " << rect[fname].get<std::string>());
  LOGT("Adding rect x: " << rd[fx].get<int>());
  LOGT("Adding rect y: " << rd[fy].get<int>());
  LOGT("Adding rect w: " << rd[fwidth].get<int>());
  LOGT("Adding rect h: " << rd[fheight].get<int>());

  ImageRecordRectPtr irr;

  try {
    irr = efactory->create_image_rect_record(
        rect[fname].get<std::string>(), rd[fx].get<int>(), rd[fy].get<int>(),
        rd[fwidth].get<int>(), rd[fheight].get<int>());
  }
  catch (const std::exception& e) {
    LOGE("Exception during image rect creation: " << e.what());
  }

  return irr;
}

}  // namespace iannotator::dbs::annotations::converters
