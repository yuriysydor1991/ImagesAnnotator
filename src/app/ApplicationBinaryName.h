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

#ifndef IMAGES_ANNOTATOR_PROJECT_APP_APPLICATIONBINARYNAME_H
#define IMAGES_ANNOTATOR_PROJECT_APP_APPLICATIONBINARYNAME_H

#include <string>

#include "project-global-decls.h"

namespace app
{

/**
 * @brief The name of the binary being run.
 *
 * The project builds one and the same app component into the main application
 * and into the converter only binary beside it, so whatever names the running
 * program - the usage line of the help message and the version message - asks
 * here instead of assuming the project name.
 *
 * The IMAGES_ANNOTATOR_CONVERTER_ONLY definition this branches on is given to
 * the converter target alone (see src/ImagesAnnotatorConverter-target.cmake),
 * and only the sources of the app component are compiled once per application
 * target, so this header belongs to that component and is included from
 * nowhere else: an object library shared by both binaries is compiled once,
 * without the definition, and would always read the main name here.
 *
 * @return Returns the name of the binary this translation unit is compiled
 * into.
 */
inline const std::string& application_binary_name()
{
#ifdef IMAGES_ANNOTATOR_CONVERTER_ONLY
  return project_decls::PROJECT_CONVERTER_NAME;
#else
  return project_decls::PROJECT_NAME;
#endif  // IMAGES_ANNOTATOR_CONVERTER_ONLY
}

}  // namespace app

#endif  // IMAGES_ANNOTATOR_PROJECT_APP_APPLICATIONBINARYNAME_H
