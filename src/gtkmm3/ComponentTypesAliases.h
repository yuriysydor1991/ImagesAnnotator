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

#ifndef IMAGES_ANNOTATOR_PROJECT_COMPONENTTYPEALIASES_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_COMPONENTTYPEALIASES_CLASS_H

#include "src/annotator-events/events/DisplayErrorEvent.h"
#include "src/annotator-events/events/DisplayErrorEventHandler.h"
#include "src/annotator-events/events/ImageRecord.h"
#include "src/annotator-events/events/ImageRecordRect.h"
#include "src/gtkmm3/main-window/custom-widgets/AllAnnotationsLabel.h"
#include "src/gtkmm3/main-window/custom-widgets/CentralWorkingCanvas.h"
#include "src/gtkmm3/main-window/custom-widgets/CustomWidgetsFactory.h"
#include "src/gtkmm3/main-window/custom-widgets/ImageRectsLabel.h"

namespace templateGtkmm3::window::custom_widgets
{
class CustomWidgetsFactory;
}

namespace templateGtkmm3
{

class ComponentTypesAliases
{
 public:
  virtual ~ComponentTypesAliases() = default;
  ComponentTypesAliases() = default;

  using CWFactory = window::custom_widgets::CustomWidgetsFactory;
  using ImagesVisualDB = CWFactory::ImagesVisualDB;
  using ImagePathLabel = window::custom_widgets::ImagePathLabel;
  using ImagePathLabelPtr = window::custom_widgets::ImagePathLabelPtr;
  using CentralWorkingCanvas =
      templateGtkmm3::window::custom_widgets::CentralWorkingCanvas;
  using ImageRecord = events::events::ImageRecord;
  using ImageRecordRectSet = events::events::ImageRecordRectSet;
  using ImageRectsLabel = window::custom_widgets::ImageRectsLabel;
  using ImageRectsLabelSet = window::custom_widgets::ImageRectsLabelSet;
  using ImageRectsLabelPtr = window::custom_widgets::ImageRectsLabelPtr;
  using AnnotationsVisualList = CWFactory::AnnotationsVisualList;
  using AllAnnotationsLabel = window::custom_widgets::AllAnnotationsLabel;
  using AllAnnotationsLabelPtr = window::custom_widgets::AllAnnotationsLabelPtr;
  using DisplayErrorEvent = events::events::DisplayErrorEvent;
  using DisplayErrorEventPtr = events::events::DisplayErrorEventPtr;
  using DisplayErrorEventHandler = events::events::DisplayErrorEventHandler;
};

}  // namespace templateGtkmm3

#endif  // IMAGES_ANNOTATOR_PROJECT_COMPONENTTYPEALIASES_CLASS_H
