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

#include "src/annotator-business/dbs/AnnotationsDBs/converters/ImageRecordJSON2RecordConverter.h"

#include <cassert>
#include <nlohmann/json.hpp>

#include "project-global-decls.h"
#include "src/annotator-business/dbs/AnnotationsDBs/AnnotationsDBTypes.h"
#include "src/annotator-business/dbs/AnnotationsDBs/AnnotationsJSONSerializator.h"
#include "src/annotator-business/dbs/AnnotationsDBs/converters/ConvertersAliases.h"
#include "src/annotator-business/dbs/AnnotationsDBs/converters/ImageRecordRectJSON2RecordConverter.h"
#include "src/annotator-events/events/EventsFactory.h"
#include "src/annotator-events/events/ImageRecord.h"
#include "src/annotator-events/events/ImageRecordRect.h"
#include "src/log/log.h"

namespace iannotator::dbs::annotations::converters
{

ImageRecordJSON2RecordConverter::ImageRecordJSON2RecordConverter()
    : efactory{std::make_shared<EventsFactory>()}
{
  assert(efactory != nullptr);
}

ImageRecordJSON2RecordConverter::ImageRecordJSON2RecordConverter(
    std::shared_ptr<EventsFactory> factory)
    : efactory{factory}
{
  assert(efactory != nullptr);
}

ImageRecordJSON2RecordConverter::ImageRecordPtr
ImageRecordJSON2RecordConverter::convert(const nlohmann::json& fan,
                                         const std::string& absdirpath)
{
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

  auto rectConv =
      std::make_shared<ImageRecordRectJSON2RecordConverter>(efactory);

  ir->rects.reserve(fan[fann].size());

  for (const auto& rect : fan[fann]) {
    auto irr = rectConv->convert(rect);

    assert(irr != nullptr);

    if (irr == nullptr) {
      continue;
    }

    ir->rects.emplace_back(irr);
  }

  return ir;
}

}  // namespace iannotator::dbs::annotations::converters
