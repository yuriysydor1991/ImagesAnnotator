#include "src/gtkmm3/main-window/WindowEventsHandler.h"

#include <cassert>
#include <memory>

#include "src/gtkmm3/MainWindowContext.h"
#include "src/gtkmm3/gtkmm_includes.h"
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

  /**
   * Sending requests for the initial objects.
   */
  auto efactory = mwctx->actx->eventer->get_events_factory();

  auto idbRequest = efactory->create_image_dir_object_request();

  mwctx->actx->eventer->submit(idbRequest);
}

void WindowEventsHandler::subscribe_4_visual_events()
{
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

  assert(imagesListBox != nullptr);
  assert(zoomInB != nullptr);
  assert(zoomOutB != nullptr);
  assert(drawArea != nullptr);
  assert(oAnnB != nullptr);
  assert(imagesFolderB != nullptr);
  assert(miOpenImagesF != nullptr);
  assert(miOpenAnnotationsF != nullptr);
  assert(annSaveM != nullptr);
  assert(annSaveAsM != nullptr);
  assert(annClose != nullptr);
  assert(crDeleteB != nullptr);
  assert(aSearchE != nullptr);
  assert(allAnnL != nullptr);
  assert(rectEntry != nullptr);
  assert(copyNameB != nullptr);
  assert(prevButton != nullptr);
  assert(nextButton != nullptr);
  assert(dupCurretAnn != nullptr);

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

  auto* overlay = mwctx->wloader->get_main_overlay();
  auto* spinner = mwctx->wloader->get_spinner();

  assert(overlay != nullptr);
  assert(spinner != nullptr);

  overlay->add_overlay(*spinner);

  spinner->set_valign(Gtk::ALIGN_CENTER);
  spinner->set_halign(Gtk::ALIGN_CENTER);
  spinner->set_size_request(spinner_size, spinner_size);

  drawArea->set_halign(Gtk::ALIGN_CENTER);
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
  auto current = imagesBox->get_selected_row();

  if (current == nullptr) {
    if (!children.empty()) {
      select_list_box_child(imagesBox, *children.begin());
      return;
    }
    LOGT("No files are loaded");
    return;
  }

  auto it = std::find(children.begin(), children.end(), current);

  if (it == children.end()) {
    LOGD("The end of the images list reached");
    return;
  }

  if (std::next(it) == children.end()) {
    LOGD("The end of the images list reached");
    return;
  }

  select_list_box_child(imagesBox, *std::next(it));
}

void WindowEventsHandler::on_prev_file_button_click()
{
  auto* imagesBox = mwctx->wloader->get_images_list();

  assert(imagesBox != nullptr);

  auto children = imagesBox->get_children();
  auto current = imagesBox->get_selected_row();

  if (current == nullptr && !children.empty()) {
    select_list_box_child(imagesBox, *std::prev(children.end()));
    return;
  }

  auto it = std::find(children.begin(), children.end(), current);

  if (it == children.end()) {
    LOGD("The top of the images list reached");
    return;
  }

  if (it == children.begin()) {
    LOGD("The top of the images list reached");
    return;
  }

  select_list_box_child(imagesBox, *std::prev(it));
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

  listBox->select_row(*row);
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
  ir->rects.insert(ir->current_rect);

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

  return true;
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
}

void WindowEventsHandler::on_annotations_db_open_click()
{
  LOGT("Open new annotations dir");

  show_spinner();

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

  // Run this after window becomes idle (rendered)
  Glib::signal_idle().connect_once([this, newFileName]() {
    mwctx->actx->eventer->onAnnotationsDirChanged(newFileName);
  });
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

  if (row == nullptr) {
    mwctx->current_image.reset();
    LOGE("No row pointer provided");
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

  /// @todo seperate into the content loader infrastructure
  mwctx->current_image_original_pixbuf =
      Gdk::Pixbuf::create_from_file(ir->get_full_path());

  if (!mwctx->current_image_original_pixbuf) {
    LOGE("Fail to load the image " << ir->get_full_path());
    return;
  }

  assert(mwctx->wloader->get_central_scrolled_window() != nullptr);

  const auto origHeight = mwctx->current_image_original_pixbuf->get_height();
  const auto centralHeight =
      mwctx->wloader->get_central_scrolled_window()->get_allocated_height();

  ir->imageScale =
      static_cast<double>(centralHeight) / static_cast<double>(origHeight);
  ir->imageScale /= load_image_scale_helper;

  LOGT("Image new scale factor: " << ir->imageScale);

  update_image_zoom();

  update_current_rects_list();

  LOGT("New image selected: " << ir->get_full_path());

  auto event = eventsFactory->create_current_image_changed(ir);

  mwctx->actx->eventer->submit(event);
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

  if (mwctx->images_provider->get_db_path().empty()) {
    LOGD("No path given, calling for save as");
    on_menu_annotations_db_saveas_activate();
    return;
  }

  auto ef = mwctx->actx->eventer->get_events_factory();

  assert(ef != nullptr);

  auto saveE = ef->create_store_request();

  mwctx->actx->eventer->submit(saveE);
}

void WindowEventsHandler::on_menu_annotations_db_saveas_activate()
{
  LOGT("Trying to save as current project");

  auto dialog = mwctx->cwFactory->create_save_json_db_dialog(
      mwctx->wloader->get_window());

  assert(dialog != nullptr);

  const int result = dialog->run();

  if (result != Gtk::RESPONSE_OK) {
    LOGD("Dialog is closed");
    return;
  }

  LOGD("Selected json db file: " << dialog->get_filename());

  auto ef = mwctx->actx->eventer->get_events_factory();

  assert(ef != nullptr);

  auto saveE = ef->create_store_request(dialog->get_filename());

  mwctx->actx->eventer->submit(saveE);
}

void WindowEventsHandler::on_menu_annotations_project_close_activate()
{
  LOGT("Trying to close the project project");

  dragging = false;

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
}

void WindowEventsHandler::handle(
    std::shared_ptr<ImagesDirProviderChanged> event)
{
  LOGT("The images db provider have changed");

  assert(event != nullptr);
  assert(event->images_provider != nullptr);
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
  update_status_bar();
  update_current_rects_list();

  hide_spinner();
}

void WindowEventsHandler::update_status_bar()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);
  assert(mwctx->wloader->get_window_status_bar() != nullptr);

  auto stCtx = mwctx->wloader->get_window_status_bar()->get_context_id("info");

  mwctx->wloader->get_window_status_bar()->push(
      mwctx->images_provider->get_db_path(), stCtx);
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
    LOGE("No row pointer provided");
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

  LOGT("Reneming current image selected rectangle to " << redit->get_text());

  mwctx->current_image->get_image_rec()->current_rect->name = redit->get_text();

  mwctx->currentVisualRect->set_text(redit->get_text());

  update_annotations_list();
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
    LOGE("No row pointer provided");
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
  mwctx->current_image->get_image_rec()->rects.insert(dup);

  update_current_rects_list();
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

}  // namespace templateGtkmm3::window
