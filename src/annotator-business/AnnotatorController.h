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

#ifndef IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATORCONTROLLER_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATORCONTROLLER_CLASS_H

#include <memory>

#include "src/annotator-business/AnnotatorTypeAliases.h"
#include "src/annotator-business/dbs/ImagesLoaders/ImagesDirLoader.h"
#include "src/annotator-events/events/AnnotationsDirChanged.h"
#include "src/annotator-events/events/AnnotationsDirChangedIHandler.h"
#include "src/annotator-events/events/CloseCurrentProject.h"
#include "src/annotator-events/events/CloseCurrentProjectHandler.h"
#include "src/annotator-events/events/CurrentImageChangedHandler.h"
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

/**
 * @brief The images annotator namespace that holds all annotator related
 * business logic.
 */
namespace iannotator
{

/**
 * @brief The annotator business controller class.
 */
class AnnotatorController
    : virtual public events::events::AnnotationsDirChangedIHandler,
      virtual public events::events::ImagesDirChangedIHandler,
      public std::enable_shared_from_this<AnnotatorController>,
      virtual public events::events::RequestImagesDirProviderHandler,
      virtual public events::events::CurrentImageChangedHandler,
      virtual public events::events::StoreRequestHandler,
      virtual public events::events::CloseCurrentProjectHandler,
      virtual public events::events::ImagesPathsDBProvider,
      virtual public events::events::ExportPlainTxt2FolderRequestHandler,
      virtual public events::events::ExportYolo4FolderRequestHandler,
      virtual public events::events::Export2PyTorchVisionRequestHandler,
      virtual public events::events::ExportUltralyticsDetectRequestHandler,
      virtual public events::events::ExportUltralyticsObbRequestHandler,
      virtual public events::events::ExportUltralyticsSegmentRequestHandler,
      virtual public events::events::ExportCocoRequestHandler,
      virtual public events::events::ExportPascalVocRequestHandler,
      virtual public events::events::ExportCreateMLRequestHandler,
      virtual public events::events::ExportDxxwjz2RequestHandler,
      virtual public events::events::ExportDxxwjz2pRequestHandler,
      virtual public events::events::ExportDxxwjz3RequestHandler,
      virtual public events::events::ExportDxxwjz3pRequestHandler,
      virtual public events::events::ImportPlainTxt2FolderRequestHandler,
      virtual public events::events::ImportYolo4FolderRequestHandler,
      virtual public events::events::ImportUltralyticsDetectRequestHandler,
      virtual public events::events::ImportUltralyticsObbRequestHandler,
      virtual public events::events::ImportUltralyticsSegmentRequestHandler,
      virtual public events::events::ImportCocoRequestHandler,
      virtual public events::events::ImportPascalVocRequestHandler,
      virtual public events::events::ImportCreateMLRequestHandler,
      virtual public events::events::ImportPyTorchVisionRequestHandler,
      virtual public events::events::DeleteCurrentImageRequestHandler,
      virtual public events::events::LoadImagesFromWebPageHandler,
      public AnnotatorTypeAliases
{
 public:
  virtual ~AnnotatorController() = default;
  AnnotatorController();

  virtual bool init(std::shared_ptr<app::ApplicationContext> ctx);

  virtual void handle(std::shared_ptr<ImagesDirChanged> event) override;
  virtual void handle(std::shared_ptr<AnnotationsDirChanged> event) override;
  virtual void handle(std::shared_ptr<RequestImagesDirProvider> event) override;
  virtual void handle(std::shared_ptr<CurrentImageChanged> event) override;
  virtual void handle(std::shared_ptr<StoreRequest> event) override;
  virtual void handle(std::shared_ptr<CloseCurrentProject> event) override;
  virtual void handle(ExportPlainTxt2FolderRequestPtr event) override;
  virtual void handle(ExportYolo4FolderRequestPtr event) override;
  virtual void handle(Export2PyTorchVisionRequestPtr event) override;
  virtual void handle(ExportUltralyticsDetectRequestPtr event) override;
  virtual void handle(ExportUltralyticsObbRequestPtr event) override;
  virtual void handle(ExportUltralyticsSegmentRequestPtr event) override;
  virtual void handle(ExportCocoRequestPtr event) override;
  virtual void handle(ExportPascalVocRequestPtr event) override;
  virtual void handle(ExportCreateMLRequestPtr event) override;
  virtual void handle(ExportDxxwjz2RequestPtr event) override;
  virtual void handle(ExportDxxwjz2pRequestPtr event) override;
  virtual void handle(ExportDxxwjz3RequestPtr event) override;
  virtual void handle(ExportDxxwjz3pRequestPtr event) override;
  virtual void handle(ImportPlainTxt2FolderRequestPtr event) override;
  virtual void handle(ImportYolo4FolderRequestPtr event) override;
  virtual void handle(ImportUltralyticsDetectRequestPtr event) override;
  virtual void handle(ImportUltralyticsObbRequestPtr event) override;
  virtual void handle(ImportUltralyticsSegmentRequestPtr event) override;
  virtual void handle(ImportCocoRequestPtr event) override;
  virtual void handle(ImportPascalVocRequestPtr event) override;
  virtual void handle(ImportCreateMLRequestPtr event) override;
  virtual void handle(ImportPyTorchVisionRequestPtr event) override;
  virtual void handle(DeleteCurrentImageRequestPtr event) override;
  virtual void handle(LoadImagesFromWebPagePtr event) override;

  virtual ImageRecordsSet& get_images_db() override;
  virtual std::string get_db_path() override;
  virtual AnnotationsList get_available_annotations() override;
  virtual bool changed() override;
  bool delete_image_record(const std::string& irFullPath) override;

  virtual void deinit();

 protected:
  /**
   * @brief Builds the library context of the ContextT dataset layout, filled
   * with the export destination and the annotations database.
   */
  template <class ContextT>
  std::shared_ptr<ContextT> build_export_context(const std::string& dirPath);

  /**
   * @brief Builds the library context of the ContextT dataset layout, filled
   * with the import source, the annotations database the recovered records are
   * merged into and the image measuring service of the GUI toolkit in use.
   */
  template <class ContextT>
  std::shared_ptr<ContextT> build_import_context(
      const std::string& dirPath, const IImageSizeFacilityProviderPtr& sizer);

 private:
  void emitImagesProviderChanged();
  void emitDisplayError(const std::string& edesc);

  ImageRecordsSet load_fs_images_records(const std::string& path);
  void try_to_append_images_dir(const std::string& path);
  AnnotationsDBPtr create_project_data_instance();

  /**
   * @brief Runs the export of the ContextT dataset layout into the given
   * directory.
   */
  template <class ContextT>
  bool handle_export(const std::string& dirPath);

  /**
   * @brief Runs the import of the ContextT dataset layout out of the given
   * directory and republishes the images provider when it recovered anything.
   */
  template <class ContextT>
  bool handle_import(const std::string& dirPath,
                     const IImageSizeFacilityProviderPtr& sizer);

  AnnotationsDBPtr annotations;

  std::shared_ptr<app::ApplicationContext> actx;
};

}  // namespace iannotator

#endif  // IMAGES_ANNOTATOR_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATORCONTROLLER_CLASS_H
