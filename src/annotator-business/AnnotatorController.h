#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATORCONTROLLER_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATORCONTROLLER_CLASS_H

#include <memory>

#include "src/annotator-business/dbs/AnnotationsDirDB.h"
#include "src/annotator-business/dbs/ImagesDirDB.h"
#include "src/annotator-events/events/AnnotationsDirChanged.h"
#include "src/annotator-events/events/AnnotationsDirChangedIHandler.h"
#include "src/annotator-events/events/CurrentImageChangedHandler.h"
#include "src/annotator-events/events/ImagesDirChanged.h"
#include "src/annotator-events/events/ImagesDirChangedIHandler.h"
#include "src/annotator-events/events/RequestImagesDirProvider.h"
#include "src/annotator-events/events/RequestImagesDirProviderHandler.h"
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
      virtual public events::events::ImagesPathsDBProvider,
      public std::enable_shared_from_this<AnnotatorController>,
      virtual public events::events::RequestImagesDirProviderHandler,
      virtual public events::events::CurrentImageChangedHandler
{
 public:
  using ImagesDirChanged = events::events::ImagesDirChanged;
  using AnnotationsDirChanged = events::events::AnnotationsDirChanged;
  using ImagesDirDB = dbs::ImagesDirDB::ImagesDB;
  using RequestImagesDirProvider = events::events::RequestImagesDirProvider;
  using ImageRecord = events::events::ImageRecord;
  using CurrentImageChanged = events::events::CurrentImageChanged;

  using RequestImagesDirProviderHandler =
      events::events::RequestImagesDirProviderHandler;

  virtual ~AnnotatorController() = default;
  AnnotatorController();

  virtual bool init(std::shared_ptr<app::ApplicationContext> ctx);

  virtual void handle(std::shared_ptr<ImagesDirChanged> event) override;
  virtual void handle(std::shared_ptr<AnnotationsDirChanged> event) override;
  virtual void handle(std::shared_ptr<RequestImagesDirProvider> event) override;
  virtual void handle(std::shared_ptr<CurrentImageChanged> event) override;

  virtual ImagesDirDB& get_images_db();

  virtual void deinit();

 private:
  void emitImagesProviderChanged();

  std::shared_ptr<dbs::ImagesDirDB> images;
  std::shared_ptr<dbs::AnnotationsDirDB> annotations;

  std::shared_ptr<app::ApplicationContext> actx;
};

}  // namespace iannotator

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATORCONTROLLER_CLASS_H
