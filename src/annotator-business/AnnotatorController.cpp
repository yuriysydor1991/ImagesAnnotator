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

bool AnnotatorController::init(std::shared_ptr<app::ApplicationContext> ctx)
{
  assert(ctx != nullptr);
  assert(ctx->eventer != nullptr);

  if (ctx == nullptr) {
    LOGE("No valid context was provided");
    return false;
  }

  if (ctx->eventer == nullptr) {
    LOGE("No valid context' eventer was provided");
    return false;
  }

  return true;
}

void AnnotatorController::handle(std::shared_ptr<ImagesDirChanged> event)
{
  assert(event != nullptr);

  if (event == nullptr) {
    LOGE("No valid event object pointer was given");
    return;
  }

  LOGD("The images dir have changed: " << event->images_dir);
}

void AnnotatorController::handle(std::shared_ptr<AnnotationsDirChanged> event)
{
  assert(event != nullptr);

  if (event == nullptr) {
    LOGE("No valid event object pointer was given");
    return;
  }

  LOGD("The annotations dir have changed: " << event->annotations_dir);
}

}  // namespace iannotator
