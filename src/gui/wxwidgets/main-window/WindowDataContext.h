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

#ifndef IMAGES_ANNOTATOR_PROJECT_WX_WINDOWDATACONTEXT_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_WX_WINDOWDATACONTEXT_CLASS_H

#include <set>

#include "src/gui/wxwidgets/wx_includes.h"

namespace imagesAnnotatorWx::window
{

/**
 * @brief Holds the stable command identifiers for the menu items of the main
 * window - the wxWidgets analogue of imagesAnnotatorGtkmm3's WindowDataContext.
 *
 * In GTKmm the window was built from a Glade .ui file and every widget was
 * looked up by a string id, so that context held the id strings and resource
 * paths. The wx window is built in code and its controls (buttons, lists,
 * entries) are bound through their own pointers, so only the menu items - which
 * dispatch wxEVT_MENU by integer id - need stable identifiers, gathered here.
 */
class WindowDataContext
{
 public:
  virtual ~WindowDataContext() = default;
  WindowDataContext() = default;

  enum MenuId
  {
    ID_IMAGES_OPEN = wxID_HIGHEST + 1,
    ID_IMAGES_WEB_PAGE_OPEN,
    ID_ANNOTATIONS_DB_OPEN,
    ID_ANNOTATIONS_DB_SAVE,
    ID_ANNOTATIONS_DB_SAVE_AS,
    ID_ANNOTATIONS_PROJECT_CLOSE,
    ID_EXPORT_TXT,
    ID_EXPORT_YOLO4,
    ID_EXPORT_PYTORCH,
    ID_EXPORT_ULTRALYTICS_DETECT,
    ID_EXPORT_ULTRALYTICS_OBB,
    ID_EXPORT_ULTRALYTICS_SEGMENT,
    ID_EXPORT_COCO,
    ID_EXPORT_PASCAL_VOC,
    ID_EXPORT_CREATEML,
    ID_EXPORT_DXXWJZ2,
    ID_EXPORT_DXXWJZ2P,
    ID_EXPORT_DXXWJZ3,
    ID_EXPORT_DXXWJZ3P,
    ID_IMPORT_TXT,
    ID_IMPORT_YOLO4,
    ID_IMPORT_ULTRALYTICS_DETECT,
    ID_IMPORT_ULTRALYTICS_OBB,
    ID_IMPORT_ULTRALYTICS_SEGMENT,
    ID_IMPORT_COCO,
    ID_IMPORT_PASCAL_VOC,
    ID_IMPORT_CREATEML,
    ID_IMPORT_PYTORCH,
    ID_IMPORT_DXXWJZ2,
    ID_IMPORT_DXXWJZ2P,
    ID_IMPORT_DXXWJZ3,
    ID_IMPORT_DXXWJZ3P,
    ID_AUTO_INSERT_TO_WHOLE,
    ID_ABOUT
  };

  /// @brief The full set of menu command ids the window installs.
  static const std::set<int>& get_ids();
};

}  // namespace imagesAnnotatorWx::window

#endif  // IMAGES_ANNOTATOR_PROJECT_WX_WINDOWDATACONTEXT_CLASS_H
