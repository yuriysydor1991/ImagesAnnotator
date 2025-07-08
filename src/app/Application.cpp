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

  abcontroller = create_annotator_b_controller();

  if (!abcontroller->init(ctx)) {
    LOGE(
        "The business controller signalled about invalid "
        "state");
    return INVALID;
  }

  gtkmmIniter = create_gtkmm_initer();

  auto rt = gtkmmIniter->run(ctx) ? 0 : INVALID;

  if (rt == INVALID) {
    LOGE("The gtkmm controller returned invalid status");
  }

  gtkmmIniter->deinit();
  abcontroller->deinit();
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
