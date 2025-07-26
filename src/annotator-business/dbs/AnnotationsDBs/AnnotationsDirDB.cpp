/**
 * The "Simplified BSD License"
 *
 * Copyright (c) 2025, Yurii Sydor (yuriysydor1991@gmail.com)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   - Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "src/annotator-business/dbs/AnnotationsDBs/AnnotationsDirDB.h"

#include <algorithm>
#include <cassert>
#include <exception>
#include <fstream>
#include <nlohmann/json.hpp>

#include "src/annotator-business/dbs/AnnotationsDBs/AnnotationsJSONSerializator.h"
#include "src/annotator-business/dbs/AnnotationsDBs/converters/ImageRecords2JSONConverter.h"
#include "src/annotator-business/dbs/AnnotationsDBs/converters/JSON2ImageRecordsConverter.h"
#include "src/log/log.h"

namespace iannotator::dbs::annotations
{

AnnotationsDirDB::AnnotationsDirDB() { update_current_last_saved_no_check(); }

bool AnnotationsDirDB::load_db(const std::string& fpath)
{
  assert(!fpath.empty());

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

    LOGT("Trying to serialize retrieved file");

    if (!serialize()) {
      LOGE("Fail to serialize the JSON: " << json.dump());
      return false;
    }

    LOGT("Trying to extract the records");

    irdb = load_the_irs();

    LOGT("Now DB contains " << irdb.size() << " records");
  }
  catch (const std::exception& e) {
    LOGE("Exception occurred: " << e.what());
    return false;
  }

  update_current_last_saved();

  return true;
}

void AnnotationsDirDB::update_current_last_saved()
{
  update_current_last_saved_no_check();

  if (changed()) {
    LOGE("Duplicated images record set is not equal to original");
  }
}

void AnnotationsDirDB::update_current_last_saved_no_check()
{
  last_saved_db = ImageRecord::duplicate(irdb);

  assert(ImageRecord::equal(last_saved_db, irdb));
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
  irdb.reserve(irdb.size() + andb.size());

  irdb.insert(irdb.begin(), andb.begin(), andb.end());

  std::sort(irdb.begin(), irdb.end(), get_image_records_sorter());
}

bool AnnotationsDirDB::store_db() { return store_db(current_db_path); }

bool AnnotationsDirDB::store_db(const std::string& fpath)
{
  auto irsConverter =
      std::make_shared<converters::ImageRecords2JSONConverter>();

  LOGD("Trying to store annotations to " << fpath);

  current_db_path = fpath;

  const bool sret = irsConverter->store(get_images_db(), fpath);

  if (sret) {
    update_current_last_saved();
  } else {
    LOGE("Failure during records saving");
  }

  return sret;
}

std::string AnnotationsDirDB::get_db_path() { return current_db_path; }

AnnotationsDirDB::AnnotationsList AnnotationsDirDB::get_available_annotations()
{
  AnnotationsList alist;

  for (const auto& ir : irdb) {
    assert(ir != nullptr);

    for (const auto& rect : ir->rects) {
      assert(rect != nullptr);
      alist.insert(rect->name);
    }
  }

  return alist;
}

bool AnnotationsDirDB::changed()
{
  return !ImageRecord::equal(last_saved_db, irdb);
}

bool AnnotationsDirDB::delete_image_record(const std::string& irFullPath)
{
  auto nend =
      std::remove_if(irdb.begin(), irdb.end(), [irFullPath](ImageRecordPtr ir) {
        return ir != nullptr && ir->get_full_path() == irFullPath;
      });

  irdb.erase(nend, irdb.end());

  return true;
}

}  // namespace iannotator::dbs::annotations
