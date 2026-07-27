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

#ifndef IMAGES_ANNOTATOR_PROJECT_QT6_QT_STR_UTILS_H
#define IMAGES_ANNOTATOR_PROJECT_QT6_QT_STR_UTILS_H

#include <algorithm>
#include <cctype>
#include <string>

#include "src/gui/qt6/qt_includes.h"

namespace templateQt6
{

/// @brief std::string (UTF-8) -> QString.
inline QString u8(const std::string& s)
{
  return QString::fromUtf8(s.c_str(), static_cast<qsizetype>(s.size()));
}

/// @brief QString -> std::string (UTF-8).
inline std::string from_qt(const QString& s)
{
  const QByteArray utf8 = s.toUtf8();

  return std::string(utf8.constData(), static_cast<std::size_t>(utf8.size()));
}

/// @brief ASCII-lowercased copy, used for the case-insensitive list filters
/// (the Qt analogue of Glib::ustring::lowercase() in the GTKmm handlers).
inline std::string to_lower(std::string s)
{
  std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) {
    return static_cast<char>(std::tolower(c));
  });
  return s;
}

}  // namespace templateQt6

#endif  // IMAGES_ANNOTATOR_PROJECT_QT6_QT_STR_UTILS_H
