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

#include "src/gui/qt6/main-window/event-handlers/WindowOps.h"

#include <algorithm>
#include <cassert>

#include "src/annotator-events/events/ImageRecord.h"
#include "src/gui/qt6/MainWindowContext.h"
#include "src/gui/qt6/qt_str_utils.h"
#include "src/log/log.h"

namespace imagesAnnotatorQt6::window::event_handlers
{

namespace
{
// The annotated-image row background, matching the .image_row_has_records CSS
// colour (#255A1B) the GTKmm GUI paints such rows with. Qt Style Sheets cannot
// address an individual list row, so it is applied per item here - see the note
// in src/gui/qt6/ui/resources/css/main-window.qss.
const QColor hasRecordsColour{37, 90, 27};
}  // namespace

WindowOps::WindowOps(std::shared_ptr<MainWindowContext> nmwctx) : mwctx{nmwctx}
{
}

void WindowOps::show_spinner()
{
  // Qt has no spinner widget; an application-wide wait cursor is the closest
  // "working" feedback. Guarded by spinnerOn so the override cursor stack stays
  // balanced even when a handler returns early between show and hide.
  if (!spinnerOn) {
    QGuiApplication::setOverrideCursor(QCursor{Qt::WaitCursor});
    spinnerOn = true;
  }
}

void WindowOps::hide_spinner()
{
  if (spinnerOn) {
    QGuiApplication::restoreOverrideCursor();
    spinnerOn = false;
  }
}

void WindowOps::update_image_zoom()
{
  if (!MainWindowContext::validate_context(mwctx)) {
    LOGE("Invalid context pointer provided");
    return;
  }

  if (mwctx->current_irecord() == nullptr) {
    LOGT("No current image record available");
    return;
  }

  const double currentScale = mwctx->current_irecord()->imageScale;

  LOGD("Current image zoom factor: " << currentScale);

  if (mwctx->current_image_original.isNull()) {
    LOGE("No original image available");
    return;
  }

  const int width =
      ceilInt(toD(mwctx->current_image_original.width()) * currentScale);
  const int height =
      ceilInt(toD(mwctx->current_image_original.height()) * currentScale);

  if (width < 1 || height < 1) {
    LOGD("Computed scaled image size is degenerate; skipping the rescale");
    return;
  }

  const QImage scaled = mwctx->current_image_original.scaled(
      width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

  if (mwctx->centralCanvas == nullptr) {
    LOGE("No central canvas available");
    return;
  }

  mwctx->centralCanvas->set_pixmap(QPixmap::fromImage(scaled));

  update_action_buttons_sensitivity();
}

void WindowOps::update_statuses()
{
  assert(mwctx != nullptr);

  if (mwctx->images_provider == nullptr) {
    LOGT("No images provider yet, nothing to report");
    return;
  }

  lastChangedStatus = lastChangedStatus || mwctx->images_provider->changed();

  const std::string cstatus = compute_title(lastChangedStatus);

  update_title(cstatus);
  update_status_bar(cstatus);
}

void WindowOps::update_title(const std::string& ntitle)
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);

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

  auto* statusBar = mwctx->wloader->get_window_status_bar();

  assert(statusBar != nullptr);

  if (nstatus.size() <= 1 && mwctx->imagesVDB.empty()) {
    LOGD("Empty status, no project opened");
    statusBar->showMessage(u8(nstatus));
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

  if (!mwctx->current_image_original.isNull() &&
      mwctx->current_image != nullptr) {
    const float scalePercs =
        static_cast<float>(mwctx->current_image->get_image_rec()->imageScale) *
        100.0f;
    compStatus += " [pixels,scale: " +
                  std::to_string(mwctx->current_image_original.width()) + "x" +
                  std::to_string(mwctx->current_image_original.height()) + " " +
                  to_string_with_precision(scalePercs, 2U) + "%]";
  }

  statusBar->showMessage(u8(compStatus));
}

void WindowOps::update_images_list()
{
  assert(mwctx != nullptr);
  assert(mwctx->images_provider != nullptr);

  auto& imagesDB = mwctx->images_provider->get_images_db();

  LOGT("New images count: " << imagesDB.size());

  mwctx->imagesVDB = mwctx->cwFactory->create_images_visual_db(imagesDB);
  displayedImages = mwctx->imagesVDB;

  rebuild_images_view();

  update_action_buttons_sensitivity();
}

void WindowOps::mark_images_row(QListWidgetItem* item, const bool annotated)
{
  assert(item != nullptr);

  if (item == nullptr) {
    return;
  }

  if (annotated) {
    item->setBackground(QBrush{hasRecordsColour});
    item->setForeground(QBrush{Qt::white});
    return;
  }

  // An empty brush restores the view's own palette for that row.
  item->setBackground(QBrush{});
  item->setForeground(QBrush{});
}

void WindowOps::rebuild_images_view()
{
  auto* list = mwctx->wloader->get_images_list();

  assert(list != nullptr);

  // Repopulating a QListWidget emits currentRowChanged/itemSelectionChanged,
  // which would re-enter the image selection handler with a stale row. Blocking
  // the signals keeps the rebuild purely visual, the way the GTKmm code only
  // toggled row visibility.
  const QSignalBlocker blocker{list};

  list->clear();

  for (auto& r : displayedImages) {
    auto* item = new QListWidgetItem{u8(r->get_text()), list};

    mark_images_row(item, r->has_records());
  }
}

void WindowOps::apply_images_filter()
{
  const std::string filter =
      to_lower(from_qt(mwctx->wloader->get_images_search_entry()->text()));

  displayedImages.clear();

  for (auto& r : mwctx->imagesVDB) {
    if (filter.empty() ||
        to_lower(r->get_text()).find(filter) != std::string::npos) {
      displayedImages.emplace_back(r);
    }
  }

  rebuild_images_view();

  update_action_buttons_sensitivity();
}

void WindowOps::update_action_buttons_sensitivity()
{
  assert(mwctx != nullptr);
  assert(mwctx->wloader != nullptr);

  auto& wl = *mwctx->wloader;

  const bool haveImages = !mwctx->imagesVDB.empty();

  // Previous / Next: sensitive when moving would not be a no-op, i.e. there is
  // a displayed row before / after the current selection. With no selection
  // either direction jumps to the first / last displayed row.
  bool canPrev = false;
  bool canNext = false;
  if (!displayedImages.empty()) {
    const int sel = images_selected_index();
    if (sel == no_selection) {
      canPrev = true;
      canNext = true;
    } else {
      canPrev = sel > 0;
      canNext = sel < static_cast<int>(displayedImages.size()) - 1;
    }
  }

  wl.get_prev_file_button()->setEnabled(canPrev);
  wl.get_next_file_button()->setEnabled(canNext);

  // Zoom in / out: a direction is a no-op once the current image sits at
  // ImageRecord::{max,min}Scale.
  bool canZoomIn = false;
  bool canZoomOut = false;
  if (haveImages && mwctx->current_irecord() != nullptr) {
    const double scale = mwctx->current_irecord()->imageScale;
    canZoomIn = scale < events::events::ImageRecord::maxScale;
    canZoomOut = scale > events::events::ImageRecord::minScale;
  }

  wl.get_current_image_zoom_in()->setEnabled(canZoomIn);
  wl.get_current_image_zoom_out()->setEnabled(canZoomOut);

  wl.get_delete_image_record()->setEnabled(haveImages);

  const bool haveCurrentAnnotation =
      haveImages && mwctx->currentVisualRect != nullptr;

  wl.get_copy_annotation_name_button()->setEnabled(
      haveCurrentAnnotation && mwctx->current_annotation_name != nullptr);
  wl.get_copy_current_annotation_button()->setEnabled(haveCurrentAnnotation);

  wl.get_delete_current_image_selected_annotation()->setEnabled(haveImages);
}

void WindowOps::update_current_rects_list()
{
  LOGT("Updating rects list");

  assert(mwctx != nullptr);

  auto* ciRectsList = mwctx->wloader->get_current_image_annotations();

  assert(ciRectsList != nullptr);

  {
    const QSignalBlocker blocker{ciRectsList};
    ciRectsList->clear();
  }

  if (mwctx->current_image == nullptr) {
    LOGT("No current image selected");
    return;
  }

  auto& rectsList = mwctx->current_image->get_image_rec()->rects;

  mwctx->currentVisualRects = mwctx->cwFactory->create_rects_labels(rectsList);

  rebuild_current_rects_view();

  if (mwctx->centralCanvas != nullptr) {
    mwctx->centralCanvas->update();
  }

  mwctx->find_current_image_current_visual_rect();

  update_current_annotations_selection();

  update_action_buttons_sensitivity();
}

void WindowOps::rebuild_current_rects_view()
{
  auto* ciRectsList = mwctx->wloader->get_current_image_annotations();

  const QSignalBlocker blocker{ciRectsList};

  ciRectsList->clear();

  for (auto& r : mwctx->currentVisualRects) {
    LOGT("Inserting the rect: " << r->get_text());
    ciRectsList->addItem(u8(r->get_text()));
  }
}

void WindowOps::update_current_annotations_selection()
{
  auto* ciRectsList = mwctx->wloader->get_current_image_annotations();

  assert(ciRectsList != nullptr);

  if (mwctx->currentVisualRect == nullptr) {
    return;
  }

  for (size_t i = 0; i < mwctx->currentVisualRects.size(); ++i) {
    if (mwctx->currentVisualRects[i] == mwctx->currentVisualRect) {
      LOGT("Selecting the row");
      // Programmatic sync of the view with the model: blocked so it does not
      // re-enter the row-selected handler that produced this state.
      const QSignalBlocker blocker{ciRectsList};
      ciRectsList->setCurrentRow(static_cast<int>(i));
      return;
    }
  }

  LOGD("No current rect found");
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

  // Blocked so this programmatic sync does not fire textChanged and re-enter
  // the rename handler (the Qt analogue of the wx ChangeValue()).
  const QSignalBlocker blocker{rentry};

  if (mwctx->current_image->get_image_rec()->current_rect == nullptr) {
    LOGT("No current rect selected, cleaning entry");
    rentry->clear();
    return;
  }

  rentry->setText(
      u8(mwctx->current_image->get_image_rec()->current_rect->name));
}

void WindowOps::update_annotations_list()
{
  assert(mwctx != nullptr);

  std::string current_selected;

  if (mwctx->current_annotation_name != nullptr) {
    current_selected = mwctx->current_annotation_name->get_text();
  }

  auto alist = mwctx->images_provider->get_available_annotations();

  mwctx->annotationsList = mwctx->cwFactory->create_annotations_labels(alist);

  alist.clear();

  apply_annotations_filter();

  if (!current_selected.empty()) {
    select_all_annotations_name(current_selected);
  }
}

void WindowOps::apply_annotations_filter()
{
  if (!MainWindowContext::validate_context(mwctx)) {
    LOGE("Invalid context pointer provided");
    return;
  }

  const std::string filter =
      to_lower(from_qt(mwctx->wloader->get_annotation_search_entry()->text()));

  displayedAnnotations.clear();

  for (auto& a : mwctx->annotationsList) {
    if (filter.empty() ||
        to_lower(a->get_text()).find(filter) != std::string::npos) {
      displayedAnnotations.emplace_back(a);
    }
  }

  rebuild_annotations_view();
}

void WindowOps::rebuild_annotations_view()
{
  auto* aListBox = mwctx->wloader->get_annotations_db_list();

  const QSignalBlocker blocker{aListBox};

  aListBox->clear();

  for (auto& a : displayedAnnotations) {
    aListBox->addItem(u8(a->get_text()));
  }
}

void WindowOps::show_error_dialog(const std::string& emsg)
{
  // The dialog is modal, so the wait cursor is dropped for its lifetime and
  // restored afterwards - the user is being asked something, not waited on.
  const bool wasOn = spinnerOn;

  hide_spinner();

  LOGE(emsg);

  mwctx->cwFactory->show_error(mwctx->wloader->get_window(), emsg);

  if (wasOn) {
    show_spinner();
  }
}

void WindowOps::save_project()
{
  LOGT("Trying to store current project");

  if (mwctx->images_provider->get_db_path().empty()) {
    LOGD("No path given, calling for save as");
    save_project_as();
    return;
  }

  // Raised only once the destination is known: the wait cursor covers the
  // actual store, never a modal dialog waiting on the user.
  show_spinner();

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

  std::string newFileName;

  if (!mwctx->cwFactory->ask_save_json_db(mwctx->wloader->get_window(),
                                          newFileName)) {
    LOGD("Dialog is closed");
    return;
  }

  LOGD("Selected json db file: " << newFileName);

  show_spinner();

  auto ef = mwctx->actx->eventer->get_events_factory();

  assert(ef != nullptr);

  auto saveE = ef->create_store_request(newFileName);

  mwctx->actx->eventer->submit(saveE);

  lastChangedStatus = false;

  update_statuses();

  hide_spinner();
}

bool WindowOps::ask_about_unsaved_changes()
{
  assert(mwctx != nullptr);
  assert(mwctx->cwFactory != nullptr);

  const auto answer =
      mwctx->cwFactory->ask_save_changes(mwctx->wloader->get_window());

  switch (answer) {
    case CWFactory::SaveAnswer::Save:
      LOGT("user choose to save data");
      save_project();
      return true;
    case CWFactory::SaveAnswer::Discard:
      LOGI("user discards the changes");
      return true;
    case CWFactory::SaveAnswer::Cancel:
    default:
      LOGT("user cancels project close");
      return false;
  }
}

bool WindowOps::ask_about_rects_delete()
{
  assert(mwctx != nullptr);
  assert(mwctx->cwFactory != nullptr);

  const auto answer =
      mwctx->cwFactory->ask_lost_rects(mwctx->wloader->get_window());

  switch (answer) {
    case CWFactory::LostRectsAnswer::Delete:
      LOGT("user choose delete annotations with an image");
      save_project();
      return true;
    case CWFactory::LostRectsAnswer::Leave:
      LOGI("user discards the annotations with the image");
      return false;
    case CWFactory::LostRectsAnswer::Cancel:
    default:
      LOGT("user cancels image record delete");
      return false;
  }
}

void WindowOps::select_all_annotations_name(const std::string& name)
{
  auto* allA = mwctx->wloader->get_annotations_db_list();

  for (size_t i = 0; i < displayedAnnotations.size(); ++i) {
    if (displayedAnnotations[i]->get_text() == name) {
      const QSignalBlocker blocker{allA};
      allA->setCurrentRow(static_cast<int>(i));
      mwctx->current_annotation_name = displayedAnnotations[i];
      return;
    }
  }

  LOGD("Edited annotation not found in the displayed all annotations");
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

    return ir != nullptr && !ir->rects.empty();
  };
}

int WindowOps::images_selected_index()
{
  return mwctx->wloader->get_images_list()->currentRow();
}

void WindowOps::select_image_row(int row)
{
  auto* list = mwctx->wloader->get_images_list();

  if (row < 0 || row >= list->count()) {
    return;
  }

  // Deliberately NOT blocked: setCurrentRow() emits currentRowChanged, which
  // the image list handler turns into the image load (the Qt analogue of GTK's
  // select_row() firing row-selected).
  list->setCurrentRow(row);
  list->scrollToItem(list->item(row));
}

WindowOps::ImagePathLabelPtr WindowOps::images_row_label(int row)
{
  if (row < 0 || row >= static_cast<int>(displayedImages.size())) {
    return {};
  }

  return displayedImages[static_cast<size_t>(row)];
}

void WindowOps::update_current_image_row_marking()
{
  if (mwctx->current_image == nullptr) {
    return;
  }

  auto* list = mwctx->wloader->get_images_list();

  for (size_t i = 0; i < displayedImages.size(); ++i) {
    if (displayedImages[i] != mwctx->current_image) {
      continue;
    }

    mark_images_row(list->item(static_cast<int>(i)),
                    mwctx->current_image->has_records());
    return;
  }
}

void WindowOps::clear_all_lists()
{
  auto* imagesList = mwctx->wloader->get_images_list();
  auto* annotationsList = mwctx->wloader->get_annotations_db_list();
  auto* currentRects = mwctx->wloader->get_current_image_annotations();
  auto* annotationSearch = mwctx->wloader->get_annotation_search_entry();
  auto* rectEdit = mwctx->wloader->get_edit_current_rect_entry();

  {
    const QSignalBlocker imagesBlocker{imagesList};
    const QSignalBlocker annotationsBlocker{annotationsList};
    const QSignalBlocker rectsBlocker{currentRects};
    const QSignalBlocker searchBlocker{annotationSearch};
    const QSignalBlocker editBlocker{rectEdit};

    imagesList->clear();
    annotationsList->clear();
    currentRects->clear();

    annotationSearch->clear();
    rectEdit->clear();
  }

  displayedImages.clear();
  displayedAnnotations.clear();

  mwctx->annotationsList.clear();
  mwctx->currentVisualRects.clear();
  mwctx->imagesVDB.clear();
  mwctx->currentVisualRect.reset();
  mwctx->current_annotation_name.reset();
  mwctx->current_image.reset();

  if (mwctx->centralCanvas != nullptr) {
    mwctx->centralCanvas->clear();
  }
}

}  // namespace imagesAnnotatorQt6::window::event_handlers
