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

#include "src/annotator-business/dbs/AnnotationsDBs/converters/ImageRecordRectJSON2RecordConverter.h"

#include <cassert>
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

ImageRecordRectJSON2RecordConverter::ImageRecordRectJSON2RecordConverter()
    : efactory{std::make_shared<EventsFactory>()}
{
  assert(efactory != nullptr);
}

ImageRecordRectJSON2RecordConverter::ImageRecordRectJSON2RecordConverter(
    std::shared_ptr<EventsFactory> factory)
    : efactory{factory}
{
  assert(efactory != nullptr);
}

ImageRecordRectJSON2RecordConverter::ImageRecordRectPtr
ImageRecordRectJSON2RecordConverter::convert(const nlohmann::json& rect)
{
  assert(efactory != nullptr);

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

  ImageRecordRectPtr irr;

  try {
    irr = efactory->create_image_rect_record(
        rect[fname].get<std::string>(), rd[fx].get<int>(), rd[fy].get<int>(),
        rd[fwidth].get<int>(), rd[fheight].get<int>());
  }
  catch (const std::exception& e) {
    LOGE("Exception during image rect creation: " << e.what());
  }

  return irr;
}

}  // namespace iannotator::dbs::annotations::converters
