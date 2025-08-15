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

#include "src/gtkmm3/main-window/WindowEventsHandler.h"

#include <cassert>
#include <memory>

#include "src/gtkmm3/MainWindowContext.h"
#include "src/gtkmm3/gtkmm_includes.h"
#include "src/gtkmm3/helpers/GtkmmImageCropperProvider.h"
#include "src/gtkmm3/main-window/WindowDataContext.h"
#include "src/gtkmm3/main-window/custom-widgets/ImagePathLabel.h"
#include "src/log/log.h"

namespace templateGtkmm3::window
{

bool WindowEventsHandler::init(std::shared_ptr<MainWindowContext> nmwctx)
{
  assert(MainWindowContext::validate_context(nmwctx));

  if (!MainWindowContext::validate_context(nmwctx)) {
    LOGE("Invalid context pointer provided");
    return false;
  }

  iloader = ::helpers::ImageLoader::create();

  assert(iloader != nullptr);

  mwctx = nmwctx;

  subscribe_4_visual_events();
  subscribe_4_system_events();

  return true;
}

void WindowEventsHandler::subscribe_4_system_events()
{
  auto myshared = shared_from_this();
  std::shared_ptr<ImagesDirProviderChangedHandler> myptr = myshared;

  mwctx->actx->eventer->subscribe(myptr);
  mwctx->actx->eventer->subscribe(DisplayErrorEventHandlerPtr{myshared});

  /**
   * Sending requests for the initial objects.
   */
  auto efactory = mwctx->actx->eventer->get_events_factory();

  auto idbRequest = efactory->create_image_dir_object_request();

  mwctx->actx->eventer->submit(idbRequest);
}

void WindowEventsHandler::subscribe_4_visual_events()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);
  assert(mwctx->wloader->all_widget_are_valid());

  auto* imagesListBox = mwctx->wloader->get_images_list();
  auto* zoomInB = mwctx->wloader->get_current_image_zoom_in();
  auto* zoomOutB = mwctx->wloader->get_current_image_zoom_out();
  auto& drawArea = mwctx->centralCanvas;
  auto* oAnnB = mwctx->wloader->get_annotations_db_open_button();
  auto* imagesFolderB = mwctx->wloader->get_images_folder_open_button();
  auto* miOpenImagesF = mwctx->wloader->get_images_open_mi();
  auto* miOpenAnnotationsF = mwctx->wloader->get_annotations_db_open_mi();
  auto* annSaveM = mwctx->wloader->get_annotations_db_save_mi();
  auto* annSaveAsM = mwctx->wloader->get_annotations_db_saveas_mi();
  auto* annClose = mwctx->wloader->get_annotations_project_close_mi();
  auto* cImageA = mwctx->wloader->get_current_image_annotations();
  auto* crDeleteB =
      mwctx->wloader->get_delete_current_image_selected_annotation();
  auto* aSearchE = mwctx->wloader->get_annotation_search_entry();
  auto* allAnnL = mwctx->wloader->get_annotations_db_list();
  auto* rectEntry = mwctx->wloader->get_edit_current_rect_entry();
  auto* copyNameB = mwctx->wloader->get_copy_annotation_name_button();
  auto* prevButton = mwctx->wloader->get_prev_file_button();
  auto* nextButton = mwctx->wloader->get_next_file_button();
  auto* dupCurretAnn = mwctx->wloader->get_copy_current_annotation_button();
  auto* aboutM = mwctx->wloader->get_about_mi();
  auto* exportTxt2FolderM = mwctx->wloader->get_export_txt2_folder_mi();
  auto* window = mwctx->wloader->get_window();
  auto* exportYolo42FolderM = mwctx->wloader->get_export_yolo4_folder_mi();
  auto* export2PyTorchVM = mwctx->wloader->get_export_pytorchvision_folder_mi();
  auto* imagesSearchE = mwctx->wloader->get_images_search_entry();
  auto* deleteImageB = mwctx->wloader->get_delete_image_record();
  auto* webLoadM = mwctx->wloader->get_images_web_page_open_menu_item_mi();

  imagesListBox->signal_row_selected().connect(
      sigc::mem_fun(*this, &WindowEventsHandler::on_images_row_selected));
  zoomInB->signal_clicked().connect(
      sigc::mem_fun(*this, &WindowEventsHandler::on_zoom_in_clicked));
  zoomOutB->signal_clicked().connect(
      sigc::mem_fun(*this, &WindowEventsHandler::on_zoom_out_clicked));

  drawArea->signal_button_press_event().connect(
      sigc::mem_fun(*this, &WindowEventsHandler::on_mouse_motion_start));
  drawArea->signal_button_release_event().connect(
      sigc::mem_fun(*this, &WindowEventsHandler::on_mouse_motion_end));
  drawArea->signal_motion_notify_event().connect(
      sigc::mem_fun(*this, &WindowEventsHandler::on_mouse_motion_event));

  oAnnB->signal_clicked().connect(
      sigc::mem_fun(*this, &WindowEventsHandler::on_annotations_db_open_click));
  imagesFolderB->signal_clicked().connect(
      sigc::mem_fun(*this, &WindowEventsHandler::on_images_dir_open_click));

  miOpenImagesF->signal_activate().connect(sigc::mem_fun(
      *this, &WindowEventsHandler::on_menu_images_folder_open_activate));
  miOpenAnnotationsF->signal_activate().connect(sigc::mem_fun(
      *this, &WindowEventsHandler::on_menu_annotations_db_open_activate));
  annSaveM->signal_activate().connect(sigc::mem_fun(
      *this, &WindowEventsHandler::on_menu_annotations_db_save_activate));
  annSaveAsM->signal_activate().connect(sigc::mem_fun(
      *this, &WindowEventsHandler::on_menu_annotations_db_saveas_activate));
  annClose->signal_activate().connect(sigc::mem_fun(
      *this, &WindowEventsHandler::on_menu_annotations_project_close_activate));
  cImageA->signal_row_selected().connect(sigc::mem_fun(
      *this, &WindowEventsHandler::on_current_image_rect_row_selected));

  crDeleteB->signal_clicked().connect(sigc::mem_fun(
      *this, &WindowEventsHandler::on_current_rectangle_delete_click));

  rectEntry->signal_changed().connect(
      sigc::mem_fun(*this, &WindowEventsHandler::on_rect_edit_entry_changed));

  allAnnL->signal_row_selected().connect(
      sigc::mem_fun(*this, &WindowEventsHandler::on_all_annotations_selected));

  copyNameB->signal_clicked().connect(sigc::mem_fun(
      *this, &WindowEventsHandler::on_annotations_name_copy_click));

  prevButton->signal_clicked().connect(
      sigc::mem_fun(*this, &WindowEventsHandler::on_prev_file_button_click));
  nextButton->signal_clicked().connect(
      sigc::mem_fun(*this, &WindowEventsHandler::on_next_file_button_click));

  dupCurretAnn->signal_clicked().connect(
      sigc::mem_fun(*this, &WindowEventsHandler::on_ci_annotation_copy_click));

  aSearchE->signal_search_changed().connect(sigc::mem_fun(
      *this, &WindowEventsHandler::on_annotations_search_text_changed));

  aboutM->signal_activate().connect(
      sigc::mem_fun(*this, &WindowEventsHandler::on_menu_about_activate));

  exportTxt2FolderM->signal_activate().connect(sigc::mem_fun(
      *this, &WindowEventsHandler::on_export_txt_2_folder_activate));

  window->signal_delete_event().connect(
      sigc::mem_fun(*this, &WindowEventsHandler::on_window_close), false);

  exportYolo42FolderM->signal_activate().connect(sigc::mem_fun(
      *this, &WindowEventsHandler::on_export_yolo4_folder_activate));

  export2PyTorchVM->signal_activate().connect(sigc::mem_fun(
      *this, &WindowEventsHandler::on_export_pytorchvision_folder_activate));

  imagesSearchE->signal_search_changed().connect(sigc::mem_fun(
      *this, &WindowEventsHandler::on_images_search_text_changed));

  deleteImageB->signal_clicked().connect(sigc::mem_fun(
      *this, &WindowEventsHandler::on_current_image_delete_click));

  webLoadM->signal_activate().connect(sigc::mem_fun(
      *this, &WindowEventsHandler::on_menu_web_page_images_load_activate));

  mwctx->wloader->get_window()->add_events(Gdk::KEY_PRESS_MASK);

  mwctx->wloader->get_window()->signal_key_press_event().connect(
      sigc::mem_fun(*this, &WindowEventsHandler::on_key_press), false);
}

bool WindowEventsHandler::on_key_press(GdkEventKey* event)
{
  if (event->keyval == GDK_KEY_Up) {
    LOGD("User pressed up key");
    mwctx->wloader->get_prev_file_button()->clicked();
    return true;  // handled
  }

  if (event->keyval == GDK_KEY_Down) {
    LOGD("User pressed down key");
    mwctx->wloader->get_next_file_button()->clicked();
    return true;  // handled
  }

  return false;  // let other handlers process it
}

void WindowEventsHandler::show_spinner()
{
  LOGT("Starting spinner");

  assert(mwctx->wloader->get_spinner() != nullptr);
  assert(mwctx->wloader->get_main_overlay() != nullptr);

  auto* overlay = mwctx->wloader->get_main_overlay();
  auto* spinner = mwctx->wloader->get_spinner();

  overlay->get_style_context()->add_class(overlay_class);

  spinner->start();
  spinner->show();
}

void WindowEventsHandler::hide_spinner()
{
  LOGT("Stopping spinner");

  assert(mwctx->wloader->get_spinner() != nullptr);
  assert(mwctx->wloader->get_main_overlay() != nullptr);

  auto* spinner = mwctx->wloader->get_spinner();
  auto* overlay = mwctx->wloader->get_main_overlay();

  spinner->stop();
  spinner->hide();

  overlay->get_style_context()->remove_class(overlay_class);
}

void WindowEventsHandler::on_next_file_button_click()
{
  auto* imagesBox = mwctx->wloader->get_images_list();

  assert(imagesBox != nullptr);

  auto children = imagesBox->get_children();
  auto* current = imagesBox->get_selected_row();

  if (current == nullptr && !children.empty()) {
    auto tmprit = std::find_if(children.begin(), children.end(),
                               get_wvisibility_unary_op());
    if (tmprit != children.end()) {
      select_list_box_child(imagesBox, *tmprit);
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

  select_list_box_child(imagesBox, *tmpit);

  update_rect_edit_entry();
}

void WindowEventsHandler::on_prev_file_button_click()
{
  auto* imagesBox = mwctx->wloader->get_images_list();

  assert(imagesBox != nullptr);

  auto children = imagesBox->get_children();
  auto current = imagesBox->get_selected_row();

  if (current == nullptr && !children.empty()) {
    auto tmprit = std::find_if(children.rbegin(), children.rend(),
                               get_wvisibility_unary_op());
    if (tmprit != children.rend()) {
      select_list_box_child(imagesBox, *tmprit);
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

  select_list_box_child(imagesBox, *tmprit);

  update_rect_edit_entry();
}

std::function<bool(const Gtk::Widget*)>
WindowEventsHandler::get_wvisibility_unary_op()
{
  return [](const Gtk::Widget* wptr) -> bool {
    assert(wptr != nullptr);
    return wptr->is_visible();
  };
}

void WindowEventsHandler::select_list_box_child(Gtk::ListBox* listBox,
                                                Gtk::Widget* child)
{
  assert(listBox != nullptr);

  if (listBox == nullptr) {
    LOGD("List box child pointer was not provided");
    return;
  }

  auto* row = dynamic_cast<Gtk::ListBoxRow*>(child);

  if (row == nullptr) {
    LOGE("Unexpected row type");
    return;
  }

  try {
    listBox->select_row(*row);
  }
  catch (const std::exception& e) {
    LOGE("Failure during list box row selection: " << e.what());
  }
}

bool WindowEventsHandler::on_rectangle_draw_start(GdkEventButton* event)
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

  dragging = true;

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

  update_current_rects_list();

  if (mwctx->current_annotation_name == nullptr) {
    update_annotations_list();
  }

  update_statuses();

  return true;
}

bool WindowEventsHandler::on_rectangle_draw_end(GdkEventButton* event)
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

  dragging = false;

  assert(mwctx->centralCanvas != nullptr);

  mwctx->centralCanvas->queue_draw();

  return true;
}

void WindowEventsHandler::set_resize_cursor()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);
  assert(mwctx->wloader->get_window() != nullptr);

  auto display = Gdk::Display::get_default();
  auto cursor = Gdk::Cursor::create(display, Gdk::FLEUR);

  mwctx->wloader->get_window()->get_window()->set_cursor(cursor);
}

void WindowEventsHandler::reset_cursor()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);
  assert(mwctx->wloader->get_window() != nullptr);

  mwctx->wloader->get_window()->get_window()->set_cursor();
}

bool WindowEventsHandler::on_mouse_motion_start(GdkEventButton* event)
{
  assert(mwctx != nullptr);
  assert(event != nullptr);
  assert(mwctx->centralCanvas != nullptr);

  if (event->button != 1) {
    return true;
  }

  isOverResize = mwctx->centralCanvas->mouse_is_over_resize(toI(event->x),
                                                            toI(event->y)) &&
                 mwctx->centralCanvas->get_mouse_over_rect() != nullptr;

  if (isOverResize) {
    return on_mouse_resize_motion_start(event);
  } else {
    return on_rectangle_draw_start(event);
  }

  return true;
}

bool WindowEventsHandler::update_current_resize(GdkEventMotion* event)
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

bool WindowEventsHandler::on_mouse_resize_motion_start(
    [[maybe_unused]] GdkEventButton* event)
{
  assert(mwctx != nullptr);
  assert(event != nullptr);

  if (!isOverResize) {
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

  update_current_annotations_selection();
  update_statuses();

  return true;
}

bool WindowEventsHandler::on_mouse_resize_motion_event(GdkEventMotion* event)
{
  assert(mwctx != nullptr);
  assert(event != nullptr);
  assert(mwctx->centralCanvas != nullptr);

  if (!isOverResize) {
    LOGE("Not in the resize mode");
    return false;
  }

  return update_current_resize(event);
}

bool WindowEventsHandler::on_mouse_motion_end(GdkEventButton* event)
{
  assert(mwctx != nullptr);
  assert(event != nullptr);

  if (dragging) {
    return on_rectangle_draw_end(event);
  }

  isOverResize = false;
  update_statuses();

  return true;
}

bool WindowEventsHandler::on_mouse_motion_event(GdkEventMotion* event)
{
  assert(mwctx != nullptr);
  assert(event != nullptr);
  assert(mwctx->centralCanvas != nullptr);

  if (mwctx == nullptr) {
    LOGT("No context available");
    return true;
  }

  if (dragging) {
    return on_rectangle_size_change(event);
  }

  if (isOverResize) {
    return on_mouse_resize_motion_event(event);
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

bool WindowEventsHandler::on_rectangle_size_change(GdkEventMotion* event)
{
  assert(mwctx != nullptr);
  assert(event != nullptr);
  assert(mwctx->centralCanvas != nullptr);

  if (mwctx == nullptr) {
    LOGT("No context available");
    return true;
  }

  if (!dragging) {
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

void WindowEventsHandler::normalize_current_rect()
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

void WindowEventsHandler::on_zoom_in_clicked()
{
  assert(MainWindowContext::validate_context(mwctx));

  if (!MainWindowContext::validate_context(mwctx)) {
    LOGE("Invalid context pointer provided");
    return;
  }

  if (mwctx->current_image == nullptr) {
    LOGT("No image available");
    return;
  }

  assert(mwctx->current_irecord() != nullptr);

  mwctx->current_irecord()->scaleStepIn();

  update_image_zoom();
  update_statuses();
}

void WindowEventsHandler::on_zoom_out_clicked()
{
  assert(MainWindowContext::validate_context(mwctx));

  if (!MainWindowContext::validate_context(mwctx)) {
    LOGE("Invalid context pointer provided");
    return;
  }

  if (mwctx->current_image == nullptr) {
    LOGT("No image available");
    return;
  }

  assert(mwctx->current_irecord() != nullptr);

  mwctx->current_irecord()->scaleStepOut();

  update_image_zoom();
  update_statuses();
}

void WindowEventsHandler::on_annotations_db_open_click()
{
  LOGT("Open new annotations dir");

  show_spinner();

  if (lastChangedStatus) {
    if (!ask_about_unsaved_changes()) {
      LOGD("User interupted data changes discard");
      return;
    }
  }

  auto dialog =
      mwctx->cwFactory->create_json_db_dialog(mwctx->wloader->get_window());

  const int result = dialog->run();

  if (result != Gtk::RESPONSE_OK) {
    LOGD("Dialog is closed");
    hide_spinner();
    return;
  }

  const auto newFileName = dialog->get_filename();

  LOGD("Selected json db file: " << newFileName);

  mwctx->actx->eventer->onAnnotationsDirChanged(newFileName);

  update_statuses();
}

void WindowEventsHandler::update_image_zoom()
{
  assert(MainWindowContext::validate_context(mwctx));

  if (!MainWindowContext::validate_context(mwctx)) {
    LOGE("Invalid context pointer provided");
    return;
  }

  if (mwctx->current_irecord() == nullptr) {
    LOGT("No current image record available");
    return;
  }

  assert(mwctx->current_irecord() != nullptr);

  const auto currentScale = mwctx->current_irecord()->imageScale;

  LOGD("Current image zoom factor: " << currentScale);

  auto pb = mwctx->current_image_original_pixbuf;

  const int width = ceilInt(toD(pb->get_width()) * currentScale);
  const int height = ceilInt(toD(pb->get_height()) * currentScale);

  auto scaled = pb->scale_simple(width, height, Gdk::INTERP_BILINEAR);

  assert(mwctx->centralCanvas != nullptr);

  if (mwctx->centralCanvas == nullptr) {
    LOGE("No central canvas available");
    return;
  }

  mwctx->centralCanvas->set_pixbuf(scaled);
}

bool WindowEventsHandler::on_image_scroll(GdkEventScroll* scroll_event)
{
  assert(scroll_event != nullptr);

  if (scroll_event == nullptr) {
    LOGE("No valid event pointer provided");
    return false;
  }

  if (!(scroll_event->state & GDK_SHIFT_MASK)) {
    return false;
  }

  if (scroll_event->direction == GDK_SCROLL_UP) {
    on_zoom_in_clicked();
  } else if (scroll_event->direction == GDK_SCROLL_DOWN) {
    on_zoom_out_clicked();
  } else {
    LOGE("Unknown scroll event type");
    return false;
  }

  return true;
}

void WindowEventsHandler::on_images_row_selected(Gtk::ListBoxRow* row)
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

  update_image_zoom();
  update_current_rects_list();

  LOGT("New image selected: " << ir->get_full_path());

  auto event = eventsFactory->create_current_image_changed(ir);

  mwctx->actx->eventer->submit(event);
}

void WindowEventsHandler::normilize_initial_image_load_scale()
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

  update_statuses();
}

bool WindowEventsHandler::load_image(ImageRecordPtr ir)
{
  assert(MainWindowContext::validate_context(mwctx));
  assert(ir != nullptr);
  assert(iloader != nullptr);

  if (ir == nullptr) {
    const std::string emsg = "Invalid image record pointer provided";
    LOGE(emsg);
    show_error_dialog(emsg);
    return false;
  }

  show_spinner();

  if (!iloader->load(ir)) {
    const std::string emsg =
        "Failure to load the image from the network " + ir->get_full_path();
    LOGE(emsg);
    show_error_dialog(emsg);
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
    show_error_dialog(emsg);
    return false;
  }

  if (!mwctx->current_image_original_pixbuf) {
    const std::string emsg = "Fail to load the image:\n\n" + filepath +
                             (errorIfAny.empty() ? "" : "\n\n" + errorIfAny);
    LOGE(emsg);
    show_error_dialog(emsg);
    return false;
  }

  LOGD("Image seems to be loaded " << filepath);
  normilize_initial_image_load_scale();

  hide_spinner();

  return true;
}

void WindowEventsHandler::show_error_dialog(const std::string& emsg)
{
  show_spinner();

  LOGE(emsg);

  auto errD =
      mwctx->cwFactory->create_error_dialog(emsg, mwctx->wloader->get_window());

  assert(errD != nullptr);

  errD->run();

  hide_spinner();
}

void WindowEventsHandler::on_images_dir_open_click()
{
  LOGT("Open new images dir");

  show_spinner();

  auto dialog = mwctx->cwFactory->create_folder_choose_dialog(
      mwctx->wloader->get_window());

  const int result = dialog->run();

  if (result != Gtk::RESPONSE_OK) {
    LOGD("Dialog is closed");
    hide_spinner();
    return;
  }

  LOGD("Selected new images directory: " << dialog->get_filename());

  mwctx->actx->eventer->onImagesDirChanged(dialog->get_filename());

  update_statuses();
}

void WindowEventsHandler::on_menu_images_folder_open_activate()
{
  on_images_dir_open_click();
}

void WindowEventsHandler::on_menu_annotations_db_open_activate()
{
  on_annotations_db_open_click();
}

void WindowEventsHandler::on_menu_annotations_db_save_activate()
{
  LOGT("Trying to store current project");

  show_spinner();

  if (mwctx->images_provider->get_db_path().empty()) {
    LOGD("No path given, calling for save as");
    on_menu_annotations_db_saveas_activate();
    hide_spinner();
    return;
  }

  auto ef = mwctx->actx->eventer->get_events_factory();

  assert(ef != nullptr);

  auto saveE = ef->create_store_request();

  mwctx->actx->eventer->submit(saveE);

  lastChangedStatus = false;

  update_statuses();

  hide_spinner();
}

void WindowEventsHandler::on_menu_annotations_db_saveas_activate()
{
  LOGT("Trying to save as current project");

  show_spinner();

  auto dialog = mwctx->cwFactory->create_save_json_db_dialog(
      mwctx->wloader->get_window());

  assert(dialog != nullptr);

  const int result = dialog->run();

  if (result != Gtk::RESPONSE_OK) {
    LOGD("Dialog is closed");
    hide_spinner();
    return;
  }

  LOGD("Selected json db file: " << dialog->get_filename());

  auto ef = mwctx->actx->eventer->get_events_factory();

  assert(ef != nullptr);

  auto saveE = ef->create_store_request(dialog->get_filename());

  mwctx->actx->eventer->submit(saveE);

  lastChangedStatus = false;

  update_statuses();

  hide_spinner();
}

bool WindowEventsHandler::ask_about_unsaved_changes()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);
  assert(mwctx->wloader->get_window() != nullptr);
  assert(mwctx->cwFactory != nullptr);

  show_spinner();

  auto dialog = mwctx->cwFactory->create_save_changes_ask_user(
      mwctx->wloader->get_window());

  assert(dialog != nullptr);

  int result = dialog->run();

  if (result == Gtk::RESPONSE_YES) {
    LOGT("user choose to save data");
    on_menu_annotations_db_save_activate();
    hide_spinner();
    return true;
  } else if (result == Gtk::RESPONSE_NO) {
    LOGI("user discards the changes");
    hide_spinner();
    return true;
  } else if (result == Gtk::RESPONSE_CANCEL) {
    LOGT("user cancels project close");
    hide_spinner();
    return false;
  } else {
    LOGE("Unknown dialog response, saving data");
  }

  hide_spinner();

  return false;
}

bool WindowEventsHandler::ask_about_rects_delete()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);
  assert(mwctx->wloader->get_window() != nullptr);
  assert(mwctx->cwFactory != nullptr);

  show_spinner();

  auto dialog = mwctx->cwFactory->create_ask_user_about_lost_rects(
      mwctx->wloader->get_window());

  assert(dialog != nullptr);

  int result = dialog->run();

  if (result == Gtk::RESPONSE_YES) {
    LOGT("user choose delete annotations with an image");
    on_menu_annotations_db_save_activate();
    hide_spinner();
    return true;
  } else if (result == Gtk::RESPONSE_NO) {
    LOGI("user discards the annotations with the image");
    hide_spinner();
    return false;
  } else if (result == Gtk::RESPONSE_CANCEL) {
    LOGT("user cancels image record delete");
    hide_spinner();
    return false;
  } else {
    LOGE("Unknown dialog response");
  }

  hide_spinner();

  return false;
}

void WindowEventsHandler::on_menu_annotations_project_close_activate()
{
  LOGT("Trying to close the project project");

  if (lastChangedStatus) {
    if (!ask_about_unsaved_changes()) {
      LOGT("User doesn't want to close project anymore");
      return;
    }
  }

  dragging = false;
  lastChangedStatus = false;

  auto ef = mwctx->actx->eventer->get_events_factory();

  auto pcloseE = ef->create_close_event();

  mwctx->actx->eventer->submit(pcloseE);

  clean_list_box(mwctx->wloader->get_images_list());
  clean_list_box(mwctx->wloader->get_annotations_db_list());
  clean_list_box(mwctx->wloader->get_current_image_annotations());

  mwctx->wloader->get_annotation_search_entry()->set_text("");
  mwctx->wloader->get_edit_current_rect_entry()->set_text("");

  mwctx->annotationsList.clear();
  mwctx->currentVisualRects.clear();
  mwctx->imagesVDB.clear();
  mwctx->centralCanvas->clear();
  mwctx->annotationsList.clear();
  mwctx->current_annotation_name.reset();

  update_statuses();
}

void WindowEventsHandler::handle(
    std::shared_ptr<ImagesDirProviderChanged> event)
{
  LOGT("The images db provider have changed");

  assert(event != nullptr);
  assert(event->images_provider != nullptr);
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);
  assert(mwctx->wloader->get_images_list() != nullptr);
  assert(mwctx->cwFactory != nullptr);

  if (event == nullptr || event->images_provider == nullptr) {
    LOGE("No valid event object provided");
    return;
  }

  mwctx->images_provider = event->images_provider;

  update_images_list();
  update_annotations_list();
  update_current_rects_list();
  update_statuses();

  hide_spinner();
}

void WindowEventsHandler::update_statuses()
{
  assert(mwctx != nullptr);
  assert(mwctx->images_provider != nullptr);

  lastChangedStatus = lastChangedStatus || (mwctx->images_provider != nullptr &&
                                            mwctx->images_provider->changed());

  const std::string cstatus = compute_title(lastChangedStatus);

  update_title(cstatus);
  update_status_bar(cstatus);
}

void WindowEventsHandler::update_title(const std::string& ntitle)
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);
  assert(mwctx->images_provider != nullptr);

  mwctx->wloader->update_window_title(ntitle);
}

std::string WindowEventsHandler::compute_title(const bool changes)
{
  assert(mwctx != nullptr);
  assert(mwctx->images_provider != nullptr);

  return mwctx->images_provider->get_db_path() +
         (changes ? changesI : std::string{});
}

void WindowEventsHandler::update_status_bar(const std::string& nstatus)
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);
  assert(mwctx->wloader->get_window_status_bar() != nullptr);

  auto stCtx = mwctx->wloader->get_window_status_bar()->get_context_id("info");

  if (nstatus.size() <= 1 && mwctx->imagesVDB.empty()) {
    LOGD("Empty status, no project opened");
    mwctx->wloader->get_window_status_bar()->push(nstatus, stCtx);
    return;
  }

  std::string compStatus =
      nstatus + " [images: " + std::to_string(mwctx->imagesVDB.size()) + "]";

  compStatus +=
      " [annotations: " + std::to_string(mwctx->annotationsList.size()) + "]";

  if (mwctx->current_annotation_name != nullptr) {
    compStatus += " [current annotation name: " +
                  mwctx->current_annotation_name->get_text() + "]";
  }

  mwctx->wloader->get_window_status_bar()->push(compStatus, stCtx);
}

void WindowEventsHandler::clean_list_box(Gtk::ListBox* listBox)
{
  assert(listBox != nullptr);

  auto children = listBox->get_children();

  for (auto* child : children) {
    listBox->remove(*child);
  }
}

void WindowEventsHandler::update_images_list()
{
  assert(mwctx != nullptr);
  assert(mwctx->images_provider != nullptr);

  auto& imagesDB = mwctx->images_provider->get_images_db();

  LOGT("New images count: " << imagesDB.size());

  mwctx->imagesVDB = mwctx->cwFactory->create_images_visual_db(imagesDB);

  auto& imagesListView = *mwctx->wloader->get_images_list();

  clean_list_box(&imagesListView);

  LOGT("Created images widgets count: " << mwctx->imagesVDB.size());

  for (auto& r : mwctx->imagesVDB) {
    imagesListView.append(*r);
  }

  imagesListView.show_all_children();
}

void WindowEventsHandler::update_current_rects_list()
{
  LOGT("Updating rects list");

  assert(mwctx != nullptr);

  auto* ciRectsList = mwctx->wloader->get_current_image_annotations();

  assert(ciRectsList != nullptr);

  clean_list_box(ciRectsList);

  if (mwctx->current_image == nullptr) {
    LOGT("No current image selected");
    return;
  }

  auto& rectsList = mwctx->current_image->get_image_rec()->rects;

  auto labelsList = mwctx->cwFactory->create_rects_labels(rectsList);

  mwctx->currentVisualRects = labelsList;

  for (auto& r : labelsList) {
    LOGT("Inserting the rect: " << r->get_text());
    ciRectsList->append(*r);
  }

  ciRectsList->show_all_children();

  mwctx->centralCanvas->queue_draw();

  mwctx->find_current_image_current_visual_rect();

  update_current_annotations_selection();
}

void WindowEventsHandler::update_current_annotations_selection()
{
  auto* ciRectsList = mwctx->wloader->get_current_image_annotations();

  assert(ciRectsList != nullptr);

  if (ciRectsList == nullptr) {
    LOGE("Current image rects list box doesn't exists");
    return;
  }

  auto children = ciRectsList->get_children();

  for (Gtk::Widget* child : children) {
    auto row = dynamic_cast<Gtk::ListBoxRow*>(child);

    if (row == nullptr) continue;

    Gtk::Widget* row_child = row->get_child();

    if (row_child == nullptr) continue;

    auto* label = dynamic_cast<ImageRectsLabel*>(row_child);

    if (label == nullptr) {
      LOGE("Unexpected list box row type for current image annotations box");
      continue;
    }

    if (label == mwctx->currentVisualRect.get()) {
      LOGT("Selecting the row");
      ciRectsList->select_row(*row);
      return;
    }
  }

  LOGD("ERROR no current rect found");
}

void WindowEventsHandler::on_current_image_rect_row_selected(
    Gtk::ListBoxRow* row)
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

  update_rect_edit_entry();

  LOGT("Current image rect changed: " << rectLabel->get_text());

  mwctx->centralCanvas->queue_draw();
}

void WindowEventsHandler::update_rect_edit_entry()
{
  assert(mwctx != nullptr);

  if (mwctx->current_image == nullptr) {
    LOGE("No current image selected");
    return;
  }

  if (mwctx->current_image->get_image_rec() == nullptr) {
    LOGE("Current image label contains no image record");
    return;
  }

  auto* rentry = mwctx->wloader->get_edit_current_rect_entry();

  assert(rentry != nullptr);

  if (mwctx->current_image->get_image_rec()->current_rect == nullptr) {
    LOGT("No current rect selected, cleaning entry");
    rentry->set_text("");
    return;
  }

  rentry->set_text(mwctx->current_image->get_image_rec()->current_rect->name);
}

void WindowEventsHandler::on_rect_edit_entry_changed()
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

  LOGT("Reneming current image selected rectangle to " << nname);

  mwctx->current_image->get_image_rec()->current_rect->name = nname;
  mwctx->currentVisualRect->set_text(nname);

  update_annotations_list();
  select_all_annotations_name(nname);
  update_statuses();
}

void WindowEventsHandler::on_current_rectangle_delete_click()
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

  update_current_rects_list();
  update_annotations_list();
  update_rect_edit_entry();
  update_statuses();

  if (mwctx->current_image->get_image_rec()->rects.empty()) {
    LOGT("Current images no longer have rects, deleting the marking");
    mwctx->current_image->remove_has_records_mark();
  }

  LOGT("Current rect erased");
}

void WindowEventsHandler::update_annotations_list()
{
  assert(mwctx != nullptr);

  auto* aListBox = mwctx->wloader->get_annotations_db_list();

  assert(aListBox != nullptr);

  auto alist = mwctx->images_provider->get_available_annotations();

  clean_list_box(aListBox);

  mwctx->annotationsList = mwctx->cwFactory->create_annotations_labels(alist);

  alist.clear();

  for (auto& a : mwctx->annotationsList) {
    aListBox->append(*a);
  }

  aListBox->show_all_children();

  on_annotations_search_text_changed();
}

void WindowEventsHandler::on_all_annotations_selected(Gtk::ListBoxRow* row)
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

  update_statuses();
}

void WindowEventsHandler::on_annotations_name_copy_click()
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

  update_rect_edit_entry();
  update_statuses();
}

void WindowEventsHandler::on_ci_annotation_copy_click()
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

  update_current_rects_list();
  update_statuses();
}

void WindowEventsHandler::on_annotations_search_text_changed()
{
  assert(MainWindowContext::validate_context(mwctx));

  if (!MainWindowContext::validate_context(mwctx)) {
    LOGE("Invalid context pointer provided");
    return;
  }

  auto* aListBox = mwctx->wloader->get_annotations_db_list();

  auto searchText = mwctx->wloader->get_annotation_search_entry()->get_text();

  const auto filter_text = searchText.lowercase();

  for (auto* child : aListBox->get_children()) {
    auto* row = dynamic_cast<Gtk::ListBoxRow*>(child);

    if (row == nullptr) {
      LOGT("Unexpected row type on the Gtk::ListBox");
      continue;
    }

    auto* label = dynamic_cast<AllAnnotationsLabel*>(row->get_child());

    if (label == nullptr) {
      LOGT("Unexpected label type in the ListBox");
      continue;
    }

    Glib::ustring text = label->get_text().lowercase();

    row->set_visible(filter_text.empty() ||
                     text.find(filter_text) != Glib::ustring::npos);
  }
}

void WindowEventsHandler::on_menu_about_activate()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader->get_about() != nullptr);
  assert(mwctx->cwFactory != nullptr);

  auto about = mwctx->wloader->get_about();

  mwctx->cwFactory->prepare_about(about, mwctx->wloader->get_window());

  about->show();
}

void WindowEventsHandler::on_export_txt_2_folder_activate()
{
  assert(mwctx != nullptr);
  assert(mwctx->cwFactory != nullptr);

  if (!has_to_export()) {
    const std::string errmsg = "No images found or db opened to export!";
    show_error_dialog(errmsg);
    return;
  }

  show_spinner();

  auto dialog = mwctx->cwFactory->create_txt_export_folder_choose_dialog(
      mwctx->wloader->get_window());

  const int result = dialog->run();

  if (result != Gtk::RESPONSE_OK) {
    LOGD("Dialog is closed");
    hide_spinner();
    return;
  }

  const auto newFileName = dialog->get_filename();

  LOGD("Selected txt export folder: " << newFileName);

  auto efactory = mwctx->actx->eventer->get_events_factory();

  assert(efactory != nullptr);

  auto exportEvent = efactory->create_plain_txt_2_folder_export(newFileName);

  mwctx->actx->eventer->submit(exportEvent);

  update_statuses();

  hide_spinner();
}

bool WindowEventsHandler::has_to_export()
{
  assert(mwctx != nullptr);

  return mwctx != nullptr && (!mwctx->imagesVDB.empty() ||
                              (mwctx->images_provider != nullptr &&
                               !mwctx->images_provider->get_db_path().empty()));
}

bool WindowEventsHandler::on_window_close(GdkEventAny*)
{
  if (!lastChangedStatus) {
    LOGT("Seems like no changes made - allowing window close");
    return false;
  }

  const bool continueWClose = ask_about_unsaved_changes();

  if (continueWClose) {
    LOGT("user alowed window to close");
    return false;
  }

  LOGT("Interupting window close");
  return true;
}

void WindowEventsHandler::on_export_yolo4_folder_activate()
{
  assert(mwctx != nullptr);
  assert(mwctx->cwFactory != nullptr);

  if (!has_to_export()) {
    const std::string errmsg = "No images found or db opened to export!";
    show_error_dialog(errmsg);
    return;
  }

  show_spinner();

  auto dialog = mwctx->cwFactory->create_yolo4_export_folder_choose_dialog(
      mwctx->wloader->get_window());

  const int result = dialog->run();

  if (result != Gtk::RESPONSE_OK) {
    LOGD("Dialog is closed");
    hide_spinner();
    return;
  }

  const auto newFileName = dialog->get_filename();

  LOGD("Selected yolo4 export folder: " << newFileName);

  auto efactory = mwctx->actx->eventer->get_events_factory();

  assert(efactory != nullptr);

  auto exportEvent = efactory->create_yolo4_export_request(newFileName);

  mwctx->actx->eventer->submit(exportEvent);

  update_statuses();

  hide_spinner();
}

void WindowEventsHandler::on_export_pytorchvision_folder_activate()
{
  assert(mwctx != nullptr);
  assert(mwctx->cwFactory != nullptr);

  if (!has_to_export()) {
    const std::string errmsg = "No images found or db opened to export!";
    show_error_dialog(errmsg);
    return;
  }

  show_spinner();

  auto dialog = mwctx->cwFactory->create_pytorch_export_folder_choose_dialog(
      mwctx->wloader->get_window());

  const int result = dialog->run();

  if (result != Gtk::RESPONSE_OK) {
    LOGD("Dialog is closed");
    hide_spinner();
    return;
  }

  const auto newFileName = dialog->get_filename();

  LOGD("Selected yolo4 export folder: " << newFileName);

  auto efactory = mwctx->actx->eventer->get_events_factory();

  assert(efactory != nullptr);

  auto exportEvent =
      efactory->create_pytorch_vision_export_request(newFileName);

  assert(exportEvent != nullptr);

  exportEvent->cropper = std::make_shared<helpers::GtkmmImageCropperProvider>();

  mwctx->actx->eventer->submit(exportEvent);

  update_statuses();

  hide_spinner();
}

void WindowEventsHandler::on_images_search_text_changed()
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

void WindowEventsHandler::on_current_image_delete_click()
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
    if (!ask_about_rects_delete()) {
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

void WindowEventsHandler::on_menu_web_page_images_load_activate()
{
  assert(MainWindowContext::validate_context(mwctx));

  if (!MainWindowContext::validate_context(mwctx)) {
    LOGE("Invalid context pointer provided");
    return;
  }

  std::string webpageurl;

  if (!ask_user_4_webpage_url(webpageurl)) {
    LOGD("User closed the URL dialog");
    return;
  }

  if (webpageurl.empty()) {
    LOGE("No Web page URL retrieved");
    return;
  }

  assert(mwctx->actx->eventer != nullptr);

  auto efactory = mwctx->actx->eventer->get_events_factory();

  assert(efactory != nullptr);

  auto webURLEvent = efactory->create_images_web_page_load_event(webpageurl);

  assert(webURLEvent != nullptr);

  mwctx->actx->eventer->submit(webURLEvent);
}

bool WindowEventsHandler::ask_user_4_webpage_url(std::string& urldst)
{
  assert(MainWindowContext::validate_context(mwctx));

  if (!MainWindowContext::validate_context(mwctx)) {
    LOGE("Invalid context pointer provided");
    return false;
  }

  show_spinner();

  auto* dialog = mwctx->wloader->get_web_page_url_asker();

  assert(dialog != nullptr);

  mwctx->cwFactory->prepare_url_asker(
      dialog, mwctx->wloader->get_images_web_page_cache_folder(),
      mwctx->wloader->get_window());

  dialog->show_all();

  const int result = dialog->run();

  dialog->hide();

  if (result != Gtk::RESPONSE_OK) {
    LOGD("Dialog is closed");
    hide_spinner();
    return false;
  }

  auto urlEntry = mwctx->wloader->get_images_web_page_url_entry();

  assert(urlEntry != nullptr);

  urldst = urlEntry->get_text();

  hide_spinner();

  LOGD("User entered web page: " << urldst);

  return !urldst.empty();
}

void WindowEventsHandler::handle(DisplayErrorEventPtr event)
{
  assert(event != nullptr);

  LOGE("Handle error: " << event->description);
}

void WindowEventsHandler::select_all_annotations_name(const std::string& name)
{
  assert(MainWindowContext::validate_context(mwctx));

  auto allA = mwctx->wloader->get_annotations_db_list();

  auto rows = allA->get_children();

  auto siter = std::find_if(
      rows.begin(), rows.end(), [this, &name](const Gtk::Widget* w) {
        assert(w != nullptr);

        const auto* listRow = dynamic_cast<const Gtk::ListBoxRow*>(w);

        assert(listRow != nullptr);

        if (listRow == nullptr) {
          LOGE("Unexpected all annotations list item widget");
          return false;
        }

        const auto* label =
            dynamic_cast<const AllAnnotationsLabel*>(listRow->get_child());

        return label != nullptr && label->get_text() == name;
      });

  if (siter == rows.end()) {
    LOGE("Edited annotation not found in all annotations");
    return;
  }

  select_list_box_child(allA, *siter);
}

}  // namespace templateGtkmm3::window
