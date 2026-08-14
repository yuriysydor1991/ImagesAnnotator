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

#ifndef IMAGES_ANNOTATOR_PROJECT_GUI_IGUIINITER_ABSTRACT_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_GUI_IGUIINITER_ABSTRACT_CLASS_H

#include <memory>

#include "src/app/ApplicationContext.h"

/**
 * @brief The namespace to contain the toolkit-neutral graphical user
 * interface entities.
 */
namespace gui
{

/**
 * @brief The abstract class to be implemented by every toplevel GUI initer.
 *
 * The project ships four interchangeable implementations under src/gui -
 * imagesAnnotatorGtkmm3::GtkmmIniter, imagesAnnotatorGtkmm4::GtkmmIniter,
 * imagesAnnotatorWx::WxWidgetsIniter and imagesAnnotatorQt6::Qt6Initer - of
 * which exactly one is compiled into the application binary and named by the
 * toolkit-neutral gui::GUIIniter alias (see src/gui/GUIIniter.h). They live in
 * unrelated toolkit namespaces, so this interface is what keeps the contract
 * the rest of the application relies upon one and the same for all of them.
 */
class IGUIIniter
{
 public:
  virtual ~IGUIIniter() = default;
  IGUIIniter() = default;

  /**
   * @brief Creates the toplevel window and runs the toolkit main loop until
   * the user quits the application.
   *
   * @param ctx A filled context to pass data to the GUI instance.
   *
   * @return Implementors should return true in case of a graceful exit and
   * false in case of any error.
   */
  virtual bool run(std::shared_ptr<app::ApplicationContext> ctx) = 0;

  /// @brief Releases every widget and toolkit resource the run() call acquired.
  virtual void deinit() = 0;
};

}  // namespace gui

#endif  // IMAGES_ANNOTATOR_PROJECT_GUI_IGUIINITER_ABSTRACT_CLASS_H
