#include "src/annotator-business/AnnotatorController.h"

#include <cassert>
#include <memory>

#include "src/annotator-business/dbs/AnnotationsDBs/AnnotationsDirDB.h"
#include "src/annotator-business/dbs/ImagesLoaders/ImagesDirLoader.h"
#include "src/app/ApplicationContext.h"
#include "src/app/IApplication.h"
#include "src/log/log.h"

namespace iannotator
{

AnnotatorController::AnnotatorController()
    : annotations{create_project_data_instance()}, actx{}
{
}

std::shared_ptr<dbs::annotations::AnnotationsDirDB>
AnnotatorController::create_project_data_instance()
{
  return std::make_shared<dbs::annotations::AnnotationsDirDB>();
}

bool AnnotatorController::init(std::shared_ptr<app::ApplicationContext> ctx)
{
  assert(ctx != nullptr);
  assert(ctx->eventer != nullptr);
  assert(annotations != nullptr);

  if (ctx == nullptr) {
    LOGE("No valid context was provided");
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

  if (!ctx->annotation_db_path.empty()) {
    LOGD("Found initial annotations directory path: "
         << ctx->annotation_db_path);

    if (!annotations->load_db(ctx->annotation_db_path)) {
      LOGE("Fail to load directory: " << ctx->annotation_db_path);
      return false;
    }
  }

  if (!ctx->images_db_path.empty()) {
    LOGD("Found initial images directory path: " << ctx->images_db_path);

    try_to_append_images_dir(ctx->images_db_path);

    /// @todo: emit images db list changed over here
    // meaningless to send events at this point since window is not created in
    // the App class.
  }

  auto mptr = shared_from_this();

  ctx->eventer->subscribe(
      std::shared_ptr<RequestImagesDirProviderHandler>(mptr));
  ctx->eventer->subscribe(std::shared_ptr<CurrentImageChangedHandler>(mptr));
  ctx->eventer->subscribe(std::shared_ptr<AnnotationsDirChangedIHandler>(mptr));
  ctx->eventer->subscribe(std::shared_ptr<ImagesDirChangedIHandler>(mptr));
  ctx->eventer->subscribe(std::shared_ptr<StoreRequestHandler>(mptr));
  ctx->eventer->subscribe(std::shared_ptr<CloseCurrentProjectHandler>(mptr));

  return true;
}

void AnnotatorController::handle(std::shared_ptr<ImagesDirChanged> event)
{
  assert(event != nullptr);
  assert(annotations != nullptr);

  if (event == nullptr) {
    LOGE("No valid event object pointer was given");
    return;
  }

  if (annotations == nullptr) {
    LOGE("No annotations object available");
    return;
  }

  try_to_append_images_dir(event->images_dir);

  LOGD("The images db dir " << event->images_dir << " have been loaded"
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

  if (!annotations->load_db(event->annotations_dir)) {
    LOGE("Fail to load the annotations db: " << event->annotations_dir);
    return;
  }

  LOGI("The annotations db " << event->annotations_dir << " have been loaded"
                             << " submitting appropriate event");

  emitImagesProviderChanged();
}

AnnotatorController::ImageRecordsSet& AnnotatorController::get_images_db()
{
  assert(annotations != nullptr);

  static ImageRecordsSet empty{};

  if (annotations == nullptr) {
    LOGE("No images object available");
    return empty;
  }

  return annotations->get_images_db();
}

std::string AnnotatorController::get_db_path()
{
  assert(annotations != nullptr);

  if (annotations == nullptr) {
    LOGE("No images object available");
    return {};
  }

  return annotations->get_db_path();
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
      efactory->create_images_dir_provider_changed(annotations);

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

AnnotatorController::ImageRecordsSet
AnnotatorController::load_fs_images_records(const std::string& path)
{
  const auto newLoaded = std::make_shared<dbs::images::ImagesDirLoader>();

  return newLoaded->load(path);
}

void AnnotatorController::try_to_append_images_dir(const std::string& path)
{
  auto irs = load_fs_images_records(path);

  annotations->add_images_db(irs);
}

void AnnotatorController::handle(std::shared_ptr<StoreRequest> event)
{
  assert(annotations != nullptr);
  assert(event != nullptr);

  if (event == nullptr) {
    LOGE("Invalid event pointer given");
    return;
  }

  if (event->dbpath.empty()) {
    if (!annotations->store_db()) {
      LOGE("Fail to store data");
    }
    return;
  }

  LOGT("Trying to store to the path: " << event->dbpath);

  if (!annotations->store_db(event->dbpath)) {
    LOGE("Fail to store data to path " << event->dbpath);
  }
}

void AnnotatorController::handle(
    [[maybe_unused]] std::shared_ptr<CloseCurrentProject> event)
{
  LOGT("Erasing previous project data and creating a new data instance");

  annotations = create_project_data_instance();

  emitImagesProviderChanged();
}

}  // namespace iannotator
