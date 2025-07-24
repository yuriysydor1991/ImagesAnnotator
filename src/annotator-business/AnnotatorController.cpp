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

#include "src/annotator-business/AnnotatorController.h"

#include <cassert>
#include <memory>

#include "src/annotator-business/dbs/AnnotationsDBs/AnnotationsDirDB.h"
#include "src/annotator-business/dbs/ImagesLoaders/ImagesDirLoader.h"
#include "src/annotator-business/dbs/ImagesLoaders/ImagesWebPageLoader.h"
#include "src/annotator-business/exporters/PlainTxt2FolderExporter.h"
#include "src/annotator-business/exporters/PyTorchVisionFolderExporter.h"
#include "src/annotator-business/exporters/Yolo42FolderExporter.h"
#include "src/app/ApplicationContext.h"
#include "src/app/IApplication.h"
#include "src/log/log.h"

namespace iannotator
{

AnnotatorController::AnnotatorController()
    : annotations{create_project_data_instance()}, actx{}
{
}

AnnotatorController::AnnotationsDirDBPtr
AnnotatorController::create_project_data_instance()
{
  return std::make_shared<dbs::annotations::AnnotationsDirDB>();
}

bool AnnotatorController::init(std::shared_ptr<app::ApplicationContext> ctx)
{
  assert(ctx != nullptr);
  assert(ctx->eventer != nullptr);
  assert(annotations != nullptr);

  if (ctx == nullptr) {
    LOGE("No valid context was provided");
    return false;
  }

  if (annotations == nullptr) {
    LOGE("No valid annotations controller pointer was provided");
    return false;
  }

  if (ctx->eventer == nullptr) {
    LOGE("No valid context' eventer was provided");
    return false;
  }

  actx = ctx;

  if (!ctx->annotation_db_path.empty()) {
    LOGD("Found initial annotations directory path: "
         << ctx->annotation_db_path);

    if (!annotations->load_db(ctx->annotation_db_path)) {
      LOGE("Fail to load directory: " << ctx->annotation_db_path);
      return false;
    }
  }

  if (!ctx->images_db_path.empty()) {
    LOGD("Found initial images directory path: " << ctx->images_db_path);

    try_to_append_images_dir(ctx->images_db_path);

    /// @todo: emit images db list changed over here
    // meaningless to send events at this point since window is not created in
    // the App class.
  }

  auto mptr = shared_from_this();

  ctx->eventer->subscribe(
      std::shared_ptr<RequestImagesDirProviderHandler>(mptr));
  ctx->eventer->subscribe(std::shared_ptr<CurrentImageChangedHandler>(mptr));
  ctx->eventer->subscribe(std::shared_ptr<AnnotationsDirChangedIHandler>(mptr));
  ctx->eventer->subscribe(std::shared_ptr<ImagesDirChangedIHandler>(mptr));
  ctx->eventer->subscribe(std::shared_ptr<StoreRequestHandler>(mptr));
  ctx->eventer->subscribe(std::shared_ptr<CloseCurrentProjectHandler>(mptr));
  ctx->eventer->subscribe(ExportPlainTxt2FolderRequestHandlerPtr{mptr});
  ctx->eventer->subscribe(ExportYolo4FolderRequestHandlerPtr{mptr});
  ctx->eventer->subscribe(Export2PyTorchVisionRequestHandlerPtr{mptr});
  ctx->eventer->subscribe(DeleteCurrentImageRequestHandlerPtr{mptr});

  return true;
}

void AnnotatorController::handle(std::shared_ptr<ImagesDirChanged> event)
{
  assert(event != nullptr);
  assert(annotations != nullptr);

  if (event == nullptr) {
    LOGE("No valid event object pointer was given");
    return;
  }

  if (annotations == nullptr) {
    LOGE("No annotations object available");
    return;
  }

  try_to_append_images_dir(event->images_dir);

  LOGD("The images db dir " << event->images_dir << " have been loaded"
                            << " submitting appropriate event");

  emitImagesProviderChanged();
}

void AnnotatorController::handle(std::shared_ptr<AnnotationsDirChanged> event)
{
  assert(event != nullptr);
  assert(annotations != nullptr);

  if (event == nullptr) {
    LOGE("No valid event object pointer was given");
    return;
  }

  if (annotations == nullptr) {
    LOGE("No valid annotations db controller object pointer found");
    return;
  }

  if (!annotations->load_db(event->annotations_dir)) {
    LOGE("Fail to load the annotations db: " << event->annotations_dir);
    return;
  }

  LOGI("The annotations db " << event->annotations_dir << " have been loaded"
                             << " submitting appropriate event");

  emitImagesProviderChanged();
}

void AnnotatorController::handle(
    [[maybe_unused]] std::shared_ptr<RequestImagesDirProvider> event)
{
  emitImagesProviderChanged();
}

void AnnotatorController::emitImagesProviderChanged()
{
  assert(actx != nullptr);
  assert(actx->eventer != nullptr);

  if (actx == nullptr) {
    LOGE("No valid application context pointer found");
    return;
  }

  if (actx->eventer == nullptr) {
    LOGE("No valid application context' eventer pointer found");
    return;
  }

  auto efactory = actx->eventer->get_events_factory();

  auto imagesProviderChangedE =
      efactory->create_images_dir_provider_changed(annotations);

  actx->eventer->submit(imagesProviderChangedE);
}

void AnnotatorController::deinit() { actx.reset(); }

void AnnotatorController::handle(std::shared_ptr<CurrentImageChanged> event)
{
  assert(event != nullptr);
  assert(event->new_current_image != nullptr);

  if (event == nullptr) {
    LOGE("No valid image event pointer provided");
    return;
  }

  if (event->new_current_image == nullptr) {
    LOGE("No new current image pointer was provided");
    return;
  }

  LOGI("Current image changed: " << event->new_current_image->path);

  /// @todo: Insert the current image change handler over here
}

AnnotatorController::ImageRecordsSet
AnnotatorController::load_fs_images_records(const std::string& path)
{
  const auto newLoaded = std::make_shared<dbs::images::ImagesDirLoader>();

  return newLoaded->load(path);
}

void AnnotatorController::try_to_append_images_dir(const std::string& path)
{
  auto irs = load_fs_images_records(path);

  annotations->add_images_db(irs);
}

void AnnotatorController::handle(std::shared_ptr<StoreRequest> event)
{
  assert(annotations != nullptr);
  assert(event != nullptr);

  if (event == nullptr) {
    LOGE("Invalid event pointer given");
    return;
  }

  if (event->dbpath.empty()) {
    if (!annotations->store_db()) {
      LOGE("Fail to store data");
    }
    return;
  }

  LOGT("Trying to store to the path: " << event->dbpath);

  if (!annotations->store_db(event->dbpath)) {
    LOGE("Fail to store data to path " << event->dbpath);
  }
}

void AnnotatorController::handle(
    [[maybe_unused]] std::shared_ptr<CloseCurrentProject> event)
{
  LOGT("Erasing previous project data and creating a new data instance");

  annotations = create_project_data_instance();

  emitImagesProviderChanged();
}

AnnotatorController::ImageRecordsSet& AnnotatorController::get_images_db()
{
  assert(annotations != nullptr);

  return annotations->get_images_db();
}

std::string AnnotatorController::get_db_path()
{
  assert(annotations != nullptr);

  return annotations->get_db_path();
}

AnnotatorController::AnnotationsList
AnnotatorController::get_available_annotations()
{
  assert(annotations != nullptr);

  return annotations->get_available_annotations();
}

AnnotatorController::ExportContextPtr AnnotatorController::build_export_context(
    const std::string& dirPath)
{
  assert(annotations != nullptr);

  auto ectx = std::make_shared<exporters::ExportContext>();

  ectx->export_path = dirPath;
  ectx->dbProvider = annotations;

  return ectx;
}

void AnnotatorController::handle(ExportPlainTxt2FolderRequestPtr event)
{
  assert(event != nullptr);

  if (event == nullptr) {
    LOGE("Invalid event pointer provided");
    return;
  }

  if (!handle_export<exporters::PlainTxt2FolderExporter>(
          event->dst_folder_path)) {
    LOGE("Failure during the export");
  }
}

bool AnnotatorController::changed()
{
  assert(annotations != nullptr);

  return annotations != nullptr && annotations->changed();
}

void AnnotatorController::handle(ExportYolo4FolderRequestPtr event)
{
  assert(event != nullptr);

  if (event == nullptr) {
    LOGE("Invalid event pointer provided");
    return;
  }

  if (!handle_export<exporters::Yolo42FolderExporter>(event->dst_folder_path)) {
    LOGE("Failure during the export");
  }
}

template <class ExporterT>
bool AnnotatorController::handle_export(const std::string& dirPath)
{
  if (dirPath.empty()) {
    LOGE("Empty dir path provided");
    return true;
  }

  LOGD("Trying to export to " << dirPath);

  auto ectx = build_export_context(dirPath);

  assert(ectx != nullptr);

  auto exporter = std::make_shared<ExporterT>();

  if (!exporter->export_db(ectx)) {
    LOGE("Invalid export status");
    return false;
  }

  LOGD("DB seems to be exported");

  return true;
}

AnnotatorController::ExportContextPtr AnnotatorController::build_export_context(
    Export2PyTorchVisionRequestPtr event)
{
  assert(event != nullptr);
  assert(!event->dst_folder_path.empty());
  assert(event->cropper != nullptr);

  if (event == nullptr) {
    LOGE("Invalid event pointer provided");
    return {};
  }

  assert(annotations != nullptr);

  auto ectx = std::make_shared<exporters::ExportContext>();

  ectx->export_path = event->dst_folder_path;
  ectx->dbProvider = annotations;
  ectx->cropper = event->cropper;

  return ectx;
}

void AnnotatorController::handle(Export2PyTorchVisionRequestPtr event)
{
  assert(event != nullptr);
  assert(!event->dst_folder_path.empty());
  assert(event->cropper != nullptr);

  if (event == nullptr) {
    LOGE("Invalid event pointer provided");
    return;
  }

  if (event->dst_folder_path.empty()) {
    LOGE("No dst folder export path given");
    return;
  }

  if (event->cropper == nullptr) {
    LOGE("No image cropper provided");
    return;
  }

  auto ectx = build_export_context(event);

  assert(ectx != nullptr);

  auto exporter = std::make_shared<exporters::PyTorchVisionFolderExporter>();

  LOGI("Handle the PyTorch export event to " << ectx->export_path);

  if (!exporter->export_db(ectx)) {
    LOGE("Invalid export status");
    return;
  }

  LOGD("DB seems to be exported");
}

void AnnotatorController::handle(DeleteCurrentImageRequestPtr event)
{
  assert(event != nullptr);
  assert(!event->image_full_path.empty());
  assert(annotations != nullptr);

  if (event == nullptr) {
    LOGE("Invalid event pointer provided");
    return;
  }

  if (event->image_full_path.empty()) {
    LOGE("No image full path given");
    return;
  }

  if (!annotations->delete_image_record(event->image_full_path)) {
    LOGE("Fail to remove the image: " << event->image_full_path);
    return;
  }

  emitImagesProviderChanged();
}

bool AnnotatorController::delete_image_record(const std::string& irFullPath)
{
  assert(annotations != nullptr);

  return annotations->delete_image_record(irFullPath);
}

}  // namespace iannotator
