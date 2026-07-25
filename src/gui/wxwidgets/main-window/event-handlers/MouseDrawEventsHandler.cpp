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

#include "src/gui/wxwidgets/main-window/event-handlers/MouseDrawEventsHandler.h"

#include <algorithm>
#include <cassert>
#include <cmath>

#include "src/gui/wxwidgets/MainWindowContext.h"
#include "src/gui/wxwidgets/main-window/event-handlers/WindowOps.h"
#include "src/gui/wxwidgets/wx_str_utils.h"
#include "src/log/log.h"

namespace templateWx::window::event_handlers
{

MouseDrawEventsHandler::MouseDrawEventsHandler(std::shared_ptr<WindowOps> nops)
    : AbstractWindowEventHandler(nops)
{
}

bool MouseDrawEventsHandler::subscribe()
{
  assert(mwctx != nullptr);
  assert(mwctx->centralCanvas != nullptr);

  auto* canvas = mwctx->centralCanvas;

  canvas->Bind(wxEVT_LEFT_DOWN, &MouseDrawEventsHandler::handle_motion_start,
               this);
  canvas->Bind(wxEVT_LEFT_UP, &MouseDrawEventsHandler::handle_motion_end, this);
  canvas->Bind(wxEVT_MOTION, &MouseDrawEventsHandler::handle_motion_event,
               this);
  canvas->Bind(wxEVT_MOUSE_CAPTURE_LOST,
               &MouseDrawEventsHandler::handle_capture_lost, this);

  return true;
}

wxPoint MouseDrawEventsHandler::to_image_point(const wxMouseEvent& event)
{
  // Viewport -> image (unscrolled/virtual) coordinates, so the numbers match
  // the GTK drawing-area space the rest of the port assumes.
  return mwctx->centralCanvas->CalcUnscrolledPosition(event.GetPosition());
}

void MouseDrawEventsHandler::handle_motion_start(wxMouseEvent& event)
{
  assert(mwctx != nullptr);
  assert(mwctx->centralCanvas != nullptr);

  const wxPoint p = to_image_point(event);

  ops->isOverResize = mwctx->centralCanvas->mouse_is_over_resize(p.x, p.y) &&
                      mwctx->centralCanvas->get_mouse_over_rect() != nullptr;

  if (!mwctx->centralCanvas->HasCapture()) {
    mwctx->centralCanvas->CaptureMouse();
  }

  if (ops->isOverResize) {
    handle_resize_motion_start();
  } else {
    handle_rectangle_draw_start(p.x, p.y);
  }
}

bool MouseDrawEventsHandler::handle_rectangle_draw_start(int x, int y)
{
  assert(mwctx != nullptr);

  if (mwctx->current_image == nullptr) {
    LOGT("No current image available");
    return false;
  }

  ops->dragging = true;

  mwctx->current_image->mark_as_has_records();
  ops->update_current_image_row_marking();

  auto ir = mwctx->current_irecord();

  assert(ir != nullptr);

  auto efactory = mwctx->actx->eventer->get_events_factory();

  assert(efactory != nullptr);

  if (efactory == nullptr) {
    LOGE("No factory available");
    return false;
  }

  ir->current_rect = efactory->create_image_rect_record(ir);
  ir->rects.emplace_back(ir->current_rect);

  assert(ir->current_rect != nullptr);

  if (mwctx->current_annotation_name != nullptr) {
    LOGT("Setting text to the currently selected "
         << mwctx->current_annotation_name->get_text());
    ir->current_rect->name = mwctx->current_annotation_name->get_text();
  }

  const double imageScale = ir->imageScale;

  ir->current_rect->x = toI(toD(x) / imageScale);
  ir->current_rect->y = toI(toD(y) / imageScale);

  ops->update_current_rects_list();

  if (mwctx->current_annotation_name == nullptr) {
    ops->update_annotations_list();
  }

  ops->update_statuses();

  return true;
}

void MouseDrawEventsHandler::handle_rectangle_draw_end()
{
  assert(mwctx != nullptr);
  assert(mwctx->centralCanvas != nullptr);

  if (mwctx->current_image == nullptr) {
    LOGT("No current image available");
    return;
  }

  auto ir = mwctx->current_irecord();

  if (ir == nullptr || ir->current_rect == nullptr) {
    LOGT("No current image current rectangle available");
    return;
  }

  ops->dragging = false;

  mwctx->centralCanvas->Refresh();
}

void MouseDrawEventsHandler::handle_resize_motion_start()
{
  assert(mwctx != nullptr);

  if (!ops->isOverResize) {
    LOGE("Not in the resize mode");
    return;
  }

  auto rectOver = mwctx->centralCanvas->get_mouse_over_rect();

  if (rectOver == nullptr) {
    LOGE("No mouse over rectangle was provided");
    return;
  }

  auto ir = mwctx->current_irecord();

  ir->current_rect = rectOver;

  mwctx->currentVisualRect = mwctx->find_current_image_current_visual_rect();

  ops->update_current_annotations_selection();
  ops->update_statuses();
}

void MouseDrawEventsHandler::handle_resize_motion_event(int x, int y)
{
  assert(mwctx != nullptr);

  if (!ops->isOverResize) {
    LOGE("Not in the resize mode");
    return;
  }

  update_current_resize(x, y);
}

bool MouseDrawEventsHandler::update_current_resize(int x, int y)
{
  assert(mwctx != nullptr);

  auto rectOver = mwctx->centralCanvas->get_mouse_over_rect();

  if (rectOver == nullptr) {
    LOGE("No mouse over rectangle was provided");
    return false;
  }

  auto im = mwctx->current_irecord();

  const double is = im->imageScale;

  const int tx = toI(toD(x) / is);
  const int ty = toI(toD(y) / is);

  const int origX = rectOver->x;
  const int origY = rectOver->y;

  if (mwctx->centralCanvas->is_over_up_left()) {
    rectOver->x = tx;
    rectOver->y = ty;
    rectOver->width += origX - tx;
    rectOver->height += origY - ty;
  } else if (mwctx->centralCanvas->is_over_up_right()) {
    rectOver->width = tx - rectOver->x;
    rectOver->y = ty;
    rectOver->height += origY - ty;
  } else if (mwctx->centralCanvas->is_over_down_left()) {
    rectOver->x = tx;
    rectOver->height = ty - rectOver->y;
    rectOver->width += origX - tx;
  } else if (mwctx->centralCanvas->is_over_down_right()) {
    rectOver->width = tx - rectOver->x;
    rectOver->height = ty - rectOver->y;
  }

  normalize_current_rect();

  mwctx->centralCanvas->Refresh();

  return true;
}

void MouseDrawEventsHandler::handle_motion_end(wxMouseEvent& event)
{
  assert(mwctx != nullptr);

  if (mwctx->centralCanvas->HasCapture()) {
    mwctx->centralCanvas->ReleaseMouse();
  }

  if (ops->dragging) {
    handle_rectangle_draw_end();
    return;
  }

  ops->isOverResize = false;
  ops->update_statuses();

  event.Skip();
}

void MouseDrawEventsHandler::handle_motion_event(wxMouseEvent& event)
{
  assert(mwctx != nullptr);
  assert(mwctx->centralCanvas != nullptr);

  const wxPoint p = to_image_point(event);

  update_tooltip();

  if (ops->dragging) {
    handle_rectangle_size_change(p.x, p.y);
    return;
  }

  if (ops->isOverResize) {
    handle_resize_motion_event(p.x, p.y);
    return;
  }

  if (mwctx->centralCanvas->mouse_is_over_resize(p.x, p.y)) {
    LOGT("Setting the resize cursor");
    set_resize_cursor();
  } else {
    LOGT("resetting the cursor");
    reset_cursor();
  }

  event.Skip();
}

void MouseDrawEventsHandler::handle_capture_lost(
    wxMouseCaptureLostEvent& /*event*/)
{
  ops->dragging = false;
  ops->isOverResize = false;
}

void MouseDrawEventsHandler::handle_rectangle_size_change(int x, int y)
{
  assert(mwctx != nullptr);
  assert(mwctx->centralCanvas != nullptr);

  if (!ops->dragging) {
    LOGT("No dragging");
    return;
  }

  auto ir = mwctx->current_irecord();

  if (ir == nullptr || ir->current_rect == nullptr) {
    LOGT("No current image current rectangle available");
    return;
  }

  if (!mwctx->centralCanvas->get_bitmap().IsOk()) {
    LOGE("Canvas doesn't contain an image bitmap");
    return;
  }

  const double imageScale = ir->imageScale;

  double start_x = toD(ir->current_rect->x);
  double start_y = toD(ir->current_rect->y);
  const double end_x = toD(x) / imageScale;
  const double end_y = toD(y) / imageScale;

  if (start_x < 0) {
    start_x = 0;
  }

  if (start_y < 0) {
    start_y = 0;
  }

  ir->current_rect->x = toI(std::min(start_x, end_x));
  ir->current_rect->y = toI(std::min(start_y, end_y));
  ir->current_rect->width = toI(std::abs(end_x - start_x));
  ir->current_rect->height = toI(std::abs(end_y - start_y));

  mwctx->centralCanvas->Refresh();

  normalize_current_rect();
}

void MouseDrawEventsHandler::normalize_current_rect()
{
  assert(mwctx != nullptr);
  assert(mwctx->centralCanvas != nullptr);

  auto ir = mwctx->current_irecord();

  if (ir == nullptr || ir->current_rect == nullptr) {
    LOGT("No current image current rectangle available");
    return;
  }

  const wxBitmap bitmap = mwctx->centralCanvas->get_bitmap();

  if (!bitmap.IsOk()) {
    LOGE("Canvas doesn't contain an image bitmap");
    return;
  }

  const double imageScale = ir->imageScale;
  auto* rect = ir->current_rect.get();

  if ((toD(rect->width + rect->x) * imageScale) > toD(bitmap.GetWidth())) {
    const double scaledWidth = toD(bitmap.GetWidth()) / imageScale;
    rect->width = toI(scaledWidth) - rect->x;
  }

  if ((toD(rect->height + rect->y) * imageScale) > toD(bitmap.GetHeight())) {
    const double scaledHeight = toD(bitmap.GetHeight()) / imageScale;
    rect->height = toI(scaledHeight) - rect->y;
  }

  if (rect->width < 0) {
    rect->x += rect->width;
    rect->width = 1;
  }

  if (rect->height < 0) {
    rect->y += rect->height;
    rect->height = 1;
  }
}

void MouseDrawEventsHandler::set_resize_cursor()
{
  mwctx->centralCanvas->SetCursor(wxCursor(wxCURSOR_SIZING));
}

void MouseDrawEventsHandler::reset_cursor()
{
  mwctx->centralCanvas->SetCursor(wxNullCursor);
}

void MouseDrawEventsHandler::update_tooltip()
{
  std::string textUsed;

  if (mwctx->current_annotation_name != nullptr) {
    textUsed = mwctx->current_annotation_name->get_text();
  }

  if (mwctx->currentVisualRect != nullptr) {
    assert(mwctx->currentVisualRect->get() != nullptr);
    textUsed = mwctx->currentVisualRect->get()->name;
  }

  if (textUsed == lastTooltip) {
    return;
  }

  lastTooltip = textUsed;

  if (textUsed.empty()) {
    mwctx->centralCanvas->UnsetToolTip();
  } else {
    mwctx->centralCanvas->SetToolTip(u8(textUsed));
  }
}

}  // namespace templateWx::window::event_handlers
