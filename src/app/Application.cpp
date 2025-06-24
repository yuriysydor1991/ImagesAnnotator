#include "src/app/Application.h"

#include <cassert>
#include <iostream>
#include <memory>

#include "src/annotator-business/AnnotatorController.h"
#include "src/gtkmm3/GtkmmIniter.h"
#include "src/log/log.h"

namespace app
{

int Application::run(std::shared_ptr<ApplicationContext> ctx)
{
  assert(ctx != nullptr);

  if (ctx == nullptr) {
    LOGE("No valid context pointer provided");
    return INVALID;
  }

  ctx->eventer = create_event_controller(ctx);

  gtkmmIniter = create_gtkmm_initer();

  abcontroller = create_annotator_b_controller();

  if (!abcontroller->init(ctx)) {
    LOGE(
        "The business controller signalled about invalid "
        "state");
    return INVALID;
  }

  auto rt = gtkmmIniter->run(ctx) ? 0 : INVALID;

  if (rt == INVALID) {
    LOGE("The gtkmm controller returned invalid status");
  }

  ctx->eventer->deinit();

  return rt;
}

std::shared_ptr<templateGtkmm3::GtkmmIniter> Application::create_gtkmm_initer()
{
  return std::make_shared<templateGtkmm3::GtkmmIniter>();
}

std::shared_ptr<iannotator::AnnotatorController>
Application::create_annotator_b_controller()
{
  return std::make_shared<iannotator::AnnotatorController>();
}

std::shared_ptr<events::ImagesAnnotatorEventController>
Application::create_event_controller(std::shared_ptr<ApplicationContext> ctx)
{
  return std::make_shared<events::ImagesAnnotatorEventController>(ctx);
}

}  // namespace app
