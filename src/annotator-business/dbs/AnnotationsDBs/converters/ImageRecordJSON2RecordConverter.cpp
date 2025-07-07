#include "src/annotator-business/dbs/AnnotationsDBs/converters/ImageRecordJSON2RecordConverter.h"

#include <cassert>
#include <nlohmann/json.hpp>

#include "project-global-decls.h"
#include "src/annotator-business/dbs/AnnotationsDBs/AnnotationsDBTypes.h"
#include "src/annotator-business/dbs/AnnotationsDBs/AnnotationsJSONSerializator.h"
#include "src/annotator-business/dbs/AnnotationsDBs/converters/ConvertersAliases.h"
#include "src/annotator-business/dbs/AnnotationsDBs/converters/ImageRecordRectJSON2RecordConverter.h"
#include "src/annotator-events/events/EventsFactory.h"
#include "src/annotator-events/events/ImageRecord.h"
#include "src/annotator-events/events/ImageRecordRect.h"
#include "src/log/log.h"

namespace iannotator::dbs::annotations::converters
{

ImageRecordJSON2RecordConverter::ImageRecordJSON2RecordConverter()
    : efactory{std::make_shared<EventsFactory>()}
{
  assert(efactory != nullptr);
}

ImageRecordJSON2RecordConverter::ImageRecordJSON2RecordConverter(
    std::shared_ptr<EventsFactory> factory)
    : efactory{factory}
{
  assert(efactory != nullptr);
}

ImageRecordJSON2RecordConverter::ImageRecordPtr
ImageRecordJSON2RecordConverter::convert(const nlohmann::json& fan,
                                         const std::string& absdirpath)
{
  assert(fan.contains(frel_path));
  assert(fan.contains(fann));
  assert(fan.contains(fannIScale));

  const auto& relPath = fan[frel_path].get<std::string>();
  const auto& arp = absdirpath + "/" + relPath;

  LOGT("Creating image record for path: " << arp);

  auto ir = efactory->create_image_record(relPath, absdirpath);

  if (fan.contains(fannIScale)) {
    LOGT("image scale factor: " << fan[fannIScale].get<double>());

    ir->imageScale = fan[fannIScale].get<double>();
  }

  assert(ir != nullptr);

  auto rectConv =
      std::make_shared<ImageRecordRectJSON2RecordConverter>(efactory);

  for (const auto& rect : fan[fann]) {
    auto irr = rectConv->convert(rect);

    assert(irr != nullptr);

    if (irr == nullptr) {
      continue;
    }

    ir->rects.insert(irr);
  }

  return ir;
}

}  // namespace iannotator::dbs::annotations::converters
