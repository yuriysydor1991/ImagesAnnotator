#include "src/annotator-business/dbs/AnnotationsDBs/AnnotationsDirDB.h"

#include <exception>
#include <fstream>
#include <nlohmann/json.hpp>

#include "src/annotator-business/dbs/AnnotationsDBs/AnnotationsJSONSerializator.h"
#include "src/annotator-business/dbs/AnnotationsDBs/converters/JSON2ImageRecordsConverter.h"
#include "src/log/log.h"

namespace iannotator::dbs::annotations
{

bool AnnotationsDirDB::load_db(const std::string& fpath)
{
  assert(fpath != nullptr);

  if (fpath.empty()) {
    LOGE("No filepath is given");
    return false;
  }

  try {
    std::ifstream f(fpath);

    if (!f.is_open()) {
      const int cerrno = errno;
      LOGE("Can't open file: " << fpath << " because " << strerror(cerrno));
      return false;
    }

    json = nlohmann::json::parse(f);

    if (!serialize()) {
      LOGE("Fail to serialize the JSON: " << json.dump());
      return false;
    }
  }
  catch (const std::exception& e) {
    LOGE("Exception occurred: " << e.what());
    return false;
  }

  return true;
}

bool AnnotationsDirDB::serialize()
{
  auto s = std::make_shared<AnnotationsJSONSerializator>();

  return s->serialize(json);
}

AnnotationsDirDB::ImageRecordsSet AnnotationsDirDB::get_image_records()
{
  const auto& fannotations =
      AnnotationsJSONSerializator::annotations_field_name;

  assert(json.contains(fannotations));

  auto converter = std::make_shared<converters::JSON2ImageRecordsConverter>();

  return converter->fetch_records(json[fannotations]);
}

}  // namespace iannotator::dbs::annotations
