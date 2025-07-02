#include "src/annotator-business/dbs/AnnotationsDBs/converters/JSON2ImageRecordsConverter.h"

#include <nlohmann/json.hpp>

#include "project-global-decls.h"
#include "src/annotator-business/dbs/AnnotationsDBs/AnnotationsDBTypes.h"
#include "src/annotator-business/dbs/AnnotationsDBs/AnnotationsJSONSerializator.h"
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

  for (const auto& afolder : allAJSon) {
    assert(afolder.contains(fdb));
    assert(afolder[fdb].contains(fpath));
    assert(!afolder[fdb][fpath].get<std::string>().empty());
    assert(afolder[fdb][fpath].contains(fann));

    const std::string& absdirpath = afolder[fdb][fpath].get<std::string>();

    if (absdirpath.empty()) {
      LOGE("No abs path for the db record");
      return rset;
    }

    LOGT("Filling the images set for the " << absdirpath);

    const auto& annjson = afolder[fann];

    for (const auto& fan : annjson) {
      assert(fan.contains(frel_path));
      assert(fan.contains(fann));
      assert(fan.contains(fannIScale));

      const auto& arp = absdirpath + "/" + fan[frel_path].get<std::string>();

      LOGT("Creating image record for path: " << arp);

      auto ir = efactory->create_image_record(arp);

      LOGT("image scale factor: " << fan[fannIScale].get<double>());

      ir->imageScale = fan[fannIScale].get<double>();

      assert(ir != nullptr);

      rset.insert(ir);

      for (const auto& rect : fan[fann]) {
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

        const auto irr = efactory->create_image_rect_record(
            rect[fname].get<std::string>(), rd[fx].get<int>(),
            rd[fy].get<int>(), rd[fwidth].get<int>(), rd[fheight].get<int>());

        ir->rects.insert(irr);
      }
    }
  }

  return rset;
}

}  // namespace iannotator::dbs::annotations::converters
