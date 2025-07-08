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

#ifndef IMAGEANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_ANNOTATIONS_DB_CLASS_H
#define IMAGEANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_ANNOTATIONS_DB_CLASS_H

#include <nlohmann/json.hpp>

#include "project-global-decls.h"
#include "src/annotator-business/dbs/AnnotationsDBs/AnnotationsDBTypes.h"
#include "src/annotator-events/events/ImageRecord.h"
#include "src/annotator-events/events/ImagesPathsDBProvider.h"
#include "src/helpers/SortHelper.h"

namespace iannotator::dbs::annotations
{

/**
 * @brief The annotator annotations dir db controller.
 */
class AnnotationsDirDB : public virtual AnnotationsDBTypes,
                         virtual public events::events::ImagesPathsDBProvider,
                         virtual public helpers::SortHelper
{
 public:
  using AnnotationsList =
      events::events::ImagesPathsDBProvider::AnnotationsList;

  virtual ~AnnotationsDirDB() = default;
  AnnotationsDirDB();

  virtual bool load_db(const std::string& fpath);

  virtual ImageRecordsSet& get_images_db() override;
  virtual std::string get_db_path() override;
  virtual AnnotationsList get_available_annotations() override;

  virtual bool changed() override;

  virtual void add_images_db(const ImageRecordsSet& andb);

  virtual bool store_db();
  virtual bool store_db(const std::string& fpath);

 protected:
  virtual bool serialize();
  virtual ImageRecordsSet load_the_irs();

  void update_current_last_saved();

  nlohmann::json json;
  ImageRecordsSet irdb;
  std::string current_db_path;

  ImageRecordsSet last_saved_db;
};

using AnnotationsDirDBPtr = std::shared_ptr<AnnotationsDirDB>;

}  // namespace iannotator::dbs::annotations

#endif  // IMAGEANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_ANNOTATIONS_DB_CLASS_H
