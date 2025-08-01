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

#include "src/annotator-business/dbs/AnnotationsDBs/AnnotationsJSONSerializator.h"

#include <nlohmann/json.hpp>

#include "project-global-decls.h"
#include "src/annotator-business/dbs/AnnotationsDBs/converters/JSON2ImageRecordsConverter.h"
#include "src/annotator-events/events/ImageRecord.h"
#include "src/log/log.h"

namespace iannotator::dbs::annotations
{

bool AnnotationsJSONSerializator::serialize(const nlohmann::json& json)
{
  return serialize_app_info(json) && serialize_annotations_records(json);
}

bool AnnotationsJSONSerializator::serialize_app_info(const nlohmann::json& json)
{
  if (!json.contains(root_app_name)) {
    LOGE("Containing JSON has no " << root_app_name << " root element inside");
    return false;
  }

  const auto& filesAppV =
      json[root_app_name][file_version_field_name].get<std::string>();

  LOGI("file's " << root_app_name << " app version is: " << filesAppV);

  if (filesAppV != project_decls::PROJECT_BUILD_VERSION) {
    LOGW("Current app version and files's ones missmatch: "
         << filesAppV << " != " << project_decls::PROJECT_BUILD_VERSION);
  }

  if (!json.contains(annotations_field_name)) {
    LOGE("Containing JSON has no " << annotations_field_name
                                   << " root element inside");
    return false;
  }

  return true;
}

bool AnnotationsJSONSerializator::serialize_annotations_records(
    const nlohmann::json& json)
{
  LOGI("Count of the root annotations records: "
       << json[annotations_field_name].size());

  for (const auto& arecords : json[annotations_field_name]) {
    if (!serialize_annotations_record(arecords)) {
      return false;
    }
  }

  return true;
}

bool AnnotationsJSONSerializator::serialize_annotations_record(
    const nlohmann::json& arecords)
{
  if (!arecords.contains(annotation_db_field_name)) {
    LOGE("Containing JSON annotation record has no " << annotation_db_field_name
                                                     << " element inside");
    return false;
  }

  if (!arecords.contains(annotations_field_name)) {
    LOGE("Containing JSON annotation record has no " << annotations_field_name
                                                     << " element inside");
    return false;
  }

  const auto& db = arecords[annotation_db_field_name];

  if (!db.contains(annotations_db_path_field_name)) {
    LOGE("Containing JSON annotation db record has no "
         << annotation_db_field_name << " element inside");
    return false;
  }

  const auto& dbdir = db[annotations_db_path_field_name].get<std::string>();

  if (dbdir.empty()) {
    LOGE("Empty root database dir abs path");
    return false;
  }

  LOGI("Annotation db source directory: " << dbdir);
  LOGI("has: " << arecords[annotations_field_name].size() << " annotations");

  return true;
}

}  // namespace iannotator::dbs::annotations
