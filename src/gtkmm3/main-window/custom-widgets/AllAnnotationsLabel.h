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

#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM3_CUSTOM_WIDGETS_ALLANNOTATIONSLABEL_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM3_CUSTOM_WIDGETS_ALLANNOTATIONSLABEL_CLASS_H

#include <memory>

#include "src/gtkmm3/gtkmm_includes.h"
#include "src/gtkmm3/main-window/WindowDataContext.h"

namespace templateGtkmm3::window::custom_widgets
{

/**
 * @brief Subclass for all the annotations display in the LibBox.
 */
class AllAnnotationsLabel
    : public Gtk::Label,
      public std::enable_shared_from_this<AllAnnotationsLabel>
{
 public:
  virtual ~AllAnnotationsLabel() = default;
  explicit AllAnnotationsLabel(const std::string& ntext);
};

using AllAnnotationsLabelPtr = std::shared_ptr<AllAnnotationsLabel>;
using AllAnnotationsLabelSet = std::vector<AllAnnotationsLabel>;

}  // namespace templateGtkmm3::window::custom_widgets

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM3_CUSTOM_WIDGETS_ALLANNOTATIONSLABEL_CLASS_H
