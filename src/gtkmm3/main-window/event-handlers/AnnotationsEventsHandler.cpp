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

#include "src/gtkmm3/main-window/event-handlers/AnnotationsEventsHandler.h"

#include <cassert>
#include <memory>
#include <string>

#include "src/gtkmm3/MainWindowContext.h"
#include "src/gtkmm3/main-window/custom-widgets/AllAnnotationsLabel.h"
#include "src/gtkmm3/main-window/custom-widgets/ImageRectsLabel.h"
#include "src/gtkmm3/main-window/event-handlers/WindowOps.h"
#include "src/log/log.h"

namespace templateGtkmm3::window::event_handlers
{

AnnotationsEventsHandler::AnnotationsEventsHandler(
    std::shared_ptr<WindowOps> nops)
    : AbstractWindowEventHandler(nops)
{
}

bool AnnotationsEventsHandler::subscribe()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);

  auto* cImageA = mwctx->wloader->get_current_image_annotations();
  auto* crDeleteB =
      mwctx->wloader->get_delete_current_image_selected_annotation();
  auto* rectEntry = mwctx->wloader->get_edit_current_rect_entry();
  auto* allAnnL = mwctx->wloader->get_annotations_db_list();
  auto* copyNameB = mwctx->wloader->get_copy_annotation_name_button();
  auto* dupCurretAnn = mwctx->wloader->get_copy_current_annotation_button();
  auto* aSearchE = mwctx->wloader->get_annotation_search_entry();
  auto* insertWholeM =
      mwctx->wloader->auto_insert_current_annotation_to_whole_mi();

  cImageA->signal_row_selected().connect(sigc::mem_fun(
      *this, &AnnotationsEventsHandler::handle_ci_rect_row_selected));

  crDeleteB->signal_clicked().connect(sigc::mem_fun(
      *this, &AnnotationsEventsHandler::handle_current_rectangle_delete));

  rectEntry->signal_changed().connect(sigc::mem_fun(
      *this, &AnnotationsEventsHandler::handle_rect_edit_entry_changed));

  allAnnL->signal_row_selected().connect(sigc::mem_fun(
      *this, &AnnotationsEventsHandler::handle_all_annotations_selected));

  copyNameB->signal_clicked().connect(sigc::mem_fun(
      *this, &AnnotationsEventsHandler::handle_annotations_name_copy));

  dupCurretAnn->signal_clicked().connect(sigc::mem_fun(
      *this, &AnnotationsEventsHandler::handle_ci_annotation_copy));

  aSearchE->signal_search_changed().connect(sigc::mem_fun(
      *this, &AnnotationsEventsHandler::handle_annotations_search_text_changed));

  insertWholeM->signal_activate().connect(sigc::mem_fun(
      *this, &AnnotationsEventsHandler::handle_auto_insert_to_whole));

  return true;
}

void AnnotationsEventsHandler::handle_ci_rect_row_selected(Gtk::ListBoxRow* row)
{
  assert(MainWindowContext::validate_context(mwctx));

  if (!MainWindowContext::validate_context(mwctx)) {
    LOGE("Invalid context pointer provided");
    return;
  }

  if (row == nullptr) {
    mwctx->currentVisualRect.reset();
    LOGT("No row pointer provided");
    return;
  }

  ImageRectsLabel* rectLabel = dynamic_cast<ImageRectsLabel*>(row->get_child());

  assert(rectLabel != nullptr);

  if (rectLabel == nullptr) {
    LOGE("Invalid label type for the obtained row widget");
    return;
  }

  if (mwctx->current_image == nullptr) {
    LOGE("No current image selected");
    return;
  }

  mwctx->currentVisualRect = rectLabel->shared_from_this();

  assert(mwctx->current_image->get_image_rec() != nullptr);

  mwctx->current_image->get_image_rec()->current_rect = rectLabel->get();

  ops->update_rect_edit_entry();

  LOGT("Current image rect changed: " << rectLabel->get_text());

  mwctx->centralCanvas->queue_draw();
}

void AnnotationsEventsHandler::handle_rect_edit_entry_changed()
{
  assert(mwctx != nullptr);

  if (mwctx->current_image == nullptr) {
    LOGT("No current image selected");
    return;
  }

  if (mwctx->currentVisualRect == nullptr) {
    LOGT("No current visual rect contained");
    return;
  }

  if (mwctx->current_image->get_image_rec()->current_rect == nullptr) {
    LOGT("No selected current rectangle");
    return;
  }

  auto* redit = mwctx->wloader->get_edit_current_rect_entry();

  assert(mwctx->current_image->get_image_rec() != nullptr);

  const std::string nname = redit->get_text();

  LOGT("Renaming current image selected rectangle to " << nname);

  mwctx->current_image->get_image_rec()->current_rect->name = nname;
  mwctx->currentVisualRect->set_text(nname);

  ops->update_annotations_list();
  ops->update_statuses();
}

void AnnotationsEventsHandler::handle_current_rectangle_delete()
{
  assert(mwctx != nullptr);

  if (mwctx->current_image == nullptr) {
    LOGE("No current image avaialble");
    return;
  }

  if (mwctx->current_image->get_image_rec()->current_rect == nullptr) {
    LOGT("No current rect avaialble");
    return;
  }

  mwctx->current_image->get_image_rec()->erase_current_rect();

  ops->update_current_rects_list();
  ops->update_annotations_list();
  ops->update_rect_edit_entry();
  ops->update_statuses();

  if (mwctx->current_image->get_image_rec()->rects.empty()) {
    LOGT("Current images no longer have rects, deleting the marking");
    mwctx->current_image->remove_has_records_mark();
  }

  LOGT("Current rect erased");
}

void AnnotationsEventsHandler::handle_all_annotations_selected(
    Gtk::ListBoxRow* row)
{
  assert(MainWindowContext::validate_context(mwctx));

  if (!MainWindowContext::validate_context(mwctx)) {
    LOGE("Invalid context pointer provided");
    return;
  }

  if (row == nullptr) {
    LOGT("No row pointer provided");
    return;
  }

  AllAnnotationsLabel* aLabel =
      dynamic_cast<AllAnnotationsLabel*>(row->get_child());

  if (aLabel == nullptr) {
    LOGE("Unexpected label type provided");
    return;
  }

  mwctx->current_annotation_name = aLabel->shared_from_this();

  LOGT("Current label changed to "
       << mwctx->current_annotation_name->get_text());

  ops->update_statuses();
}

void AnnotationsEventsHandler::handle_annotations_name_copy()
{
  assert(MainWindowContext::validate_context(mwctx));

  if (!MainWindowContext::validate_context(mwctx)) {
    LOGE("Invalid context pointer provided");
    return;
  }

  if (mwctx->current_annotation_name == nullptr) {
    LOGT("No current all annotations list row selected");
    return;
  }

  if (mwctx->current_image == nullptr) {
    LOGT("No current image selected");
    return;
  }

  if (mwctx->currentVisualRect == nullptr) {
    LOGT("No current image rect selected");
    return;
  }

  assert(mwctx->current_image->get_image_rec() != nullptr);

  if (mwctx->current_image->get_image_rec()->current_rect == nullptr) {
    LOGT("No current image rect record selected");
    return;
  }

  mwctx->current_image->get_image_rec()->current_rect->name =
      mwctx->current_annotation_name->get_text();

  mwctx->currentVisualRect->set_text(
      mwctx->current_annotation_name->get_text());

  ops->update_rect_edit_entry();
  ops->update_statuses();
}

void AnnotationsEventsHandler::handle_ci_annotation_copy()
{
  assert(MainWindowContext::validate_context(mwctx));

  if (!MainWindowContext::validate_context(mwctx)) {
    LOGE("Invalid context pointer provided");
    return;
  }

  if (mwctx->current_annotation_name == nullptr) {
    LOGT("No current all annotations list row selected");
    return;
  }

  if (mwctx->current_image == nullptr) {
    LOGT("No current image selected");
    return;
  }

  if (mwctx->currentVisualRect == nullptr) {
    LOGT("No current image rect selected");
    return;
  }

  assert(mwctx->current_image->get_image_rec() != nullptr);

  if (mwctx->current_image->get_image_rec()->current_rect == nullptr) {
    LOGT("No current image rect record selected");
    return;
  }

  auto dup =
      mwctx->current_image->get_image_rec()->current_rect->duplicate_shared();

  mwctx->current_image->get_image_rec()->current_rect = dup;
  mwctx->current_image->get_image_rec()->rects.emplace_back(dup);

  ops->update_current_rects_list();
  ops->update_statuses();
}

void AnnotationsEventsHandler::handle_annotations_search_text_changed()
{
  ops->apply_annotations_filter();
}

void AnnotationsEventsHandler::handle_auto_insert_to_whole()
{
  assert(mwctx != nullptr);

  if (mwctx->current_image == nullptr) {
    static const std::string e = "No current image available";
    LOGT(e);
    ops->show_error_dialog(e);
    return;
  }

  auto& pixbuf = mwctx->current_image_original_pixbuf;

  assert(pixbuf);

  if (!pixbuf) {
    static const std::string e = "No current image buffer available";
    LOGT(e);
    ops->show_error_dialog(e);
    return;
  }

  mwctx->current_image->mark_as_has_records();

  auto ir = mwctx->current_irecord();

  assert(ir != nullptr);

  auto efactory = mwctx->actx->eventer->get_events_factory();

  assert(efactory != nullptr);

  if (efactory == nullptr) {
    LOGE("No factory available");
    return;
  }

  ir->current_rect = efactory->create_image_rect_record(ir);
  ir->rects.emplace_back(ir->current_rect);

  assert(ir->current_rect != nullptr);

  if (mwctx->current_annotation_name != nullptr) {
    LOGT("Setting text to the currently selected "
         << mwctx->current_annotation_name->get_text());
    ir->current_rect->name = mwctx->current_annotation_name->get_text();
  }

  ir->current_rect->x = 0;
  ir->current_rect->y = 0;

  ir->current_rect->width = pixbuf->get_width();
  ir->current_rect->height = pixbuf->get_height();

  ops->update_current_rects_list();
  ops->update_statuses();
}

}  // namespace templateGtkmm3::window::event_handlers
