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

#ifndef IMAGES_ANNOTATOR_PROJECT_QT6_QT6INITER_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_QT6_QT6INITER_CLASS_H

#include <memory>

#include "src/app/ApplicationContext.h"
#include "src/gui/IGUIIniter.h"

namespace imagesAnnotatorQt6
{

class MainWindowContext;

/**
 * @brief The Qt6 analogue of imagesAnnotatorGtkmm3::GtkmmIniter.
 *
 * Owns the Qt application lifetime and the main window context. The rest of the
 * application depends only on the toolkit-neutral alias gui::GUIIniter (see
 * src/gui/GUIIniter.h), so this class implements the very same gui::IGUIIniter
 * interface the GtkmmIniter does.
 */
class Qt6Initer : public std::enable_shared_from_this<Qt6Initer>,
                  virtual public gui::IGUIIniter
{
 public:
  virtual ~Qt6Initer() = default;
  Qt6Initer();
  explicit Qt6Initer(std::shared_ptr<MainWindowContext> nmwctx);

  virtual bool run(std::shared_ptr<app::ApplicationContext> ctx) override;

  virtual void deinit() override;

 protected:
  virtual bool prepare_widgets();

 private:
  std::shared_ptr<MainWindowContext> mwctx;
};

}  // namespace imagesAnnotatorQt6

#endif  // IMAGES_ANNOTATOR_PROJECT_QT6_QT6INITER_CLASS_H
