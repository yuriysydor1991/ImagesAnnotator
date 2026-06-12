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

#include "src/gtkmm3/main-window/event-handlers/MouseDrawEventsHandler.h"

#include <algorithm>
#include <cassert>
#include <memory>

#include "src/gtkmm3/MainWindowContext.h"
#include "src/gtkmm3/main-window/event-handlers/WindowOps.h"
#include "src/log/log.h"

namespace templateGtkmm3::window::event_handlers
{

MouseDrawEventsHandler::MouseDrawEventsHandler(std::shared_ptr<WindowOps> nops)
    : AbstractWindowEventHandler(nops)
{
}

bool MouseDrawEventsHandler::subscribe()
{
  assert(mwctx != nullptr);
  assert(mwctx->centralCanvas != nullptr);

  auto& drawArea = mwctx->centralCanvas;

  drawArea->signal_button_press_event().connect(
      sigc::mem_fun(*this, &MouseDrawEventsHandler::handle_motion_start));
  drawArea->signal_button_release_event().connect(
      sigc::mem_fun(*this, &MouseDrawEventsHandler::handle_motion_end));
  drawArea->signal_motion_notify_event().connect(
      sigc::mem_fun(*this, &MouseDrawEventsHandler::handle_motion_event));

  drawArea->set_has_tooltip(true);

  drawArea->signal_query_tooltip().connect(get_tooltip_lambda(), false);

  return true;
}

std::function<bool(const int& x, const int& y, bool keyboard_tooltip,
                   const Glib::RefPtr<Gtk::Tooltip>& tooltip)>
MouseDrawEventsHandler::get_tooltip_lambda()
{
  return [this]([[maybe_unused]] const int& x, [[maybe_unused]] const int& y,
                [[maybe_unused]] bool keyboard_tooltip,
                const Glib::RefPtr<Gtk::Tooltip>& tooltip) -> bool {
    assert(mwctx != nullptr);

    auto textUsed = mwctx->current_annotation_name != nullptr
                        ? mwctx->current_annotation_name->get_text()
                        : Glib::ustring{};

    if (mwctx->currentVisualRect != nullptr) {
      assert(mwctx->currentVisualRect->get() != nullptr);
      textUsed = mwctx->currentVisualRect->get()->name;
    }

    if (textUsed.empty()) {
      LOGT("No data available for the tooltip");
      return false;
    }

    LOGT("Setting the tooltip text: " << textUsed);

    tooltip->set_text(textUsed);

    // We handled the tooltip
    return true;
  };
}

bool MouseDrawEventsHandler::handle_rectangle_draw_start(GdkEventButton* event)
{
  assert(mwctx != nullptr);
  assert(event != nullptr);

  if (event->button != 1) {
    return true;
  }

  if (mwctx->current_image == nullptr) {
    LOGT("No current image available");
    return true;
  }

  ops->dragging = true;

  mwctx->current_image->mark_as_has_records();

  auto ir = mwctx->current_irecord();

  assert(ir != nullptr);

  auto efactory = mwctx->actx->eventer->get_events_factory();

  assert(efactory != nullptr);

  if (efactory == nullptr) {
    LOGE("No factory available");
    return true;
  }

  ir->current_rect = efactory->create_image_rect_record(ir);
  ir->rects.emplace_back(ir->current_rect);

  assert(ir->current_rect != nullptr);

  if (mwctx->current_annotation_name != nullptr) {
    LOGT("Setting text to the currently selected "
         << mwctx->current_annotation_name->get_text());
    ir->current_rect->name = mwctx->current_annotation_name->get_text();
  }

  const auto& imageScale = ir->imageScale;

  ir->current_rect->x = toI(toD(event->x) / imageScale);
  ir->current_rect->y = toI(toD(event->y) / imageScale);

  ops->update_current_rects_list();

  if (mwctx->current_annotation_name == nullptr) {
    ops->update_annotations_list();
  }

  ops->update_statuses();

  return true;
}

bool MouseDrawEventsHandler::handle_rectangle_draw_end(GdkEventButton* event)
{
  assert(mwctx != nullptr);
  assert(mwctx->centralCanvas != nullptr);
  assert(event != nullptr);

  if (mwctx->current_image == nullptr) {
    LOGT("No current image available");
    return true;
  }

  auto ir = mwctx->current_irecord();

  assert(ir != nullptr);

  if (ir == nullptr) {
    LOGT("No current image available");
    return true;
  }

  if (event->button != 1 || ir->current_rect == nullptr) {
    return true;
  }

  ops->dragging = false;

  assert(mwctx->centralCanvas != nullptr);

  mwctx->centralCanvas->queue_draw();

  return true;
}

void MouseDrawEventsHandler::set_resize_cursor()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);
  assert(mwctx->wloader->get_window() != nullptr);

  auto display = Gdk::Display::get_default();
  auto cursor = Gdk::Cursor::create(display, Gdk::FLEUR);

  mwctx->wloader->get_window()->get_window()->set_cursor(cursor);
}

void MouseDrawEventsHandler::reset_cursor()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);
  assert(mwctx->wloader->get_window() != nullptr);

  mwctx->wloader->get_window()->get_window()->set_cursor();
}

bool MouseDrawEventsHandler::handle_motion_start(GdkEventButton* event)
{
  assert(mwctx != nullptr);
  assert(event != nullptr);
  assert(mwctx->centralCanvas != nullptr);

  if (event->button != 1) {
    return true;
  }

  ops->isOverResize =
      mwctx->centralCanvas->mouse_is_over_resize(toI(event->x),
                                                 toI(event->y)) &&
      mwctx->centralCanvas->get_mouse_over_rect() != nullptr;

  if (ops->isOverResize) {
    return handle_resize_motion_start(event);
  } else {
    return handle_rectangle_draw_start(event);
  }

  return true;
}

bool MouseDrawEventsHandler::update_current_resize(GdkEventMotion* event)
{
  assert(mwctx != nullptr);
  assert(event != nullptr);

  auto rectOver = mwctx->centralCanvas->get_mouse_over_rect();

  if (rectOver == nullptr) {
    LOGE("No mouse over rectangle was provided");
    return false;
  }

  auto im = mwctx->current_irecord();

  const double& is = im->imageScale;

  const auto tx = toI(toD(event->x) / is);
  const auto ty = toI(toD(event->y) / is);

  const auto origX = rectOver->x;
  const auto origY = rectOver->y;

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

  mwctx->centralCanvas->queue_draw();

  return true;
}

bool MouseDrawEventsHandler::handle_resize_motion_start(
    [[maybe_unused]] GdkEventButton* event)
{
  assert(mwctx != nullptr);
  assert(event != nullptr);

  if (!ops->isOverResize) {
    LOGE("Not in the resize mode");
    return false;
  }

  auto rectOver = mwctx->centralCanvas->get_mouse_over_rect();

  if (rectOver == nullptr) {
    LOGE("No mouse over rectangle was provided");
    return false;
  }

  auto ir = mwctx->current_irecord();

  ir->current_rect = rectOver;

  mwctx->currentVisualRect = mwctx->find_current_image_current_visual_rect();

  ops->update_current_annotations_selection();
  ops->update_statuses();

  return true;
}

bool MouseDrawEventsHandler::handle_resize_motion_event(GdkEventMotion* event)
{
  assert(mwctx != nullptr);
  assert(event != nullptr);
  assert(mwctx->centralCanvas != nullptr);

  if (!ops->isOverResize) {
    LOGE("Not in the resize mode");
    return false;
  }

  return update_current_resize(event);
}

bool MouseDrawEventsHandler::handle_motion_end(GdkEventButton* event)
{
  assert(mwctx != nullptr);
  assert(event != nullptr);

  if (ops->dragging) {
    return handle_rectangle_draw_end(event);
  }

  ops->isOverResize = false;
  ops->update_statuses();

  return true;
}

bool MouseDrawEventsHandler::handle_motion_event(GdkEventMotion* event)
{
  assert(mwctx != nullptr);
  assert(event != nullptr);
  assert(mwctx->centralCanvas != nullptr);

  if (mwctx == nullptr) {
    LOGT("No context available");
    return true;
  }

  mwctx->centralCanvas->trigger_tooltip_query();

  if (ops->dragging) {
    return handle_rectangle_size_change(event);
  }

  if (ops->isOverResize) {
    return handle_resize_motion_event(event);
  }

  const bool currentlyIsOverResize =
      mwctx->centralCanvas->mouse_is_over_resize(toI(event->x), toI(event->y));

  if (currentlyIsOverResize) {
    LOGT("Setting the resize cursor");
    set_resize_cursor();
  } else {
    LOGT("resetting the cursor");
    reset_cursor();
  }

  return true;
}

bool MouseDrawEventsHandler::handle_rectangle_size_change(GdkEventMotion* event)
{
  assert(mwctx != nullptr);
  assert(event != nullptr);
  assert(mwctx->centralCanvas != nullptr);

  if (mwctx == nullptr) {
    LOGT("No context available");
    return true;
  }

  if (!ops->dragging) {
    LOGT("No dragging");
    return true;
  }

  auto ir = mwctx->current_irecord();

  assert(ir != nullptr);

  if (ir == nullptr) {
    LOGT("No current image available");
    return true;
  }

  if (ir->current_rect == nullptr) {
    LOGT("No current image current rectangle available");
    return true;
  }

  auto pixbuf = mwctx->centralCanvas->get_pixbuf();

  assert(pixbuf);

  const auto& imageScale = ir->imageScale;

  double start_x = toD(ir->current_rect->x);
  double start_y = toD(ir->current_rect->y);
  const double end_x = event->x / imageScale;
  const double end_y = event->y / imageScale;

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

  assert(mwctx->centralCanvas != nullptr);

  mwctx->centralCanvas->queue_draw();

  if (!pixbuf) {
    LOGE("Canvas doesn't contain image pixbuf");
    return true;
  }

  normalize_current_rect();

  return true;
}

void MouseDrawEventsHandler::normalize_current_rect()
{
  assert(mwctx != nullptr);
  assert(mwctx->centralCanvas != nullptr);

  if (mwctx == nullptr) {
    LOGT("No context available");
    return;
  }

  auto ir = mwctx->current_irecord();

  assert(ir != nullptr);

  if (ir == nullptr) {
    LOGT("No current image available");
    return;
  }

  if (ir->current_rect == nullptr) {
    LOGT("No current image current rectangle available");
    return;
  }

  auto pixbuf = mwctx->centralCanvas->get_pixbuf();

  assert(pixbuf);

  const auto& imageScale = ir->imageScale;

  if (((ir->current_rect->width + ir->current_rect->x) * imageScale) >
      pixbuf->get_width()) {
    const auto scaledWidth = toD(pixbuf->get_width()) / imageScale;
    ir->current_rect->width = toI(scaledWidth) - ir->current_rect->x;
  }

  if (((ir->current_rect->height + ir->current_rect->y) * imageScale) >
      pixbuf->get_height()) {
    const auto scaledHeight = toD(pixbuf->get_height()) / imageScale;
    ir->current_rect->height = toI(scaledHeight) - ir->current_rect->y;
  }

  if (ir->current_rect->width < 0) {
    ir->current_rect->x += ir->current_rect->width;

    ir->current_rect->width = 1;
  }

  if (ir->current_rect->height < 0) {
    ir->current_rect->y += ir->current_rect->height;
    ir->current_rect->height = 1;
  }
}

}  // namespace templateGtkmm3::window::event_handlers
