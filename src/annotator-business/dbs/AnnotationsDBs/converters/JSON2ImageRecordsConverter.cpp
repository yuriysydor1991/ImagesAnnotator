#include "src/annotator-business/dbs/AnnotationsDBs/converters/JSON2ImageRecordsConverter.h"

#include <nlohmann/json.hpp>

#include "project-global-decls.h"
#include "src/annotator-business/dbs/AnnotationsDBs/AnnotationsDBTypes.h"
#include "src/annotator-business/dbs/AnnotationsDBs/AnnotationsJSONSerializator.h"
#include "src/annotator-business/dbs/AnnotationsDBs/converters/ImageRecordRectJSON2RecordConverter.h"
#include "src/annotator-events/events/ImageRecord.h"
#include "src/log/log.h"

namespace iannotator::dbs::annotations::converters
{

JSON2ImageRecordsConverter::JSON2ImageRecordsConverter()
    : efactory{std::make_shared<EventsFactory>()}
{
  assert(efactory != nullptr);
}

JSON2ImageRecordsConverter::ImageRecordsSet
JSON2ImageRecordsConverter::fetch_records(const nlohmann::json& allAJSon)
{
  ImageRecordsSet rset;

  LOGT("Given JSON: " << allAJSon.dump(2));

  auto rectConv =
      std::make_shared<ImageRecordRectJSON2RecordConverter>(efactory);

  for (const auto& afolder : allAJSon) {
    assert(afolder.contains(fdb));
    assert(afolder[fdb].contains(fpath));
    assert(!afolder[fdb][fpath].get<std::string>().empty());
    assert(afolder.contains(fann));

    const std::string& absdirpath = afolder[fdb][fpath].get<std::string>();

    if (absdirpath.empty()) {
      LOGE("No abs path for the db record");
      return rset;
    }

    LOGT("Filling the images set for the " << absdirpath);

    const auto& annjson = afolder[fann];

    rset.reserve(annjson.size());

    for (const auto& fan : annjson) {
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

      rset.emplace_back(ir);

      for (const auto& rect : fan[fann]) {
        auto irr = rectConv->convert(rect);

        assert(irr != nullptr);

        if (irr == nullptr) {
          continue;
        }

        ir->rects.insert(irr);
      }
    }
  }

  return rset;
}

}  // namespace iannotator::dbs::annotations::converters
