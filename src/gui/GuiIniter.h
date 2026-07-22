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

#ifndef IMAGES_ANNOTATOR_PROJECT_GUI_GUIINITER_FACADE_H
#define IMAGES_ANNOTATOR_PROJECT_GUI_GUIINITER_FACADE_H

/**
 * @file
 * @brief Toolkit-neutral facade over the concrete GtkmmIniter implementation.
 *
 * The project ships two interchangeable GUI implementations under src/gui
 * (gtkmm3 and gtkmm4). Exactly one is compiled into the application binary,
 * selected at build time by the mutually exclusive USE_GTKMM3 / USE_GTKMM4
 * CMake options, which define the matching preprocessor macro (see
 * cmake/template-project-misc-variables-declare.cmake). The rest of the
 * application depends only on the toolkit-neutral alias @c gui::GtkmmIniter and
 * never on a specific toolkit namespace.
 */

#if defined(USE_GTKMM4)
#include "src/gui/gtkmm4/GtkmmIniter.h"
#else
#include "src/gui/gtkmm3/GtkmmIniter.h"
#endif

namespace gui
{

#if defined(USE_GTKMM4)
using GtkmmIniter = templateGtkmm4::GtkmmIniter;
#else
using GtkmmIniter = templateGtkmm3::GtkmmIniter;
#endif

}  // namespace gui

#endif  // IMAGES_ANNOTATOR_PROJECT_GUI_GUIINITER_FACADE_H
