#include "src/annotator-business/AnnotatorController.h"

#include <cassert>
#include <memory>

#include "src/annotator-business/dbs/AnnotationsDBs/AnnotationsDirDB.h"
#include "src/annotator-business/dbs/ImagesDBs/ImagesDirDB.h"
#include "src/app/ApplicationContext.h"
#include "src/app/IApplication.h"
#include "src/log/log.h"

namespace iannotator
{

AnnotatorController::AnnotatorController()
    : images{std::make_shared<dbs::ImagesDirDB>()},
      annotations{std::make_shared<dbs::AnnotationsDirDB>()},
      actx{}
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

  actx = ctx;

  if (!ctx->images_db_path.empty()) {
    LOGD("Found initial images directory path: " << ctx->images_db_path);

    if (!images->load_directory(ctx->images_db_path)) {
      LOGE("Fail to load directory: " << ctx->images_db_path);
      return false;
    }

    /// @todo: emit images db list changed over here
    // meaningless to send events at this point since window is not created in
    // the App class.
  }

  auto mptr = shared_from_this();

  ctx->eventer->subscribe(
      std::shared_ptr<RequestImagesDirProviderHandler>(mptr));
  ctx->eventer->subscribe(std::shared_ptr<CurrentImageChangedHandler>(mptr));

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

  LOGD("The images db dir have changed: " << event->images_dir
                                          << " submitting appropriate event");

  emitImagesProviderChanged();
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

AnnotatorController::ImagesDirDB& AnnotatorController::get_images_db()
{
  assert(images != nullptr);

  static ImagesDirDB empty{};

  if (images == nullptr) {
    LOGE("No images object available");
    return empty;
  }

  return images->get_images_db();
}

void AnnotatorController::handle(
    [[maybe_unused]] std::shared_ptr<RequestImagesDirProvider> event)
{
  emitImagesProviderChanged();
}

void AnnotatorController::emitImagesProviderChanged()
{
  assert(actx != nullptr);
  assert(actx->eventer != nullptr);

  if (actx == nullptr) {
    LOGE("No valid application context pointer found");
    return;
  }

  if (actx->eventer == nullptr) {
    LOGE("No valid application context' eventer pointer found");
    return;
  }

  auto efactory = actx->eventer->get_events_factory();

  auto imagesProviderChangedE =
      efactory->create_images_dir_provider_changed(images);

  actx->eventer->submit(imagesProviderChangedE);
}

void AnnotatorController::deinit() { actx.reset(); }

void AnnotatorController::handle(std::shared_ptr<CurrentImageChanged> event)
{
  assert(event != nullptr);
  assert(event->new_current_image != nullptr);

  if (event == nullptr) {
    LOGE("No valid image event pointer provided");
    return;
  }

  if (event->new_current_image == nullptr) {
    LOGE("No new current image pointer was provided");
    return;
  }

  LOGI("Current image changed: " << event->new_current_image->path);

  /// @todo: Insert the current image change handler over here
}

}  // namespace iannotator
