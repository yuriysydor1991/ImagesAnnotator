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
