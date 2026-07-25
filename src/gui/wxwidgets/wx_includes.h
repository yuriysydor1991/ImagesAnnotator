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

#ifndef IMAGES_ANNOTATOR_PROJECT_GUI_WX_INCLUDES_H
#define IMAGES_ANNOTATOR_PROJECT_GUI_WX_INCLUDES_H

/**
 * @file
 * @brief Single funnel for every wxWidgets header used by the wx GUI.
 *
 * Mirrors src/gui/gtkmm3/gtkmm_includes.h: the @c \#pragma GCC system_header
 * makes the compiler treat the wxWidgets headers pulled in below as system
 * headers, so the project's strict warning set (-Wold-style-cast,
 * -Wuseless-cast, -Wzero-as-null-pointer-constant, ...) does not fire inside
 * them. Project code stays fully warned.
 */

#pragma GCC system_header

#include <wx/aboutdlg.h>
#include <wx/app.h>
#include <wx/busyinfo.h>
#include <wx/dcbuffer.h>
#include <wx/dirdlg.h>
#include <wx/filedlg.h>
#include <wx/frame.h>
#include <wx/image.h>
#include <wx/listctrl.h>
#include <wx/msgdlg.h>
#include <wx/mstream.h>
#include <wx/scrolwin.h>
#include <wx/splitter.h>
#include <wx/srchctrl.h>
#include <wx/statusbr.h>
#include <wx/textdlg.h>
#include <wx/wx.h>

#endif  // IMAGES_ANNOTATOR_PROJECT_GUI_WX_INCLUDES_H
