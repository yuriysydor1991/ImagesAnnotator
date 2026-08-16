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

#ifndef IMAGES_ANNOTATOR_PROJECT_QT6_EVENTHANDLERS_EXPORTEVENTSHANDLER_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_QT6_EVENTHANDLERS_EXPORTEVENTSHANDLER_CLASS_H

#include <functional>
#include <memory>
#include <string>

#include "src/gui/qt6/main-window/event-handlers/AbstractWindowEventHandler.h"

namespace imagesAnnotatorQt6::window::event_handlers
{

/**
 * @brief Handles the annotations export menu items (plain txt, the darknet
 * YOLO4 directory, the three Ultralytics YOLO dataset layouts and the
 * PyTorch Vision exports to a folder) - the Qt analogue of GTKmm's
 * ExportEventsHandler.
 */
class ExportEventsHandler : public AbstractWindowEventHandler
{
 public:
  explicit ExportEventsHandler(std::shared_ptr<WindowOps> nops);

  bool subscribe() override;

 protected:
  void handle_export_txt();
  void handle_export_yolo4();
  void handle_export_pytorch();
  void handle_export_ultralytics_detect();
  void handle_export_ultralytics_obb();
  void handle_export_ultralytics_segment();

  /**
   * @brief The shared body of the three Ultralytics YOLO exports: the same
   * guard, directory chooser and spinner, with only the chooser and the event
   * to submit left to the caller.
   *
   * @param askFolder Runs the directory chooser of that layout.
   * @param submitExport Hands the chosen directory over to the events factory
   * and submits the resulting event.
   */
  void handle_ultralytics_export(
      const std::function<bool(std::string&)>& askFolder,
      const std::function<void(const std::string&)>& submitExport);

  bool has_to_export();
};

}  // namespace imagesAnnotatorQt6::window::event_handlers

#endif  // IMAGES_ANNOTATOR_PROJECT_QT6_EVENTHANDLERS_EXPORTEVENTSHANDLER_CLASS_H
