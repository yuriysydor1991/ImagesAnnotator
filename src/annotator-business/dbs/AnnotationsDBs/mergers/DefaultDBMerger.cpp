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

#include "src/annotator-business/dbs/AnnotationsDBs/mergers/DefaultDBMerger.h"

#include "project-global-decls.h"
#include "src/annotator-business/dbs/AnnotationsDBs/AnnotationsDBTypes.h"
#include "src/annotator-events/events/ImageRecord.h"
#include "src/helpers/ImageRecordUrlAndPathHelper.h"
#include "src/helpers/SortHelper.h"
#include "src/log/log.h"

namespace iannotator::dbs::annotations::merger
{

void DefaultDBMerger::merge(ImageRecordsSet& dst, const ImageRecordsSet& append)
{
  LOGT("Reserving for merge " << dst.capacity() << " "
                              << (dst.size() + append.size()));

  dst.reserve(dst.size() + append.size());

  LOGT("Performing copy with conditions operation");
  std::copy_if(
      append.cbegin(), append.cend(), std::back_inserter(dst),
      [&dst](const ImageRecordPtr& ir) {
        const auto irp = helpers::ImageRecordUrlAndPathHelper::get_ir_path(ir);
        const auto fiter = std::find_if(
            dst.cbegin(), dst.cend(), [&irp](const ImageRecordPtr& dstir) {
              const auto dstp =
                  helpers::ImageRecordUrlAndPathHelper::get_ir_path(dstir);
              return irp == dstp;
            });
        return fiter == dst.cend();
      });

  LOGT("Sorting the destination images records db");
  std::sort(dst.begin(), dst.end(), get_image_records_sorter());
}

DefaultDBMergerPtr DefaultDBMerger::create()
{
  return std::make_shared<DefaultDBMerger>();
}

}  // namespace iannotator::dbs::annotations::merger
