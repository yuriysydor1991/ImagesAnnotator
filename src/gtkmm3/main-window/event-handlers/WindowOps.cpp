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

#include "src/gtkmm3/main-window/event-handlers/WindowOps.h"

#include <algorithm>
#include <cassert>
#include <memory>
#include <string>

#include "src/gtkmm3/MainWindowContext.h"
#include "src/gtkmm3/main-window/custom-widgets/AllAnnotationsLabel.h"
#include "src/gtkmm3/main-window/custom-widgets/ImagePathLabel.h"
#include "src/gtkmm3/main-window/custom-widgets/ImageRectsLabel.h"
#include "src/log/log.h"

namespace templateGtkmm3::window::event_handlers
{

WindowOps::WindowOps(std::shared_ptr<MainWindowContext> nmwctx)
    : mwctx{nmwctx}
{
}

void WindowOps::show_spinner()
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

void WindowOps::hide_spinner()
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

void WindowOps::select_list_box_child(Gtk::ListBox* listBox, Gtk::Widget* child)
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

void WindowOps::update_image_zoom()
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

void WindowOps::update_statuses()
{
  assert(mwctx != nullptr);
  assert(mwctx->images_provider != nullptr);

  lastChangedStatus = lastChangedStatus || (mwctx->images_provider != nullptr &&
                                            mwctx->images_provider->changed());

  const std::string cstatus = compute_title(lastChangedStatus);

  update_title(cstatus);
  update_status_bar(cstatus);
}

void WindowOps::update_title(const std::string& ntitle)
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);
  assert(mwctx->images_provider != nullptr);

  mwctx->wloader->update_window_title(ntitle);
}

std::string WindowOps::compute_title(const bool changes)
{
  assert(mwctx != nullptr);
  assert(mwctx->images_provider != nullptr);

  return mwctx->images_provider->get_db_path() +
         (changes ? changesI : std::string{});
}

void WindowOps::update_status_bar(const std::string& nstatus)
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

  const size_t imagesCount = mwctx->imagesVDB.size();

  std::string compStatus =
      nstatus + " [images: " + std::to_string(imagesCount) + "]";

  const size_t annotated = get_annotated_images_count();

  const float percs =
      static_cast<float>(annotated) / static_cast<float>(imagesCount) * 100.f;

  compStatus += " [annotated: " + std::to_string(annotated) + " - " +
                to_string_with_precision(percs, 2U) + "%]";

  compStatus +=
      " [annotations: " + std::to_string(mwctx->annotationsList.size()) + "]";

  if (mwctx->current_annotation_name != nullptr) {
    compStatus += " [current annotation name: " +
                  mwctx->current_annotation_name->get_text() + "]";
  }

  if (mwctx->current_image_original_pixbuf && mwctx->current_image != nullptr) {
    const float scalePercs =
        static_cast<float>(mwctx->current_image->get_image_rec()->imageScale) *
        100.0f;
    compStatus +=
        " [pixels,scale: " +
        std::to_string(mwctx->current_image_original_pixbuf->get_width()) +
        "x" +
        std::to_string(mwctx->current_image_original_pixbuf->get_height()) +
        " " + to_string_with_precision(scalePercs, 2U) + "%]";
  }

  mwctx->wloader->get_window_status_bar()->push(compStatus, stCtx);
}

void WindowOps::clean_list_box(Gtk::ListBox* listBox)
{
  assert(listBox != nullptr);

  auto children = listBox->get_children();

  for (auto* child : children) {
    listBox->remove(*child);
  }
}

void WindowOps::update_images_list()
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

void WindowOps::update_current_rects_list()
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

void WindowOps::update_current_annotations_selection()
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

void WindowOps::update_rect_edit_entry()
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

void WindowOps::update_annotations_list()
{
  assert(mwctx != nullptr);

  std::string current_selected;

  if (mwctx->current_annotation_name != nullptr) {
    current_selected = mwctx->current_annotation_name->get_text();
  }

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

  apply_annotations_filter();

  if (!current_selected.empty()) {
    select_all_annotations_name(current_selected);
  }
}

void WindowOps::apply_annotations_filter()
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

void WindowOps::show_error_dialog(const std::string& emsg)
{
  show_spinner();

  LOGE(emsg);

  auto errD =
      mwctx->cwFactory->create_error_dialog(emsg, mwctx->wloader->get_window());

  assert(errD != nullptr);

  errD->run();

  hide_spinner();
}

void WindowOps::save_project()
{
  LOGT("Trying to store current project");

  show_spinner();

  if (mwctx->images_provider->get_db_path().empty()) {
    LOGD("No path given, calling for save as");
    save_project_as();
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

void WindowOps::save_project_as()
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

bool WindowOps::ask_about_unsaved_changes()
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
    save_project();
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

bool WindowOps::ask_about_rects_delete()
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
    save_project();
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

void WindowOps::select_all_annotations_name(const std::string& name)
{
  assert(MainWindowContext::validate_context(mwctx));

  auto allA = mwctx->wloader->get_annotations_db_list();

  auto rows = allA->get_children();

  auto siter = std::find_if(rows.begin(), rows.end(),
                            get_list_row_name_searcher_op(name));

  if (siter == rows.end()) {
    LOGE("Edited annotation not found in all annotations");
    return;
  }

  select_list_box_child(allA, *siter);
}

std::function<bool(const Gtk::Widget* w)> WindowOps::get_list_row_name_searcher_op(
    const std::string& name)
{
  return [this, &name](const Gtk::Widget* w) {
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
  };
}

size_t WindowOps::get_annotated_images_count()
{
  assert(mwctx != nullptr);

  const auto& irdb = mwctx->imagesVDB;

  const auto cc =
      std::count_if(irdb.cbegin(), irdb.cend(), get_annotated_images_op());

  return static_cast<size_t>(cc);
}

std::function<bool(const WindowOps::ImagePathLabelPtr& ptr)>
WindowOps::get_annotated_images_op()
{
  return [](const ImagePathLabelPtr& ptr) {
    assert(ptr != nullptr);

    if (ptr == nullptr) {
      LOGE("Invalid visual pointer in the queue");
      return false;
    }

    const auto ir = ptr->get_image_rec();

    assert(ir != nullptr);

    return ir != nullptr && !ir->rects.empty();
  };
}

}  // namespace templateGtkmm3::window::event_handlers
