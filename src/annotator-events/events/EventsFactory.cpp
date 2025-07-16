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

#include "src/annotator-events/events/EventsFactory.h"

#include <cassert>
#include <memory>
#include <set>

#include "src/annotator-events/events/AnnotationsDirChanged.h"
#include "src/annotator-events/events/AnnotationsDirChangedIHandler.h"
#include "src/annotator-events/events/ImagesDirChanged.h"
#include "src/annotator-events/events/ImagesDirChangedIHandler.h"
#include "src/log/log.h"

namespace events::events
{

std::shared_ptr<EventsFactory> EventsFactory::instance()
{
  static std::shared_ptr<EventsFactory> ins = std::make_shared<EventsFactory>();

  return ins;
}

std::shared_ptr<AnnotationsDirChanged>
EventsFactory::create_annotations_dir_changed(const std::string& newPath)
{
  if (newPath.empty()) {
    LOGE("Annotation db path is empty");
    return {};
  }

  return std::make_shared<AnnotationsDirChanged>(newPath);
}

std::shared_ptr<ImagesDirChanged> EventsFactory::create_image_dir_changed(
    const std::string& newPath)
{
  assert(!newPath.empty());

  if (newPath.empty()) {
    LOGE("Image db path is empty");
    return {};
  }

  return std::make_shared<ImagesDirChanged>(newPath);
}

std::shared_ptr<RequestImagesDirProvider>
EventsFactory::create_image_dir_object_request()
{
  return std::make_shared<RequestImagesDirProvider>();
}

std::shared_ptr<ImagesDirProviderChanged>
EventsFactory::create_images_dir_provider_changed(
    std::shared_ptr<ImagesPathsDBProvider> np)
{
  assert(np != nullptr);

  if (np == nullptr) {
    LOGE("No valid images db provider pointer provided");
    return {};
  }

  return std::make_shared<ImagesDirProviderChanged>(np);
}

std::shared_ptr<CurrentImageChanged>
EventsFactory::create_current_image_changed(std::shared_ptr<ImageRecord> ir)
{
  assert(ir != nullptr);

  if (ir == nullptr) {
    LOGE("No valid image record provided");
    return {};
  }

  return std::make_shared<CurrentImageChanged>(ir);
}

std::shared_ptr<ImageRecordRect> EventsFactory::create_image_rect_record(
    const ImageRecordPtr& ir)
{
  auto rt = std::make_shared<ImageRecordRect>();

  if (ir == nullptr) {
    rt->name = default_name_record;
  } else {
    rt->name =
        default_numbered_name_record_prefix + std::to_string(ir->rects.size());
  }

  return rt;
}

std::shared_ptr<ImageRecordRect> EventsFactory::create_image_rect_record(
    const std::string& npath, const int& nx, const int& ny, const int& nwidth,
    const int& nheight)
{
  return std::make_shared<ImageRecordRect>(npath, nx, ny, nwidth, nheight);
}

std::shared_ptr<ImageRecord> EventsFactory::create_image_record(
    const std::string& nirpath, const std::string& nabsPath)
{
  return std::make_shared<ImageRecord>(nirpath, nabsPath);
}

std::shared_ptr<StoreRequest> EventsFactory::create_store_request(
    const std::string& newdbpath)
{
  return std::make_shared<StoreRequest>(newdbpath);
}

std::shared_ptr<CloseCurrentProject> EventsFactory::create_close_event()
{
  return std::make_shared<CloseCurrentProject>();
}

ExportPlainTxt2FolderRequestPtr EventsFactory::create_plain_txt_2_folder_export(
    const std::string& eDirPath)
{
  return std::make_shared<ExportPlainTxt2FolderRequest>(eDirPath);
}

ExportYolo4FolderRequestPtr EventsFactory::create_yolo4_export_request(
    const std::string& exportDirPath)
{
  return std::make_shared<ExportYolo4FolderRequest>(exportDirPath);
}

Export2PyTorchVisionRequestPtr
EventsFactory::create_pytorch_vision_export_request(
    const std::string& exportDirPath)
{
  return std::make_shared<Export2PyTorchVisionRequest>(exportDirPath);
}

DeleteCurrentImageRequestPtr EventsFactory::create_delete_current_image_request(
    const std::string& irFullPath)
{
  return std::make_shared<DeleteCurrentImageRequest>(irFullPath);
}

}  // namespace events::events
