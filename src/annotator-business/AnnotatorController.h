#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATORCONTROLLER_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATORCONTROLLER_CLASS_H

#include <memory>

#include "src/annotator-business/dbs/AnnotationsDirDB.h"
#include "src/annotator-business/dbs/ImagesDirDB.h"
#include "src/annotator-events/events/AnnotationsDirChanged.h"
#include "src/annotator-events/events/AnnotationsDirChangedIHandler.h"
#include "src/annotator-events/events/ImagesDirChanged.h"
#include "src/annotator-events/events/ImagesDirChangedIHandler.h"
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
      public std::enable_shared_from_this<AnnotatorController>
{
 public:
  using ImagesDirChanged = events::events::ImagesDirChanged;
  using AnnotationsDirChanged = events::events::AnnotationsDirChanged;
  using ImagesDirDB = dbs::ImagesDirDB::ImagesDB;

  virtual ~AnnotatorController() = default;
  AnnotatorController();

  virtual bool init(std::shared_ptr<app::ApplicationContext> ctx);

  virtual void handle(std::shared_ptr<ImagesDirChanged> event) override;
  virtual void handle(std::shared_ptr<AnnotationsDirChanged> event) override;

  virtual ImagesDirDB& get_images_db();

 private:
  std::shared_ptr<dbs::ImagesDirDB> images;
  std::shared_ptr<dbs::AnnotationsDirDB> annotations;
};

}  // namespace iannotator

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_ANNOTATOR_BUSINESS_LOGIC_ANNOTATORCONTROLLER_CLASS_H
