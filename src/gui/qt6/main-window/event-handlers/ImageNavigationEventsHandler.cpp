/**
 * The "Simplified BSD License"
 *
 * Copyright (c) 2026, Yurii Sydor (yuriysydor1991@gmail.com)
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

#include "src/gui/qt6/main-window/event-handlers/ImageNavigationEventsHandler.h"

#include <cassert>

#include "src/gui/qt6/MainWindowContext.h"
#include "src/gui/qt6/main-window/event-handlers/WindowOps.h"
#include "src/gui/qt6/qt_includes.h"
#include "src/log/log.h"

namespace imagesAnnotatorQt6::window::event_handlers
{

ImageNavigationEventsHandler::ImageNavigationEventsHandler(
    std::shared_ptr<WindowOps> nops)
    : AbstractWindowEventHandler(nops)
{
}

bool ImageNavigationEventsHandler::subscribe()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);

  auto* window = mwctx->wloader->get_window();

  QObject::connect(mwctx->wloader->get_prev_file_button(),
                   &QPushButton::clicked, window,
                   [this]() { handle_prev_file(); });
  QObject::connect(mwctx->wloader->get_next_file_button(),
                   &QPushButton::clicked, window,
                   [this]() { handle_next_file(); });

  return true;
}

void ImageNavigationEventsHandler::handle_next_file()
{
  const int count = static_cast<int>(ops->displayedImages.size());

  if (count == 0) {
    return;
  }

  const int sel = ops->images_selected_index();

  if (sel == WindowOps::no_selection) {
    ops->select_image_row(0);
    return;
  }

  if (sel + 1 >= count) {
    LOGD("The end of the images list reached");
    return;
  }

  ops->select_image_row(sel + 1);

  ops->update_rect_edit_entry();
}

void ImageNavigationEventsHandler::handle_prev_file()
{
  const int count = static_cast<int>(ops->displayedImages.size());

  if (count == 0) {
    return;
  }

  const int sel = ops->images_selected_index();

  if (sel == WindowOps::no_selection) {
    ops->select_image_row(count - 1);
    return;
  }

  if (sel - 1 < 0) {
    LOGD("The top of the images list reached");
    return;
  }

  ops->select_image_row(sel - 1);

  ops->update_rect_edit_entry();
}

}  // namespace imagesAnnotatorQt6::window::event_handlers
