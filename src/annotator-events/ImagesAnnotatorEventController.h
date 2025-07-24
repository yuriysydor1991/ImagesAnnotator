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

#ifndef IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_EVENTS_CONTROLLER_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_EVENTS_CONTROLLER_CLASS_H

#include <memory>
#include <set>

#include "src/annotator-events/IImagesAnnotatorEventController.h"
#include "src/annotator-events/events/AnnotationsDirChanged.h"
#include "src/annotator-events/events/AnnotationsDirChangedIHandler.h"
#include "src/annotator-events/events/CloseCurrentProjectHandler.h"
#include "src/annotator-events/events/CurrentImageChanged.h"
#include "src/annotator-events/events/CurrentImageChangedHandler.h"
#include "src/annotator-events/events/EventsFactory.h"
#include "src/annotator-events/events/ImagesDirChanged.h"
#include "src/annotator-events/events/ImagesDirChangedIHandler.h"
#include "src/annotator-events/events/ImagesDirProviderChanged.h"
#include "src/annotator-events/events/ImagesDirProviderChangedHandler.h"
#include "src/annotator-events/events/RequestImagesDirProvider.h"
#include "src/annotator-events/events/RequestImagesDirProviderHandler.h"
#include "src/annotator-events/events/StoreRequest.h"
#include "src/annotator-events/events/StoreRequestHandler.h"
#include "src/app/ApplicationContext.h"

namespace events
{

/**
 * @brief Intermediate events class interface between the components.
 */
class ImagesAnnotatorEventController
    : virtual public IImagesAnnotatorEventController
{
 public:
  virtual ~ImagesAnnotatorEventController() = default;
  explicit ImagesAnnotatorEventController(
      std::shared_ptr<app::ApplicationContext> nactx);

  virtual std::shared_ptr<events::EventsFactory> get_events_factory() override;

  virtual bool deinit() override;

  virtual void onAnnotationsDirChanged(const std::string& newPath) override;
  virtual void submit(
      std::shared_ptr<events::AnnotationsDirChanged> event) override;
  virtual void subscribe(
      std::shared_ptr<events::AnnotationsDirChangedIHandler> handler) override;

  virtual void onImagesDirChanged(const std::string& newPath) override;
  virtual void submit(std::shared_ptr<events::ImagesDirChanged> event) override;
  virtual void subscribe(
      std::shared_ptr<events::ImagesDirChangedIHandler> handler) override;

  virtual void submit(std::shared_ptr<events::ImagesDirProviderChanged>
                          newIDBProvider) override;
  virtual void subscribe(
      std::shared_ptr<events::ImagesDirProviderChangedHandler> newIDBProvider)
      override;

  virtual void submit(
      std::shared_ptr<events::RequestImagesDirProvider> request) override;
  virtual void subscribe(
      std::shared_ptr<events::RequestImagesDirProviderHandler> request)
      override;

  virtual void submit(
      std::shared_ptr<events::CurrentImageChanged> event) override;
  virtual void subscribe(
      std::shared_ptr<events::CurrentImageChangedHandler> handler) override;

  virtual void submit(std::shared_ptr<events::StoreRequest> event) override;
  virtual void subscribe(
      std::shared_ptr<events::StoreRequestHandler> handler) override;

  virtual void submit(
      std::shared_ptr<events::CloseCurrentProject> event) override;
  virtual void subscribe(
      std::shared_ptr<events::CloseCurrentProjectHandler> handler) override;

  virtual void submit(events::ExportPlainTxt2FolderRequestPtr event) override;
  virtual void subscribe(
      events::ExportPlainTxt2FolderRequestHandlerPtr handler) override;

  virtual void submit(events::ExportYolo4FolderRequestPtr event) override;
  virtual void subscribe(
      events::ExportYolo4FolderRequestHandlerPtr handler) override;

  virtual void submit(events::Export2PyTorchVisionRequestPtr event) override;
  virtual void subscribe(
      events::Export2PyTorchVisionRequestHandlerPtr handler) override;

  virtual void submit(events::DeleteCurrentImageRequestPtr event) override;
  virtual void subscribe(
      events::DeleteCurrentImageRequestHandlerPtr handler) override;

  virtual void submit(events::LoadImagesFromWebPagePtr event) override;
  virtual void subscribe(
      events::LoadImagesFromWebPageHandlerPtr handler) override;

 private:
  using imagesHSet =
      std::set<std::shared_ptr<events::ImagesDirChangedIHandler>>;
  using annotationsHSet =
      std::set<std::shared_ptr<events::AnnotationsDirChangedIHandler>>;
  using imagesDBProviderSet =
      std::set<std::shared_ptr<events::ImagesDirProviderChangedHandler>>;
  using imagesDBORequestersSet =
      std::set<std::shared_ptr<events::RequestImagesDirProviderHandler>>;
  using currentImageChangedSet =
      std::set<std::shared_ptr<events::CurrentImageChangedHandler>>;
  using storeProjectSet =
      std::set<std::shared_ptr<events::StoreRequestHandler>>;
  using projectCloserSet =
      std::set<std::shared_ptr<events::CloseCurrentProjectHandler>>;
  using plainTxtEportersSet =
      std::set<events::ExportPlainTxt2FolderRequestHandlerPtr>;
  using yolo4ExportersSet =
      std::set<events::ExportYolo4FolderRequestHandlerPtr>;
  using pytorchVisionExportersSet =
      std::set<events::Export2PyTorchVisionRequestHandlerPtr>;
  using deleteCurrentImageHSet =
      std::set<events::DeleteCurrentImageRequestHandlerPtr>;
  using webpageILoadersSet = std::set<events::LoadImagesFromWebPageHandlerPtr>;

  template <class SubsQueue, class EventT>
  static void unified_submit(SubsQueue& queue, std::shared_ptr<EventT> event);

  std::shared_ptr<app::ApplicationContext> actx;

  imagesHSet images_handlers;
  annotationsHSet annotations_handlers;
  imagesDBProviderSet iprovider_handlers;
  imagesDBORequestersSet imagesDBObject_handlers;
  currentImageChangedSet currentImage_handlers;
  storeProjectSet store_handlers;
  projectCloserSet pclose_handlers;
  plainTxtEportersSet plainTxtExporters_handlers;
  yolo4ExportersSet yolo4Exporters_handlers;
  pytorchVisionExportersSet pytorchvision_handlers;
  deleteCurrentImageHSet deletecir_handlers;
  webpageILoadersSet webILoaders_handlers;

  std::shared_ptr<events::EventsFactory> efactory;
};

}  // namespace events

#endif  // IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_EVENTS_CONTROLLER_CLASS_H
