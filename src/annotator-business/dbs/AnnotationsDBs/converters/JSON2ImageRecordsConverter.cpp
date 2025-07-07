#include "src/annotator-business/dbs/AnnotationsDBs/converters/JSON2ImageRecordsConverter.h"

#include <nlohmann/json.hpp>

#include "project-global-decls.h"
#include "src/annotator-business/dbs/AnnotationsDBs/converters/ImageRecordJSON2RecordConverter.h"
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
    if (!convert_folder(afolder, rset)) {
      LOGE("Fail to convert single folder");
    }
  }

  return rset;
}

bool JSON2ImageRecordsConverter::convert_folder(const nlohmann::json& afolder,
                                                ImageRecordsSet& rset)
{
  assert(afolder.contains(fdb));
  assert(afolder[fdb].contains(fpath));
  assert(!afolder[fdb][fpath].get<std::string>().empty());
  assert(afolder.contains(fann));

  const std::string& absdirpath = afolder[fdb][fpath].get<std::string>();

  if (absdirpath.empty()) {
    LOGE("No abs path for the db record");
    return false;
  }

  LOGT("Filling the images set for the " << absdirpath);

  const auto& annjson = afolder[fann];

  rset.reserve(rset.size() + annjson.size());

  for (const auto& fan : annjson) {
    auto ir = convert_annotation(fan, absdirpath);

    assert(ir != nullptr);

    if (ir == nullptr) {
      LOGE("Failure during conversion of the image record: " << fan.dump(2));
      continue;
    }

    rset.emplace_back(ir);
  }

  return true;
}

JSON2ImageRecordsConverter::ImageRecordPtr
JSON2ImageRecordsConverter::convert_annotation(const nlohmann::json& fan,
                                               const std::string& absdirpath)
{
  auto irConverter =
      std::make_shared<ImageRecordJSON2RecordConverter>(efactory);

  return irConverter->convert(fan, absdirpath);
}

}  // namespace iannotator::dbs::annotations::converters
