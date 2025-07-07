#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATORCONTROLLER_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATORCONTROLLER_CLASS_H

#include <memory>

#include "src/annotator-business/dbs/AnnotationsDBs/AnnotationsDirDB.h"
#include "src/annotator-business/dbs/ImagesLoaders/ImagesDirLoader.h"
#include "src/annotator-business/exporters/ExportContext.h"
#include "src/annotator-events/events/AnnotationsDirChanged.h"
#include "src/annotator-events/events/AnnotationsDirChangedIHandler.h"
#include "src/annotator-events/events/CloseCurrentProject.h"
#include "src/annotator-events/events/CloseCurrentProjectHandler.h"
#include "src/annotator-events/events/CurrentImageChangedHandler.h"
#include "src/annotator-events/events/ExportPlainTxt2FolderRequestHandler.h"
#include "src/annotator-events/events/ImagesDirChanged.h"
#include "src/annotator-events/events/ImagesDirChangedIHandler.h"
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
      virtual public events::events::ExportPlainTxt2FolderRequestHandler
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

  virtual ImageRecordsSet& get_images_db() override;
  virtual std::string get_db_path() override;
  virtual AnnotationsList get_available_annotations() override;

  virtual void deinit();

 protected:
  virtual ExportContextPtr build_export_context(const std::string& dirPath);

 private:
  void emitImagesProviderChanged();

  ImageRecordsSet load_fs_images_records(const std::string& path);
  void try_to_append_images_dir(const std::string& path);
  AnnotationsDirDBPtr create_project_data_instance();

  AnnotationsDirDBPtr annotations;

  std::shared_ptr<app::ApplicationContext> actx;
};

}  // namespace iannotator

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATORCONTROLLER_CLASS_H
