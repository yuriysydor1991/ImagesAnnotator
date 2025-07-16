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

#ifndef IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_EVENTS_FACTORY_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_EVENTS_FACTORY_CLASS_H

#include <memory>
#include <string>

#include "src/annotator-events/events/AnnotationsDirChanged.h"
#include "src/annotator-events/events/AnnotationsDirChangedIHandler.h"
#include "src/annotator-events/events/CloseCurrentProject.h"
#include "src/annotator-events/events/CurrentImageChanged.h"
#include "src/annotator-events/events/DeleteCurrentImageRequest.h"
#include "src/annotator-events/events/Export2PyTorchVisionRequest.h"
#include "src/annotator-events/events/ExportPlainTxt2FolderRequest.h"
#include "src/annotator-events/events/ExportYolo4FolderRequest.h"
#include "src/annotator-events/events/ImageRecord.h"
#include "src/annotator-events/events/ImageRecordRect.h"
#include "src/annotator-events/events/ImagesDirChanged.h"
#include "src/annotator-events/events/ImagesDirChangedIHandler.h"
#include "src/annotator-events/events/ImagesDirProviderChanged.h"
#include "src/annotator-events/events/ImagesPathsDBProvider.h"
#include "src/annotator-events/events/RequestImagesDirProvider.h"
#include "src/annotator-events/events/StoreRequest.h"

namespace events::events
{

/**
 * @brief All the events objects factory.
 */
class EventsFactory : public std::enable_shared_from_this<EventsFactory>
{
 public:
  inline static const std::string default_name_record = "rectangle_default";
  inline static const std::string default_numbered_name_record_prefix =
      "rectangle_default_";

  virtual ~EventsFactory() = default;
  EventsFactory() = default;

  static std::shared_ptr<EventsFactory> instance();

  virtual std::shared_ptr<AnnotationsDirChanged> create_annotations_dir_changed(
      const std::string& newPath);

  virtual std::shared_ptr<ImagesDirChanged> create_image_dir_changed(
      const std::string& newPath);

  virtual std::shared_ptr<RequestImagesDirProvider>
  create_image_dir_object_request();

  virtual std::shared_ptr<ImagesDirProviderChanged>
  create_images_dir_provider_changed(std::shared_ptr<ImagesPathsDBProvider> np);

  virtual std::shared_ptr<CurrentImageChanged> create_current_image_changed(
      std::shared_ptr<ImageRecord>);

  virtual std::shared_ptr<ImageRecordRect> create_image_rect_record(
      const ImageRecordPtr& ir = nullptr);
  virtual std::shared_ptr<ImageRecordRect> create_image_rect_record(
      const std::string& npath, const int& nx, const int& ny, const int& nwidth,
      const int& nheight);

  virtual std::shared_ptr<ImageRecord> create_image_record(
      const std::string& nirpath = "", const std::string& nabsPath = "");

  virtual std::shared_ptr<StoreRequest> create_store_request(
      const std::string& newdbpath = "");

  virtual std::shared_ptr<CloseCurrentProject> create_close_event();

  virtual ExportPlainTxt2FolderRequestPtr create_plain_txt_2_folder_export(
      const std::string& eDirPath);

  virtual ExportYolo4FolderRequestPtr create_yolo4_export_request(
      const std::string& exportDirPath);

  virtual Export2PyTorchVisionRequestPtr create_pytorch_vision_export_request(
      const std::string& exportDirPath);

  virtual DeleteCurrentImageRequestPtr create_delete_current_image_request(
      const std::string& irFullPath);
};

}  // namespace events::events

#endif  // IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_EVENTS_FACTORY_CLASS_H
