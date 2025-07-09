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

#include "src/annotator-business/dbs/AnnotationsDBs/converters/ImageRecords2JSONConverter.h"

#include <cassert>
#include <fstream>
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

bool ImageRecords2JSONConverter::store(const ImageRecordsSet& irs,
                                       const std::string& filepath)
{
  assert(!filepath.empty());

  if (filepath.empty()) {
    LOGE("Given filepath is empty");
    return false;
  }

  std::ofstream file(filepath.c_str());

  if (!file.is_open()) {
    LOGE("Fail to open file: " << filepath);
    return false;
  }

  nlohmann::json j;

  j[fann] = convert(irs);
  j[project_decls::PROJECT_NAME][fversion] =
      project_decls::PROJECT_BUILD_VERSION;

  file << j.dump(2);

  file.close();

  return true;
}

nlohmann::json ImageRecords2JSONConverter::convert(const ImageRecordsSet& irs)
{
  nlohmann::json j = nlohmann::json::array();

  std::map<std::string, std::vector<nlohmann::json>> abs2json;

  for (const auto& ir : irs) {
    if (abs2json.find(ir->abs_dir_path) == abs2json.end()) {
      LOGT("Introducing the new abs path " << ir->abs_dir_path);
      abs2json[ir->abs_dir_path].reserve(irs.size());
    }

    LOGT("Trying to convert the " << ir->path);

    abs2json[ir->abs_dir_path].emplace_back(convert(ir));
  }

  for (const auto& p : abs2json) {
    nlohmann::json jj;

    jj[fdb][fpath] = p.first;
    jj[fann] = nlohmann::json::array();

    for (const auto& irj : p.second) {
      jj[fann].emplace_back(irj);
    }

    j.push_back(jj);
  }

  return j;
}

nlohmann::json ImageRecords2JSONConverter::convert(const ImageRecordPtr& ir)
{
  nlohmann::json j;

  j[frel_path] = ir->path;
  j[fannIScale] = ir->imageScale;
  j[fiwidth] = ir->iwidth;
  j[fiheight] = ir->iheight;
  j[fann] = convert(ir->rects);

  LOGT("The IR json: " << j.dump(2));

  return j;
}

nlohmann::json ImageRecords2JSONConverter::convert(
    const ImageRecordRectSet& rects)
{
  nlohmann::json j = nlohmann::json::array();

  for (const auto& r : rects) {
    nlohmann::json jr;

    LOGT("Converting " << r->name << " " << r->x << "/" << r->y << "/"
                       << r->width << "/" << r->height);

    jr[fname] = r->name;
    jr[frect][fx] = r->x;
    jr[frect][fy] = r->y;
    jr[frect][fwidth] = r->width;
    jr[frect][fheight] = r->height;

    j.emplace_back(jr);
  }

  return j;
}

}  // namespace iannotator::dbs::annotations::converters
