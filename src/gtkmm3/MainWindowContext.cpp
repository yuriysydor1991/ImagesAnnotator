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

#include "src/gtkmm3/MainWindowContext.h"

#include <cassert>
#include <memory>

#include "src/app/ApplicationContext.h"
#include "src/gtkmm3/ComponentTypesAliases.h"
#include "src/gtkmm3/main-window/WindowDataContext.h"
#include "src/gtkmm3/main-window/WindowLoader.h"
#include "src/gtkmm3/main-window/custom-widgets/CustomWidgetsFactory.h"
#include "src/log/log.h"

namespace templateGtkmm3
{

std::shared_ptr<MainWindowContext> MainWindowContext::build_context()
{
  auto nmwctx = std::make_shared<MainWindowContext>();

  nmwctx->wctx = std::make_shared<window::WindowDataContext>();
  nmwctx->cwFactory = std::make_shared<CWFactory>();
  nmwctx->wloader = std::make_shared<window::WindowLoader>();
  nmwctx->mweHandler = std::make_shared<window::WindowEventsHandler>();

  return nmwctx;
}

void MainWindowContext::clear()
{
  actx.reset();
  wctx.reset();
  mweHandler.reset();
  cwFactory.reset();
  wloader.reset();
  images_provider.reset();
}

std::shared_ptr<MainWindowContext::ImageRecord>
MainWindowContext::current_irecord()
{
  if (current_image == nullptr) {
    LOGT("No current image available");
    return {};
  }

  return current_image->get_image_rec();
}

bool MainWindowContext::validate_context(
    std::shared_ptr<MainWindowContext> nmwctx)
{
  assert(nmwctx != nullptr);
  assert(nmwctx->wctx != nullptr);
  assert(nmwctx->wloader != nullptr);
  assert(nmwctx->cwFactory != nullptr);
  assert(nmwctx->actx != nullptr);
  assert(nmwctx->actx->eventer != nullptr);
  assert(nmwctx->mweHandler != nullptr);

  if (nmwctx == nullptr) {
    LOGE("No valid context pointer provided");
    return false;
  }

  if (nmwctx->wctx == nullptr) {
    LOGE("No window context pointer provided");
    return false;
  }

  if (nmwctx->wloader == nullptr) {
    LOGE("No window builder instance provided");
    return false;
  }

  if (nmwctx->cwFactory == nullptr) {
    LOGE("No custom widgets factory pointer provided");
    return false;
  }

  if (!nmwctx->wloader->all_widget_are_valid()) {
    LOGE("Not all widgets are valid inside the window builder");
    return false;
  }

  if (nmwctx->actx == nullptr) {
    LOGE("Context does not contain a valid application context pointer");
    return false;
  }

  if (nmwctx->actx->eventer == nullptr) {
    LOGE("No valid iventer pointer provided in the context");
    return false;
  }

  if (nmwctx->mweHandler == nullptr) {
    LOGE("No valid main window event handler pointer available");
    return false;
  }

  if (nmwctx->centralCanvas == nullptr) {
    LOGE("No central canvas widget available");
    return false;
  }

  return true;
}

MainWindowContext::ImageRectsLabelPtr
MainWindowContext::find_current_image_current_visual_rect()
{
  if (current_image == nullptr) {
    LOGT("No current image selected");
    return {};
  }

  assert(current_image->get_image_rec() != nullptr);

  if (current_image->get_image_rec()->current_rect == nullptr) {
    LOGT("No current image rect selected");
    return {};
  }

  for (auto& r : currentVisualRects) {
    if (r->get() == current_image->get_image_rec()->current_rect) {
      currentVisualRect = r;
      return r;
    }
  }

  LOGT("Current image rect not found");

  return {};
}

}  // namespace templateGtkmm3
