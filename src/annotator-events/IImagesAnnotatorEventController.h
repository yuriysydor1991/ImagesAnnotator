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

#ifndef IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_EVENTS_CONTROLLER_INTERFACE_ABSTRACT_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_EVENTS_CONTROLLER_INTERFACE_ABSTRACT_CLASS_H

#include <memory>

#include "src/annotator-events/events/AnnotationsDirChanged.h"
#include "src/annotator-events/events/AnnotationsDirChangedIHandler.h"
#include "src/annotator-events/events/CloseCurrentProject.h"
#include "src/annotator-events/events/CloseCurrentProjectHandler.h"
#include "src/annotator-events/events/CurrentImageChanged.h"
#include "src/annotator-events/events/CurrentImageChangedHandler.h"
#include "src/annotator-events/events/DeleteCurrentImageRequest.h"
#include "src/annotator-events/events/DeleteCurrentImageRequestHandler.h"
#include "src/annotator-events/events/EventsFactory.h"
#include "src/annotator-events/events/Export2PyTorchVisionRequest.h"
#include "src/annotator-events/events/Export2PyTorchVisionRequestHandler.h"
#include "src/annotator-events/events/ExportPlainTxt2FolderRequest.h"
#include "src/annotator-events/events/ExportPlainTxt2FolderRequestHandler.h"
#include "src/annotator-events/events/ExportYolo4FolderRequest.h"
#include "src/annotator-events/events/ExportYolo4FolderRequestHandler.h"
#include "src/annotator-events/events/ImagesDirChanged.h"
#include "src/annotator-events/events/ImagesDirChangedIHandler.h"
#include "src/annotator-events/events/ImagesDirProviderChanged.h"
#include "src/annotator-events/events/ImagesDirProviderChangedHandler.h"
#include "src/annotator-events/events/LoadImagesFromWebPage.h"
#include "src/annotator-events/events/LoadImagesFromWebPageHandler.h"
#include "src/annotator-events/events/RequestImagesDirProvider.h"
#include "src/annotator-events/events/RequestImagesDirProviderHandler.h"
#include "src/annotator-events/events/StoreRequest.h"
#include "src/annotator-events/events/StoreRequestHandler.h"

namespace events
{

/**
 * @brief An abstract class for the images annotator event controller classes.
 */
class IImagesAnnotatorEventController
{
 public:
  virtual ~IImagesAnnotatorEventController() = default;
  IImagesAnnotatorEventController() = default;

  virtual bool deinit() = 0;

  virtual std::shared_ptr<events::EventsFactory> get_events_factory() = 0;

  virtual void onAnnotationsDirChanged(const std::string& newPath) = 0;
  virtual void submit(std::shared_ptr<events::AnnotationsDirChanged> event) = 0;
  virtual void subscribe(
      std::shared_ptr<events::AnnotationsDirChangedIHandler> handler) = 0;

  virtual void onImagesDirChanged(const std::string& newPath) = 0;
  virtual void submit(std::shared_ptr<events::ImagesDirChanged> event) = 0;
  virtual void subscribe(
      std::shared_ptr<events::ImagesDirChangedIHandler> handler) = 0;

  virtual void submit(
      std::shared_ptr<events::ImagesDirProviderChanged> newIDBProvider) = 0;
  virtual void subscribe(
      std::shared_ptr<events::ImagesDirProviderChangedHandler>
          newIDBProvider) = 0;

  virtual void submit(
      std::shared_ptr<events::RequestImagesDirProvider> request) = 0;
  virtual void subscribe(
      std::shared_ptr<events::RequestImagesDirProviderHandler> request) = 0;

  virtual void submit(
      std::shared_ptr<events::CurrentImageChanged> newCurrentImage) = 0;
  virtual void subscribe(
      std::shared_ptr<events::CurrentImageChangedHandler> request) = 0;

  virtual void submit(std::shared_ptr<events::StoreRequest> event) = 0;
  virtual void subscribe(
      std::shared_ptr<events::StoreRequestHandler> handler) = 0;

  virtual void submit(std::shared_ptr<events::CloseCurrentProject> event) = 0;
  virtual void subscribe(
      std::shared_ptr<events::CloseCurrentProjectHandler> handler) = 0;

  virtual void submit(events::ExportPlainTxt2FolderRequestPtr event) = 0;
  virtual void subscribe(
      events::ExportPlainTxt2FolderRequestHandlerPtr handler) = 0;

  virtual void submit(events::ExportYolo4FolderRequestPtr event) = 0;
  virtual void subscribe(
      events::ExportYolo4FolderRequestHandlerPtr handler) = 0;

  virtual void submit(events::Export2PyTorchVisionRequestPtr event) = 0;
  virtual void subscribe(
      events::Export2PyTorchVisionRequestHandlerPtr handler) = 0;

  virtual void submit(events::DeleteCurrentImageRequestPtr event) = 0;
  virtual void subscribe(
      events::DeleteCurrentImageRequestHandlerPtr handler) = 0;

  virtual void submit(events::LoadImagesFromWebPagePtr event) = 0;
  virtual void subscribe(events::LoadImagesFromWebPageHandlerPtr handler) = 0;
};

}  // namespace events

#endif  // IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_EVENTS_CONTROLLER_INTERFACE_ABSTRACT_CLASS_H
