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

#include "src/annotator-events/ImagesAnnotatorEventController.h"

#include <cassert>
#include <memory>
#include <set>

#include "src/annotator-events/events/AnnotationsDirChanged.h"
#include "src/annotator-events/events/AnnotationsDirChangedIHandler.h"
#include "src/annotator-events/events/ImagesDirChanged.h"
#include "src/annotator-events/events/ImagesDirChangedIHandler.h"
#include "src/log/log.h"

namespace events
{

ImagesAnnotatorEventController::ImagesAnnotatorEventController(
    std::shared_ptr<app::ApplicationContext> nactx)
    : actx{nactx},
      images_handlers{},
      annotations_handlers{},
      efactory{events::EventsFactory::instance()}
{
}

void ImagesAnnotatorEventController::onAnnotationsDirChanged(
    const std::string& newPath)
{
  auto annDE = efactory->create_annotations_dir_changed(newPath);

  submit(annDE);
}

void ImagesAnnotatorEventController::submit(
    std::shared_ptr<events::AnnotationsDirChanged> event)
{
  unified_submit(annotations_handlers, event);
}

void ImagesAnnotatorEventController::subscribe(
    std::shared_ptr<events::AnnotationsDirChangedIHandler> handler)
{
  assert(handler != nullptr);

  if (handler == nullptr) {
    LOGE("No valid handler pointer was given");
    return;
  }

  annotations_handlers.insert(handler);
}

void ImagesAnnotatorEventController::onImagesDirChanged(
    const std::string& newPath)
{
  auto imagesDE = efactory->create_image_dir_changed(newPath);

  submit(imagesDE);
}

void ImagesAnnotatorEventController::submit(
    std::shared_ptr<events::ImagesDirChanged> event)
{
  unified_submit(images_handlers, event);
}

void ImagesAnnotatorEventController::subscribe(
    std::shared_ptr<events::ImagesDirChangedIHandler> handler)
{
  assert(handler != nullptr);

  if (handler == nullptr) {
    LOGE("No valid handler pointer was given");
    return;
  }

  images_handlers.insert(handler);
}

bool ImagesAnnotatorEventController::deinit()
{
  actx.reset();
  return true;
}

void ImagesAnnotatorEventController::submit(
    std::shared_ptr<events::ImagesDirProviderChanged> newIDBProvider)
{
  unified_submit(iprovider_handlers, newIDBProvider);
}

void ImagesAnnotatorEventController::subscribe(
    std::shared_ptr<events::ImagesDirProviderChangedHandler> newIDBProvider)
{
  iprovider_handlers.insert(newIDBProvider);
}

std::shared_ptr<events::EventsFactory>
ImagesAnnotatorEventController::get_events_factory()
{
  return efactory;
}

template <class SubsQueue, class EventT>
inline void ImagesAnnotatorEventController::unified_submit(
    SubsQueue& queue, std::shared_ptr<EventT> event)
{
  assert(event != nullptr);

  if (event == nullptr) {
    LOGE("No valid event object pointer was given");
    return;
  }

  for (auto& ih : queue) {
    ih->handle(event);
  }
}

void ImagesAnnotatorEventController::submit(
    std::shared_ptr<events::RequestImagesDirProvider> request)
{
  unified_submit(imagesDBObject_handlers, request);
}

void ImagesAnnotatorEventController::subscribe(
    std::shared_ptr<events::RequestImagesDirProviderHandler> request)
{
  imagesDBObject_handlers.insert(request);
}

void ImagesAnnotatorEventController::submit(
    std::shared_ptr<events::CurrentImageChanged> event)
{
  unified_submit(currentImage_handlers, event);
}

void ImagesAnnotatorEventController::subscribe(
    std::shared_ptr<events::CurrentImageChangedHandler> handler)
{
  currentImage_handlers.insert(handler);
}

void ImagesAnnotatorEventController::submit(
    std::shared_ptr<events::StoreRequest> event)
{
  unified_submit(store_handlers, event);
}

void ImagesAnnotatorEventController::subscribe(
    std::shared_ptr<events::StoreRequestHandler> handler)
{
  store_handlers.insert(handler);
}

void ImagesAnnotatorEventController::submit(
    std::shared_ptr<events::CloseCurrentProject> event)
{
  unified_submit(pclose_handlers, event);
}

void ImagesAnnotatorEventController::subscribe(
    std::shared_ptr<events::CloseCurrentProjectHandler> handler)
{
  pclose_handlers.insert(handler);
}

void ImagesAnnotatorEventController::submit(
    events::ExportPlainTxt2FolderRequestPtr event)
{
  unified_submit(plainTxtExporters_handlers, event);
}

void ImagesAnnotatorEventController::subscribe(
    events::ExportPlainTxt2FolderRequestHandlerPtr handler)
{
  plainTxtExporters_handlers.insert(handler);
}

void ImagesAnnotatorEventController::submit(
    events::ExportYolo4FolderRequestPtr event)
{
  unified_submit(yolo4Exporters_handlers, event);
}

void ImagesAnnotatorEventController::subscribe(
    events::ExportYolo4FolderRequestHandlerPtr handler)
{
  yolo4Exporters_handlers.insert(handler);
}

void ImagesAnnotatorEventController::submit(
    events::Export2PyTorchVisionRequestPtr event)
{
  unified_submit(pytorchvision_handlers, event);
}

void ImagesAnnotatorEventController::subscribe(
    events::Export2PyTorchVisionRequestHandlerPtr handler)
{
  pytorchvision_handlers.insert(handler);
}

void ImagesAnnotatorEventController::submit(
    events::DeleteCurrentImageRequestPtr event)
{
  unified_submit(deletecir_handlers, event);
}

void ImagesAnnotatorEventController::subscribe(
    events::DeleteCurrentImageRequestHandlerPtr handler)
{
  deletecir_handlers.insert(handler);
}

void ImagesAnnotatorEventController::submit(
    events::LoadImagesFromWebPagePtr event)
{
  unified_submit(webILoaders_handlers, event);
}

void ImagesAnnotatorEventController::subscribe(
    events::LoadImagesFromWebPageHandlerPtr handler)
{
  webILoaders_handlers.insert(handler);
}

void ImagesAnnotatorEventController::submit(events::DisplayErrorEventPtr event)
{
  unified_submit(errDisp_handlers, event);
}

void ImagesAnnotatorEventController::subscribe(
    events::DisplayErrorEventHandlerPtr handler)
{
  errDisp_handlers.insert(handler);
}

}  // namespace events
