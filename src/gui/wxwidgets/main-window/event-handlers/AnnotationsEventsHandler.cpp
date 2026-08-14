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

#include "src/gui/wxwidgets/main-window/event-handlers/AnnotationsEventsHandler.h"

#include <cassert>
#include <string>

#include "src/gui/wxwidgets/MainWindowContext.h"
#include "src/gui/wxwidgets/main-window/WindowDataContext.h"
#include "src/gui/wxwidgets/main-window/event-handlers/WindowOps.h"
#include "src/gui/wxwidgets/wx_str_utils.h"
#include "src/log/log.h"

namespace imagesAnnotatorWx::window::event_handlers
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

  mwctx->wloader->get_current_image_annotations()->Bind(
      wxEVT_LISTBOX, &AnnotationsEventsHandler::handle_ci_rect_row_selected,
      this);

  mwctx->wloader->get_delete_current_image_selected_annotation()->Bind(
      wxEVT_BUTTON,
      [this](wxCommandEvent&) { handle_current_rectangle_delete(); });

  mwctx->wloader->get_edit_current_rect_entry()->Bind(
      wxEVT_TEXT,
      [this](wxCommandEvent&) { handle_rect_edit_entry_changed(); });

  mwctx->wloader->get_annotations_db_list()->Bind(
      wxEVT_LISTBOX, &AnnotationsEventsHandler::handle_all_annotations_selected,
      this);

  mwctx->wloader->get_copy_annotation_name_button()->Bind(
      wxEVT_BUTTON,
      [this](wxCommandEvent&) { handle_annotations_name_copy(); });

  mwctx->wloader->get_copy_current_annotation_button()->Bind(
      wxEVT_BUTTON, [this](wxCommandEvent&) { handle_ci_annotation_copy(); });

  mwctx->wloader->get_annotation_search_entry()->Bind(
      wxEVT_TEXT,
      [this](wxCommandEvent&) { handle_annotations_search_text_changed(); });

  mwctx->wloader->get_window()->Bind(
      wxEVT_MENU, [this](wxCommandEvent&) { handle_auto_insert_to_whole(); },
      WindowDataContext::ID_AUTO_INSERT_TO_WHOLE);

  return true;
}

void AnnotationsEventsHandler::update_selected_rect_row_text(
    const std::string& nname)
{
  auto* ciList = mwctx->wloader->get_current_image_annotations();

  const int sel = ciList->GetSelection();

  if (sel != wxNOT_FOUND) {
    ciList->SetString(static_cast<unsigned int>(sel), u8(nname));
  }
}

void AnnotationsEventsHandler::handle_ci_rect_row_selected(
    wxCommandEvent& event)
{
  if (!MainWindowContext::validate_context(mwctx)) {
    LOGE("Invalid context pointer provided");
    return;
  }

  const int sel = event.GetSelection();

  if (sel == wxNOT_FOUND ||
      sel >= static_cast<int>(mwctx->currentVisualRects.size())) {
    mwctx->currentVisualRect.reset();
    LOGT("No valid current rect row selected");
    ops->update_action_buttons_sensitivity();
    return;
  }

  if (mwctx->current_image == nullptr) {
    LOGE("No current image selected");
    return;
  }

  auto rectLabel = mwctx->currentVisualRects[static_cast<size_t>(sel)];

  mwctx->currentVisualRect = rectLabel;

  mwctx->current_image->get_image_rec()->current_rect = rectLabel->get();

  ops->update_rect_edit_entry();

  LOGT("Current image rect changed: " << rectLabel->get_text());

  if (mwctx->centralCanvas != nullptr) {
    mwctx->centralCanvas->Refresh();
  }

  ops->update_action_buttons_sensitivity();
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

  const std::string nname =
      from_wx(mwctx->wloader->get_edit_current_rect_entry()->GetValue());

  LOGT("Renaming current image selected rectangle to " << nname);

  mwctx->current_image->get_image_rec()->current_rect->name = nname;
  mwctx->currentVisualRect->set_text(nname);
  update_selected_rect_row_text(nname);

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
    LOGT("Current image no longer has rects, deleting the marking");
    mwctx->current_image->remove_has_records_mark();
    ops->update_current_image_row_marking();
  }

  LOGT("Current rect erased");
}

void AnnotationsEventsHandler::handle_all_annotations_selected(
    wxCommandEvent& event)
{
  if (!MainWindowContext::validate_context(mwctx)) {
    LOGE("Invalid context pointer provided");
    return;
  }

  const int sel = event.GetSelection();

  if (sel == wxNOT_FOUND ||
      sel >= static_cast<int>(ops->displayedAnnotations.size())) {
    LOGT("No valid all-annotations row selected");
    return;
  }

  mwctx->current_annotation_name =
      ops->displayedAnnotations[static_cast<size_t>(sel)];

  LOGT("Current label changed to "
       << mwctx->current_annotation_name->get_text());

  ops->update_statuses();

  ops->update_action_buttons_sensitivity();
}

void AnnotationsEventsHandler::handle_annotations_name_copy()
{
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

  if (mwctx->current_image->get_image_rec()->current_rect == nullptr) {
    LOGT("No current image rect record selected");
    return;
  }

  const std::string nname = mwctx->current_annotation_name->get_text();

  mwctx->current_image->get_image_rec()->current_rect->name = nname;
  mwctx->currentVisualRect->set_text(nname);
  update_selected_rect_row_text(nname);

  ops->update_rect_edit_entry();
  ops->update_statuses();
}

void AnnotationsEventsHandler::handle_ci_annotation_copy()
{
  if (!MainWindowContext::validate_context(mwctx)) {
    LOGE("Invalid context pointer provided");
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
    const std::string e = "No current image available";
    LOGT(e);
    ops->show_error_dialog(e);
    return;
  }

  if (!mwctx->current_image_original.IsOk()) {
    const std::string e = "No current image buffer available";
    LOGT(e);
    ops->show_error_dialog(e);
    return;
  }

  mwctx->current_image->mark_as_has_records();
  ops->update_current_image_row_marking();

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
  ir->current_rect->width = mwctx->current_image_original.GetWidth();
  ir->current_rect->height = mwctx->current_image_original.GetHeight();

  ops->update_current_rects_list();
  ops->update_statuses();
}

}  // namespace imagesAnnotatorWx::window::event_handlers
