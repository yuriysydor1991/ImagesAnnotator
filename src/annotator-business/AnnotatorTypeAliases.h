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

#ifndef IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_TYPE_ALIASES_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_TYPE_ALIASES_CLASS_H

#include "src/annotator-business/dbs/AnnotationsDBs/AnnotationsDirDB.h"
#include "src/annotator-business/dbs/ImagesLoaders/ImagesDirLoader.h"
#include "src/annotator-business/exporters/ExportContext.h"
#include "src/annotator-events/events/AnnotationsDirChanged.h"
#include "src/annotator-events/events/AnnotationsDirChangedIHandler.h"
#include "src/annotator-events/events/CloseCurrentProject.h"
#include "src/annotator-events/events/CloseCurrentProjectHandler.h"
#include "src/annotator-events/events/CurrentImageChangedHandler.h"
#include "src/annotator-events/events/DeleteCurrentImageRequest.h"
#include "src/annotator-events/events/DeleteCurrentImageRequestHandler.h"
#include "src/annotator-events/events/Export2PyTorchVisionRequest.h"
#include "src/annotator-events/events/Export2PyTorchVisionRequestHandler.h"
#include "src/annotator-events/events/ExportPlainTxt2FolderRequestHandler.h"
#include "src/annotator-events/events/ExportYolo4FolderRequest.h"
#include "src/annotator-events/events/ExportYolo4FolderRequestHandler.h"
#include "src/annotator-events/events/ImagesDirChanged.h"
#include "src/annotator-events/events/ImagesDirChangedIHandler.h"
#include "src/annotator-events/events/LoadImagesFromWebPageHandler.h"
#include "src/annotator-events/events/RequestImagesDirProvider.h"
#include "src/annotator-events/events/RequestImagesDirProviderHandler.h"
#include "src/annotator-events/events/StoreRequest.h"
#include "src/annotator-events/events/StoreRequestHandler.h"
#include "src/app/ApplicationContext.h"

namespace iannotator
{

/**
 * @brief Holder for the type aliaces.
 */
class AnnotatorTypeAliases
{
 public:
  using ImagesDirChanged = events::events::ImagesDirChanged;
  using AnnotationsDirChanged = events::events::AnnotationsDirChanged;
  using ImagesDirLoader = dbs::images::ImagesDirLoader;
  using RequestImagesDirProvider = events::events::RequestImagesDirProvider;
  using ImageRecord = events::events::ImageRecord;
  using CurrentImageChanged = events::events::CurrentImageChanged;
  using ImageRecordsSet = events::events::ImageRecordsSet;
  using StoreRequest = events::events::StoreRequest;
  using CloseCurrentProject = events::events::CloseCurrentProject;
  using ExportPlainTxt2FolderRequestHandlerPtr =
      events::events::ExportPlainTxt2FolderRequestHandlerPtr;
  using ExportPlainTxt2FolderRequestPtr =
      events::events::ExportPlainTxt2FolderRequestPtr;
  using AnnotationsDirDBPtr = dbs::annotations::AnnotationsDirDBPtr;
  using ExportContextPtr = exporters::ExportContextPtr;
  using RequestImagesDirProviderHandler =
      events::events::RequestImagesDirProviderHandler;
  using RequestImagesDirProviderHandlerPtr =
      events::events::RequestImagesDirProviderHandlerPtr;
  using ExportYolo4FolderRequestPtr =
      events::events::ExportYolo4FolderRequestPtr;
  using ExportYolo4FolderRequestHandlerPtr =
      events::events::ExportYolo4FolderRequestHandlerPtr;
  using Export2PyTorchVisionRequestPtr =
      events::events::Export2PyTorchVisionRequestPtr;
  using Export2PyTorchVisionRequestHandlerPtr =
      events::events::Export2PyTorchVisionRequestHandlerPtr;
  using DeleteCurrentImageRequestPtr =
      events::events::DeleteCurrentImageRequestPtr;
  using DeleteCurrentImageRequestHandlerPtr =
      events::events::DeleteCurrentImageRequestHandlerPtr;
  using LoadImagesFromWebPagePtr = events::events::LoadImagesFromWebPagePtr;

  virtual ~AnnotatorTypeAliases() = default;
  AnnotatorTypeAliases() = default;
};

}  // namespace iannotator

#endif  // IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATOR_TYPE_ALIASES_CLASS_H
