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

#include "src/gui/wxwidgets/main-window/event-handlers/ImageListEventsHandler.h"

#include <cassert>
#include <string>

#include "src/gui/wxwidgets/MainWindowContext.h"
#include "src/gui/wxwidgets/main-window/event-handlers/WindowOps.h"
#include "src/gui/wxwidgets/wx_str_utils.h"
#include "src/log/log.h"

namespace templateWx::window::event_handlers
{

ImageListEventsHandler::ImageListEventsHandler(std::shared_ptr<WindowOps> nops)
    : AbstractWindowEventHandler(nops)
{
  iloader = ::helpers::ImageLoader::create();

  assert(iloader != nullptr);
}

bool ImageListEventsHandler::subscribe()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);

  mwctx->wloader->get_images_list()->Bind(
      wxEVT_LIST_ITEM_SELECTED,
      &ImageListEventsHandler::handle_images_row_selected, this);

  mwctx->wloader->get_images_search_entry()->Bind(
      wxEVT_TEXT,
      [this](wxCommandEvent&) { handle_images_search_text_changed(); });

  mwctx->wloader->get_delete_image_record()->Bind(
      wxEVT_BUTTON, [this](wxCommandEvent&) { handle_current_image_delete(); });

  return true;
}

void ImageListEventsHandler::handle_images_row_selected(wxListEvent& event)
{
  if (!MainWindowContext::validate_context(mwctx)) {
    LOGE("Invalid context pointer provided");
    return;
  }

  if (mwctx->centralCanvas == nullptr) {
    LOGE("No central canvas available");
    return;
  }

  LOGT("Clearing the canvas");
  mwctx->centralCanvas->clear();

  auto label = ops->images_row_label(event.GetIndex());

  if (label == nullptr) {
    mwctx->current_image.reset();
    LOGD("No row label available");
    return;
  }

  mwctx->current_image = label;

  auto ir = label->get_image_rec();

  if (ir == nullptr) {
    LOGE("Invalid image record pointer retrieved");
    return;
  }

  auto eventsFactory = mwctx->actx->eventer->get_events_factory();

  if (!load_image(ir)) {
    LOGE("Failure while loading the image");
    return;
  }

  ops->update_image_zoom();
  ops->update_current_rects_list();

  LOGT("New image selected: " << ir->get_full_path());

  auto event2 = eventsFactory->create_current_image_changed(ir);

  mwctx->actx->eventer->submit(event2);
}

void ImageListEventsHandler::normilize_initial_image_load_scale()
{
  if (mwctx->current_image == nullptr) {
    LOGE("No image loaded");
    return;
  }

  auto ir = mwctx->current_image->get_image_rec();

  assert(ir != nullptr);

  ir->iwidth = mwctx->current_image_original.GetWidth();
  ir->iheight = mwctx->current_image_original.GetHeight();

  // Compute the initial fit-to-window scale only when the image has no
  // user-selected scale yet (imageScale == 0). Once a scale is set - by this
  // fit, by a user zoom, or restored from the project JSON - it is preserved.
  if (ir->imageScale <= 0.0) {
    const int origHeight = mwctx->current_image_original.GetHeight();
    const int centralHeight = mwctx->centralCanvas->GetClientSize().GetHeight();

    if (origHeight > 0 && centralHeight > 0) {
      ir->imageScale =
          static_cast<double>(centralHeight) / static_cast<double>(origHeight);
      ir->imageScale /= load_image_scale_helper;
    } else {
      ir->imageScale = 1.0;
    }
  }

  LOGT("Image scale factor: " << ir->imageScale);

  ops->update_statuses();
}

bool ImageListEventsHandler::load_image(ImageRecordPtr ir)
{
  assert(ir != nullptr);
  assert(iloader != nullptr);

  if (ir == nullptr) {
    const std::string emsg = "Invalid image record pointer provided";
    LOGE(emsg);
    ops->show_error_dialog(emsg);
    return false;
  }

  ops->show_spinner();

  if (!iloader->load(ir)) {
    const std::string emsg =
        "Failure to load the image from the network " + ir->get_full_path();
    LOGE(emsg);
    ops->show_error_dialog(emsg);
    return false;
  }

  const std::string filepath = ir->get_full_path();

  {
    // Suppress wxWidgets' own error pop-up on a failed decode; we present our
    // own message below.
    wxLogNull noLog;

    if (!mwctx->current_image_original.LoadFile(u8(filepath))) {
      const std::string emsg = "Fail to load the image:\n\n" + filepath;
      LOGE(emsg);
      ops->show_error_dialog(emsg);
      return false;
    }
  }

  if (!mwctx->current_image_original.IsOk()) {
    const std::string emsg = "Fail to load the image:\n\n" + filepath;
    LOGE(emsg);
    ops->show_error_dialog(emsg);
    return false;
  }

  LOGD("Image seems to be loaded " << filepath);
  normilize_initial_image_load_scale();

  ops->hide_spinner();

  return true;
}

void ImageListEventsHandler::handle_images_search_text_changed()
{
  if (!MainWindowContext::validate_context(mwctx)) {
    LOGE("Invalid context pointer provided");
    return;
  }

  ops->apply_images_filter();
}

void ImageListEventsHandler::handle_current_image_delete()
{
  if (!MainWindowContext::validate_context(mwctx)) {
    LOGE("Invalid context pointer provided");
    return;
  }

  auto ir = mwctx->current_irecord();

  if (ir == nullptr) {
    LOGD("No current image selected");
    return;
  }

  if (!ir->rects.empty()) {
    LOGD("Image contains some annotations, asking user");
    if (!ops->ask_about_rects_delete()) {
      LOGD("user cancels the image with annotations deletion");
      return;
    }
  }

  assert(mwctx->actx->eventer != nullptr);

  auto efactory = mwctx->actx->eventer->get_events_factory();

  assert(efactory != nullptr);

  auto deleteEvent =
      efactory->create_delete_current_image_request(ir->get_full_path());

  assert(deleteEvent != nullptr);

  mwctx->actx->eventer->submit(deleteEvent);
}

}  // namespace templateWx::window::event_handlers
