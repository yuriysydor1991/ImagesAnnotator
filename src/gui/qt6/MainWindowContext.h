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

#ifndef IMAGES_ANNOTATOR_PROJECT_QT6_MAINWINDOWCONTEXT_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_QT6_MAINWINDOWCONTEXT_CLASS_H

#include <memory>

#include "src/annotator-events/events/ImagesPathsDBProvider.h"
#include "src/app/ApplicationContext.h"
#include "src/gui/qt6/ComponentTypesAliases.h"
#include "src/gui/qt6/main-window/WindowDataContext.h"
#include "src/gui/qt6/main-window/WindowEventsHandler.h"
#include "src/gui/qt6/main-window/WindowLoader.h"
#include "src/gui/qt6/qt_includes.h"

namespace imagesAnnotatorQt6::window
{
class WindowEventsHandler;
class WindowLoader;
}  // namespace imagesAnnotatorQt6::window

namespace imagesAnnotatorQt6
{

/**
 * @brief The Qt analogue of imagesAnnotatorGtkmm3::MainWindowContext: the
 * single aggregate the initer, the window loader and every event handler share.
 */
class MainWindowContext : virtual public ComponentTypesAliases,
                          public std::enable_shared_from_this<MainWindowContext>
{
 public:
  virtual ~MainWindowContext() = default;

  std::shared_ptr<window::WindowDataContext> wctx;
  std::shared_ptr<window::WindowLoader> wloader;
  std::shared_ptr<CWFactory> cwFactory;
  ImagesVisualDB imagesVDB;
  std::shared_ptr<app::ApplicationContext> actx;
  std::shared_ptr<window::WindowEventsHandler> mweHandler;

  /// @brief The working canvas is a QWidget owned by the window's scroll area,
  /// so it is kept as a raw pointer (destroyed by Qt's parent-child ownership
  /// when the window goes away).
  CentralWorkingCanvas* centralCanvas{nullptr};

  std::shared_ptr<ImagePathLabel> current_image;

  /// @brief The unscaled source image of the current selection; the canvas
  /// pixmap is produced from it at ImageRecord::imageScale on every zoom change
  /// (the Qt analogue of current_image_original_pixbuf).
  QImage current_image_original;

  ImageRectsLabelSet currentVisualRects;
  AnnotationsVisualList annotationsList;

  std::shared_ptr<events::events::ImagesPathsDBProvider> images_provider;
  ImageRectsLabelPtr currentVisualRect;

  /// @brief current selected annotation in the all annotations list box
  AllAnnotationsLabelPtr current_annotation_name;

  static std::shared_ptr<MainWindowContext> build_context();

  static bool validate_context(std::shared_ptr<MainWindowContext> nmwctx);

  virtual void clear();

  virtual std::shared_ptr<ImageRecord> current_irecord();

  virtual ImageRectsLabelPtr find_current_image_current_visual_rect();
};

}  // namespace imagesAnnotatorQt6

#endif  // IMAGES_ANNOTATOR_PROJECT_QT6_MAINWINDOWCONTEXT_CLASS_H
