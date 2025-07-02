#include "src/annotator-business/dbs/AnnotationsDBs/AnnotationsDirDB.h"

#include <exception>
#include <fstream>
#include <nlohmann/json.hpp>

#include "src/annotator-business/dbs/AnnotationsDBs/AnnotationsJSONSerializator.h"
#include "src/annotator-business/dbs/AnnotationsDBs/converters/ImageRecords2JSONConverter.h"
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

  current_db_path = fpath;

  try {
    LOGT("Trying to open the file: " << fpath);

    std::ifstream f(fpath);

    if (!f.is_open()) {
      const int cerrno = errno;
      LOGE("Can't open file: " << fpath << " because " << strerror(cerrno));
      return false;
    }

    LOGT("Trying to parse the given db json");

    json = nlohmann::json::parse(f);

    if (!serialize()) {
      LOGE("Fail to serialize the JSON: " << json.dump());
      return false;
    }

    irdb = load_the_irs();

    LOGT("Now DB contains " << irdb.size() << " records");
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

AnnotationsDirDB::ImageRecordsSet AnnotationsDirDB::load_the_irs()
{
  LOGT("Trying to load the ImageRecords into the memory");

  static const auto& fannotations =
      AnnotationsJSONSerializator::annotations_field_name;

  assert(json.contains(fannotations));

  auto converter = std::make_shared<converters::JSON2ImageRecordsConverter>();

  return converter->fetch_records(json[fannotations]);
}

AnnotationsDirDB::ImageRecordsSet& AnnotationsDirDB::get_images_db()
{
  return irdb;
}

void AnnotationsDirDB::add_images_db(const ImageRecordsSet& andb)
{
  irdb.insert(andb.begin(), andb.end());
}

bool AnnotationsDirDB::store_db() { return store_db(current_db_path); }

bool AnnotationsDirDB::store_db(const std::string& fpath)
{
  auto irsConverter =
      std::make_shared<converters::ImageRecords2JSONConverter>();

  LOGD("Trying to store annotations to " << fpath);

  return irsConverter->store(get_images_db(), fpath);
}

}  // namespace iannotator::dbs::annotations
