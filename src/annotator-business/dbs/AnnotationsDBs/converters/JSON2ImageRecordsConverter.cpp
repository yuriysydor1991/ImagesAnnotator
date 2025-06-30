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

  for (const auto& afolder : allAJSon) {
    assert(afolder.contains(fdb));
    assert(afolder[fdb].contains(fpath));
    assert(!afolder[fdb][fpath].get<std::string>().empty());
    assert(afolder[fdb][fpath].contains(fann));

    const std::string& absdirpath = afolder[fdb][fpath].get<std::string>();

    if (!absdirpath.empty()) {
      LOGE("No abs path for the db record");
      return rset;
    }

    const auto& annjson = afolder[fann];

    for (const auto& fan : annjson) {
      assert(fan.contains(frel_path));
      assert(fan.contains(fann));

      const auto& arp = absdirpath + "/" + fan[frel_path].get<std::string>();

      auto ir = efactory->create_image_record(arp);

      assert(ir != nullptr);

      for (const auto& rect : fan[fann]) {
        assert(rect.contains(fname));
        assert(rect.contains(fx));
        assert(rect.contains(fy));
        assert(rect.contains(fwidth));
        assert(rect.contains(fheight));

        const auto irr = efactory->create_image_rect_record(
            rect[fname].get<std::string>(), rect[fx].get<int>(),
            rect[fy].get<int>(), rect[fwidth].get<int>(),
            rect[fheight].get<int>());

        ir->rects.insert(irr);
      }
    }
  }

  return rset;
}

}  // namespace iannotator::dbs::annotations::converters
