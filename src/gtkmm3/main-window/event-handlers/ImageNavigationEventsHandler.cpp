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

#include "src/gtkmm3/main-window/event-handlers/ImageNavigationEventsHandler.h"

#include <algorithm>
#include <cassert>
#include <memory>

#include "src/gtkmm3/MainWindowContext.h"
#include "src/gtkmm3/main-window/event-handlers/WindowOps.h"
#include "src/log/log.h"

namespace templateGtkmm3::window::event_handlers
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

  auto* prevButton = mwctx->wloader->get_prev_file_button();
  auto* nextButton = mwctx->wloader->get_next_file_button();

  prevButton->signal_clicked().connect(
      sigc::mem_fun(*this, &ImageNavigationEventsHandler::handle_prev_file));
  nextButton->signal_clicked().connect(
      sigc::mem_fun(*this, &ImageNavigationEventsHandler::handle_next_file));

  return true;
}

void ImageNavigationEventsHandler::handle_next_file()
{
  auto* imagesBox = mwctx->wloader->get_images_list();

  assert(imagesBox != nullptr);

  auto children = imagesBox->get_children();
  auto* current = imagesBox->get_selected_row();

  if (current == nullptr && !children.empty()) {
    auto tmprit = std::find_if(children.begin(), children.end(),
                               get_wvisibility_unary_op());
    if (tmprit != children.end()) {
      ops->select_list_box_child(imagesBox, *tmprit);
    }
    return;
  }

  auto it = std::find(children.begin(), children.end(), current);

  if (it == children.end()) {
    LOGD("The end of the images list reached");
    return;
  }

  auto tmpit =
      std::find_if(std::next(it), children.end(), get_wvisibility_unary_op());

  if (tmpit == children.end()) {
    LOGD("The end of the images list reached");
    return;
  }

  ops->select_list_box_child(imagesBox, *tmpit);

  ops->update_rect_edit_entry();
}

void ImageNavigationEventsHandler::handle_prev_file()
{
  auto* imagesBox = mwctx->wloader->get_images_list();

  assert(imagesBox != nullptr);

  auto children = imagesBox->get_children();
  auto current = imagesBox->get_selected_row();

  if (current == nullptr && !children.empty()) {
    auto tmprit = std::find_if(children.rbegin(), children.rend(),
                               get_wvisibility_unary_op());
    if (tmprit != children.rend()) {
      ops->select_list_box_child(imagesBox, *tmprit);
    }
    return;
  }

  auto rit = std::find(children.rbegin(), children.rend(), current);

  if (rit == children.rend()) {
    LOGD("The top of the images list reached");
    return;
  }

  auto tmprit =
      std::find_if(std::next(rit), children.rend(), get_wvisibility_unary_op());

  if (tmprit == children.rend()) {
    LOGD("The top of the images list reached");
    return;
  }

  ops->select_list_box_child(imagesBox, *tmprit);

  ops->update_rect_edit_entry();
}

std::function<bool(const Gtk::Widget*)>
ImageNavigationEventsHandler::get_wvisibility_unary_op()
{
  return [](const Gtk::Widget* wptr) -> bool {
    assert(wptr != nullptr);
    return wptr->is_visible();
  };
}

}  // namespace templateGtkmm3::window::event_handlers
