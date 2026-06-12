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

#include "src/gtkmm3/main-window/event-handlers/ImageListEventsHandler.h"

#include <cassert>
#include <memory>
#include <string>

#include "src/gtkmm3/MainWindowContext.h"
#include "src/gtkmm3/main-window/custom-widgets/ImagePathLabel.h"
#include "src/gtkmm3/main-window/event-handlers/WindowOps.h"
#include "src/log/log.h"

namespace templateGtkmm3::window::event_handlers
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

  auto* imagesListBox = mwctx->wloader->get_images_list();
  auto* imagesSearchE = mwctx->wloader->get_images_search_entry();
  auto* deleteImageB = mwctx->wloader->get_delete_image_record();

  imagesListBox->signal_row_selected().connect(sigc::mem_fun(
      *this, &ImageListEventsHandler::handle_images_row_selected));

  imagesSearchE->signal_search_changed().connect(sigc::mem_fun(
      *this, &ImageListEventsHandler::handle_images_search_text_changed));

  deleteImageB->signal_clicked().connect(
      sigc::mem_fun(*this, &ImageListEventsHandler::handle_current_image_delete));

  return true;
}

void ImageListEventsHandler::handle_images_row_selected(Gtk::ListBoxRow* row)
{
  assert(MainWindowContext::validate_context(mwctx));

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

  if (row == nullptr) {
    mwctx->current_image.reset();
    LOGD("No row pointer provided");
    return;
  }

  ImagePathLabel* label = dynamic_cast<ImagePathLabel*>(row->get_child());

  assert(label != nullptr);

  if (label == nullptr) {
    LOGE("Invalid label type for the obtained row widget");
    return;
  }

  mwctx->current_image = label->shared_from_this();

  auto ir = label->get_image_rec();

  assert(ir != nullptr);

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

  auto event = eventsFactory->create_current_image_changed(ir);

  mwctx->actx->eventer->submit(event);
}

void ImageListEventsHandler::normilize_initial_image_load_scale()
{
  assert(MainWindowContext::validate_context(mwctx));

  assert(mwctx->wloader->get_central_scrolled_window() != nullptr);

  if (mwctx->current_image == nullptr) {
    LOGE("No image loaded");
    return;
  }

  auto ir = mwctx->current_image->get_image_rec();

  assert(ir != nullptr);

  const auto origHeight = mwctx->current_image_original_pixbuf->get_height();
  const auto centralHeight =
      mwctx->wloader->get_central_scrolled_window()->get_allocated_height();

  ir->imageScale =
      static_cast<double>(centralHeight) / static_cast<double>(origHeight);
  ir->imageScale /= load_image_scale_helper;

  ir->iwidth = mwctx->current_image_original_pixbuf->get_width();
  ir->iheight = mwctx->current_image_original_pixbuf->get_height();

  LOGT("Image new scale factor: " << ir->imageScale);

  ops->update_statuses();
}

bool ImageListEventsHandler::load_image(ImageRecordPtr ir)
{
  assert(MainWindowContext::validate_context(mwctx));
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

  std::string errorIfAny;

  try {
    /// @todo seperate into the content loader infrastructure
    mwctx->current_image_original_pixbuf =
        Gdk::Pixbuf::create_from_file(filepath);
  }
  catch (const Glib::Error& ex) {
    errorIfAny = ex.what();
    const std::string emsg = "Fail to load the image:\n\n" + filepath +
                             (errorIfAny.empty() ? "" : "\n\n" + errorIfAny);
    LOGE(emsg);
    ops->show_error_dialog(emsg);
    return false;
  }

  if (!mwctx->current_image_original_pixbuf) {
    const std::string emsg = "Fail to load the image:\n\n" + filepath +
                             (errorIfAny.empty() ? "" : "\n\n" + errorIfAny);
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
  assert(MainWindowContext::validate_context(mwctx));

  if (!MainWindowContext::validate_context(mwctx)) {
    LOGE("Invalid context pointer provided");
    return;
  }

  auto* aListBox = mwctx->wloader->get_images_list();

  auto searchText = mwctx->wloader->get_images_search_entry()->get_text();

  const auto filter_text = searchText.lowercase();

  for (auto* child : aListBox->get_children()) {
    auto* row = dynamic_cast<Gtk::ListBoxRow*>(child);

    if (row == nullptr) {
      LOGT("Unexpected row type on the Gtk::ListBox");
      continue;
    }

    auto* label = dynamic_cast<ImagePathLabel*>(row->get_child());

    if (label == nullptr) {
      LOGT("Unexpected label type in the ListBox");
      continue;
    }

    Glib::ustring text = label->get_text().lowercase();

    row->set_visible(filter_text.empty() ||
                     text.find(filter_text) != Glib::ustring::npos);
  }
}

void ImageListEventsHandler::handle_current_image_delete()
{
  assert(MainWindowContext::validate_context(mwctx));

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
      LOGD("user confirms the iamge with annotation deletion");
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

}  // namespace templateGtkmm3::window::event_handlers
