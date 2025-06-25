#include "src/annotator-business/AnnotatorController.h"

#include <cassert>
#include <memory>

#include "src/annotator-business/dbs/AnnotationsDirDB.h"
#include "src/annotator-business/dbs/ImagesDirDB.h"
#include "src/app/ApplicationContext.h"
#include "src/app/IApplication.h"
#include "src/log/log.h"

namespace iannotator
{

AnnotatorController::AnnotatorController()
    : images{std::make_shared<dbs::ImagesDirDB>()},
      annotations{std::make_shared<dbs::AnnotationsDirDB>()}
{
}

bool AnnotatorController::init(std::shared_ptr<app::ApplicationContext> ctx)
{
  assert(ctx != nullptr);
  assert(ctx->eventer != nullptr);
  assert(images != nullptr);
  assert(annotations != nullptr);

  if (ctx == nullptr) {
    LOGE("No valid context was provided");
    return false;
  }

  if (images == nullptr) {
    LOGE("No valid images db controller pointer was provided");
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

  if (!ctx->images_db_path.empty()) {
    LOGD("Found initial images directory path: " << ctx->images_db_path);

    if (!images->load_directory(ctx->images_db_path)) {
      LOGE("Fail to load directory: " << ctx->images_db_path);
      return false;
    }

    /// @todo: emit images db list changed over here
  }

  return true;
}

void AnnotatorController::handle(std::shared_ptr<ImagesDirChanged> event)
{
  assert(event != nullptr);
  assert(images != nullptr);

  if (event == nullptr) {
    LOGE("No valid event object pointer was given");
    return;
  }

  if (images == nullptr) {
    LOGE("No valid images db controller object pointer found");
    return;
  }

  if (!images->load_directory(event->images_dir)) {
    LOGE("Failure during the directory load: " << event->images_dir);
    return;
  }

  LOGD("The images db dir have changed: " << event->images_dir);
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

  LOGD("The annotations dir have changed: " << event->annotations_dir);
}

const AnnotatorController::ImagesDirDB& AnnotatorController::get_images_db()
{
  assert(images != nullptr);

  static const ImagesDirDB empty{};

  if (images == nullptr) {
    LOGE("No images object available");
    return empty;
  }

  return images->get_images_db();
}

}  // namespace iannotator
