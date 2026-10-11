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

#include "src/annotator-business/dbs/ImagesLoaders/ImagesDirLoader.h"
#include "src/annotator-events/events/AnnotationsDirChanged.h"
#include "src/annotator-events/events/AnnotationsDirChangedIHandler.h"
#include "src/annotator-events/events/CloseCurrentProject.h"
#include "src/annotator-events/events/CloseCurrentProjectHandler.h"
#include "src/annotator-events/events/CurrentImageChangedHandler.h"
#include "src/annotator-events/events/DataDriversAliases.h"
#include "src/annotator-events/events/DataExportersAliases.h"
#include "src/annotator-events/events/DataImportersAliases.h"
#include "src/annotator-events/events/DeleteCurrentImageRequest.h"
#include "src/annotator-events/events/DeleteCurrentImageRequestHandler.h"
#include "src/annotator-events/events/Export2PyTorchVisionRequest.h"
#include "src/annotator-events/events/Export2PyTorchVisionRequestHandler.h"
#include "src/annotator-events/events/ExportCocoRequest.h"
#include "src/annotator-events/events/ExportCocoRequestHandler.h"
#include "src/annotator-events/events/ExportCreateMLRequest.h"
#include "src/annotator-events/events/ExportCreateMLRequestHandler.h"
#include "src/annotator-events/events/ExportDxxwjz2Request.h"
#include "src/annotator-events/events/ExportDxxwjz2RequestHandler.h"
#include "src/annotator-events/events/ExportDxxwjz2pRequest.h"
#include "src/annotator-events/events/ExportDxxwjz2pRequestHandler.h"
#include "src/annotator-events/events/ExportDxxwjz3Request.h"
#include "src/annotator-events/events/ExportDxxwjz3RequestHandler.h"
#include "src/annotator-events/events/ExportDxxwjz3pRequest.h"
#include "src/annotator-events/events/ExportDxxwjz3pRequestHandler.h"
#include "src/annotator-events/events/ExportPascalVocRequest.h"
#include "src/annotator-events/events/ExportPascalVocRequestHandler.h"
#include "src/annotator-events/events/ExportPlainTxt2FolderRequestHandler.h"
#include "src/annotator-events/events/ExportUltralyticsDetectRequest.h"
#include "src/annotator-events/events/ExportUltralyticsDetectRequestHandler.h"
#include "src/annotator-events/events/ExportUltralyticsObbRequest.h"
#include "src/annotator-events/events/ExportUltralyticsObbRequestHandler.h"
#include "src/annotator-events/events/ExportUltralyticsSegmentRequest.h"
#include "src/annotator-events/events/ExportUltralyticsSegmentRequestHandler.h"
#include "src/annotator-events/events/ExportYolo4FolderRequest.h"
#include "src/annotator-events/events/ExportYolo4FolderRequestHandler.h"
#include "src/annotator-events/events/ImagesDirChanged.h"
#include "src/annotator-events/events/ImagesDirChangedIHandler.h"
#include "src/annotator-events/events/ImportCocoRequest.h"
#include "src/annotator-events/events/ImportCocoRequestHandler.h"
#include "src/annotator-events/events/ImportCreateMLRequest.h"
#include "src/annotator-events/events/ImportCreateMLRequestHandler.h"
#include "src/annotator-events/events/ImportPascalVocRequest.h"
#include "src/annotator-events/events/ImportPascalVocRequestHandler.h"
#include "src/annotator-events/events/ImportPlainTxt2FolderRequest.h"
#include "src/annotator-events/events/ImportPlainTxt2FolderRequestHandler.h"
#include "src/annotator-events/events/ImportPyTorchVisionRequest.h"
#include "src/annotator-events/events/ImportPyTorchVisionRequestHandler.h"
#include "src/annotator-events/events/ImportUltralyticsDetectRequest.h"
#include "src/annotator-events/events/ImportUltralyticsDetectRequestHandler.h"
#include "src/annotator-events/events/ImportUltralyticsObbRequest.h"
#include "src/annotator-events/events/ImportUltralyticsObbRequestHandler.h"
#include "src/annotator-events/events/ImportUltralyticsSegmentRequest.h"
#include "src/annotator-events/events/ImportUltralyticsSegmentRequestHandler.h"
#include "src/annotator-events/events/ImportYolo4FolderRequest.h"
#include "src/annotator-events/events/ImportYolo4FolderRequestHandler.h"
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
  /// @brief The annotations project database the controller holds. It is the
  /// ImagesAnnotator-DataDrivers library abstract interface rather than a
  /// project class: the loading, the storing, the merging and the change
  /// tracking of the project JSON files all live in the library now.
  using AnnotationsDBPtr = iadd::IAnnotationsDBPtr;
  /// @brief The export configuration the controller fills in and hands to an
  /// exporter of the ImagesAnnotator-DataExporters library. The wanted dataset
  /// layout is named by the descendant instantiated, so the library carries no
  /// format enumeration any more.
  using ExportContextPtr = iade::IADataExportersContextPtr;
  using PlainTxtExportContext = iade::PlainTxtExportContext;
  using Yolo4ExportContext = iade::Yolo4ExportContext;
  using PyTorchExportContext = iade::PyTorchExportContext;
  using UltralyticsDetectExportContext =
      iade::UltralyticsDetectExportContext;
  using UltralyticsObbExportContext =
      iade::UltralyticsObbExportContext;
  using UltralyticsSegmentExportContext =
      iade::UltralyticsSegmentExportContext;
  using CocoExportContext = iade::CocoExportContext;
  using PascalVocExportContext = iade::PascalVocExportContext;
  using CreateMLExportContext = iade::CreateMLExportContext;
  using Dxxwjz2ExportContext = iade::Dxxwjz2ExportContext;
  using Dxxwjz2pExportContext = iade::Dxxwjz2pExportContext;
  using Dxxwjz3ExportContext = iade::Dxxwjz3ExportContext;
  using Dxxwjz3pExportContext = iade::Dxxwjz3pExportContext;
  /// @brief The import configuration the controller fills in and hands to an
  /// importer of the ImagesAnnotator-DataImporters library. As on the export
  /// side, the wanted dataset layout is named by the descendant instantiated.
  using ImportContextPtr = iadi::IADataImportersContextPtr;
  using PlainTxtImportContext = iadi::PlainTxtImportContext;
  using Yolo4ImportContext = iadi::Yolo4ImportContext;
  using UltralyticsDetectImportContext =
      iadi::UltralyticsDetectImportContext;
  using UltralyticsObbImportContext =
      iadi::UltralyticsObbImportContext;
  using UltralyticsSegmentImportContext =
      iadi::UltralyticsSegmentImportContext;
  using CocoImportContext = iadi::CocoImportContext;
  using PascalVocImportContext = iadi::PascalVocImportContext;
  using CreateMLImportContext = iadi::CreateMLImportContext;
  using PyTorchImportContext = iadi::PyTorchImportContext;
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
  using ExportUltralyticsDetectRequestPtr =
      events::events::ExportUltralyticsDetectRequestPtr;
  using ExportUltralyticsDetectRequestHandlerPtr =
      events::events::ExportUltralyticsDetectRequestHandlerPtr;
  using ExportUltralyticsObbRequestPtr =
      events::events::ExportUltralyticsObbRequestPtr;
  using ExportUltralyticsObbRequestHandlerPtr =
      events::events::ExportUltralyticsObbRequestHandlerPtr;
  using ExportUltralyticsSegmentRequestPtr =
      events::events::ExportUltralyticsSegmentRequestPtr;
  using ExportUltralyticsSegmentRequestHandlerPtr =
      events::events::ExportUltralyticsSegmentRequestHandlerPtr;
  using ExportCocoRequestPtr = events::events::ExportCocoRequestPtr;
  using ExportCocoRequestHandlerPtr =
      events::events::ExportCocoRequestHandlerPtr;
  using ExportPascalVocRequestPtr = events::events::ExportPascalVocRequestPtr;
  using ExportPascalVocRequestHandlerPtr =
      events::events::ExportPascalVocRequestHandlerPtr;
  using ExportCreateMLRequestPtr = events::events::ExportCreateMLRequestPtr;
  using ExportCreateMLRequestHandlerPtr =
      events::events::ExportCreateMLRequestHandlerPtr;
  using ExportDxxwjz2RequestPtr = events::events::ExportDxxwjz2RequestPtr;
  using ExportDxxwjz2RequestHandlerPtr =
      events::events::ExportDxxwjz2RequestHandlerPtr;
  using ExportDxxwjz2pRequestPtr = events::events::ExportDxxwjz2pRequestPtr;
  using ExportDxxwjz2pRequestHandlerPtr =
      events::events::ExportDxxwjz2pRequestHandlerPtr;
  using ExportDxxwjz3RequestPtr = events::events::ExportDxxwjz3RequestPtr;
  using ExportDxxwjz3RequestHandlerPtr =
      events::events::ExportDxxwjz3RequestHandlerPtr;
  using ExportDxxwjz3pRequestPtr = events::events::ExportDxxwjz3pRequestPtr;
  using ExportDxxwjz3pRequestHandlerPtr =
      events::events::ExportDxxwjz3pRequestHandlerPtr;
  /// @brief The image measuring service the GUI toolkit hands over with an
  /// import request, the counterpart of the cropper of the PyTorch export.
  using IImageSizeFacilityProviderPtr =
      events::events::IImageSizeFacilityProviderPtr;
  using ImportPlainTxt2FolderRequestPtr =
      events::events::ImportPlainTxt2FolderRequestPtr;
  using ImportPlainTxt2FolderRequestHandlerPtr =
      events::events::ImportPlainTxt2FolderRequestHandlerPtr;
  using ImportYolo4FolderRequestPtr =
      events::events::ImportYolo4FolderRequestPtr;
  using ImportYolo4FolderRequestHandlerPtr =
      events::events::ImportYolo4FolderRequestHandlerPtr;
  using ImportUltralyticsDetectRequestPtr =
      events::events::ImportUltralyticsDetectRequestPtr;
  using ImportUltralyticsDetectRequestHandlerPtr =
      events::events::ImportUltralyticsDetectRequestHandlerPtr;
  using ImportUltralyticsObbRequestPtr =
      events::events::ImportUltralyticsObbRequestPtr;
  using ImportUltralyticsObbRequestHandlerPtr =
      events::events::ImportUltralyticsObbRequestHandlerPtr;
  using ImportUltralyticsSegmentRequestPtr =
      events::events::ImportUltralyticsSegmentRequestPtr;
  using ImportUltralyticsSegmentRequestHandlerPtr =
      events::events::ImportUltralyticsSegmentRequestHandlerPtr;
  using ImportCocoRequestPtr = events::events::ImportCocoRequestPtr;
  using ImportCocoRequestHandlerPtr =
      events::events::ImportCocoRequestHandlerPtr;
  using ImportPascalVocRequestPtr = events::events::ImportPascalVocRequestPtr;
  using ImportPascalVocRequestHandlerPtr =
      events::events::ImportPascalVocRequestHandlerPtr;
  using ImportCreateMLRequestPtr = events::events::ImportCreateMLRequestPtr;
  using ImportCreateMLRequestHandlerPtr =
      events::events::ImportCreateMLRequestHandlerPtr;
  using ImportPyTorchVisionRequestPtr =
      events::events::ImportPyTorchVisionRequestPtr;
  using ImportPyTorchVisionRequestHandlerPtr =
      events::events::ImportPyTorchVisionRequestHandlerPtr;
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
