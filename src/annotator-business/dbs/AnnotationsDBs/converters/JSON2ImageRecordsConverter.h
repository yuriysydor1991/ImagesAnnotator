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

#ifndef IMAGEANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_JSON_2_IMAGERECORD_CONVERTER_CLASS_H
#define IMAGEANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_JSON_2_IMAGERECORD_CONVERTER_CLASS_H

#include <nlohmann/json.hpp>

#include "project-global-decls.h"
#include "src/annotator-business/dbs/AnnotationsDBs/AnnotationsDBTypes.h"
#include "src/annotator-business/dbs/AnnotationsDBs/AnnotationsJSONSerializator.h"
#include "src/annotator-business/dbs/AnnotationsDBs/converters/ConvertersAliases.h"
#include "src/annotator-events/events/EventsFactory.h"
#include "src/annotator-events/events/ImageRecord.h"
#include "src/annotator-events/events/ImageRecordRect.h"

namespace iannotator::dbs::annotations::converters
{

/**
 * @brief The annotator annotations JSON data to ImageRecordsSet converter.
 */
class JSON2ImageRecordsConverter : public virtual AnnotationsDBTypes,
                                   virtual public ConvertersAliases
{
 public:
  virtual ~JSON2ImageRecordsConverter() = default;
  JSON2ImageRecordsConverter();

  virtual ImageRecordsSet fetch_records(const nlohmann::json& allAJSon);

 protected:
  virtual bool convert_folder(const nlohmann::json& afolder,
                              ImageRecordsSet& rset);
  virtual ImageRecordPtr convert_annotation(const nlohmann::json& fan,
                                            const std::string& absdirpath);

 private:
  std::shared_ptr<EventsFactory> efactory;
};

}  // namespace iannotator::dbs::annotations::converters

#endif  // IMAGEANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_JSON_2_IMAGERECORD_CONVERTER_CLASS_H
