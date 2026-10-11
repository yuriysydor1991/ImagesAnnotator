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

#ifndef IMAGES_ANNOTATOR_PROJECT_QT6_EVENTHANDLERS_IMPORTEVENTSHANDLER_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_QT6_EVENTHANDLERS_IMPORTEVENTSHANDLER_CLASS_H

#include <functional>
#include <memory>
#include <string>

#include "src/annotator-events/events/IImageSizeFacilityProvider.h"
#include "src/gui/qt6/main-window/event-handlers/AbstractWindowEventHandler.h"

namespace imagesAnnotatorQt6::window::event_handlers
{

/**
 * @brief Handles the annotations import menu items (the thirteen dataset
 * layouts the ImagesAnnotator-DataImporters library reads, which are the very
 * thirteen the export side writes) - the mirror image of
 * ExportEventsHandler.
 *
 * Unlike an export, an import needs no project to be open: it fills one. What
 * it does need is a way to measure the pictures it recovers, which the
 * ImagesAnnotator-DataImporters library asks the application for, so every
 * request carries the toolkit image size reader along.
 */
class ImportEventsHandler : public AbstractWindowEventHandler
{
 public:
  explicit ImportEventsHandler(std::shared_ptr<WindowOps> nops);

  bool subscribe() override;

 protected:
  void handle_import_txt();
  void handle_import_yolo4();
  void handle_import_ultralytics_detect();
  void handle_import_ultralytics_obb();
  void handle_import_ultralytics_segment();
  void handle_import_coco();
  void handle_import_pascal_voc();
  void handle_import_createml();
  void handle_import_pytorch();
  void handle_import_dxxwjz2();
  void handle_import_dxxwjz2p();
  void handle_import_dxxwjz3();
  void handle_import_dxxwjz3p();

  /**
   * @brief The shared body of every import: the same chooser and spinner, with
   * only the chooser and the event to submit left to the caller. The chooser
   * is a directory one for every layout but the two darknetxx projects, which
   * pick their project file instead.
   *
   * @param askFolder Runs the directory chooser of that layout, or the file
   * chooser of a darknetxx project.
   * @param submitImport Hands the chosen directory (or project file) over to
   * the events factory and submits the resulting event.
   */
  void handle_folder_import(
      const std::function<bool(std::string&)>& askFolder,
      const std::function<void(const std::string&)>& submitImport);

  /**
   * @brief The image measuring service handed over with every import request.
   *
   * @return Returns a new QtImageSizeProvider instance.
   */
  static events::events::IImageSizeFacilityProviderPtr create_image_sizer();
};

}  // namespace imagesAnnotatorQt6::window::event_handlers

#endif  // IMAGES_ANNOTATOR_PROJECT_QT6_EVENTHANDLERS_IMPORTEVENTSHANDLER_CLASS_H
