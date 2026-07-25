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

#ifndef IMAGES_ANNOTATOR_PROJECT_WX_EVENTHANDLERS_ABSTRACTWINDOWEVENTHANDLER_CLASS_H
#define IMAGES_ANNOTATOR_PROJECT_WX_EVENTHANDLERS_ABSTRACTWINDOWEVENTHANDLER_CLASS_H

#include <memory>

namespace templateWx
{
class MainWindowContext;
}  // namespace templateWx

namespace templateWx::window::event_handlers
{

class WindowOps;

/**
 * @brief The common interface for the decomposed window event handlers - the
 * wxWidgets analogue of templateGtkmm3's AbstractWindowEventHandler. Each
 * concrete handler receives the shared WindowOps through its constructor and
 * binds its own wx events to its handle(...) methods inside subscribe().
 */
class AbstractWindowEventHandler
{
 public:
  using WindowOpsPtr = std::shared_ptr<WindowOps>;

  virtual ~AbstractWindowEventHandler() = default;
  explicit AbstractWindowEventHandler(WindowOpsPtr nops);

  /// @brief Bind the handler's wx events to its callbacks.
  /// @return Returns true when the handler successfully subscribed.
  virtual bool subscribe() = 0;

 protected:
  WindowOpsPtr ops;
  std::shared_ptr<MainWindowContext> mwctx;
};

}  // namespace templateWx::window::event_handlers

#endif  // IMAGES_ANNOTATOR_PROJECT_WX_EVENTHANDLERS_ABSTRACTWINDOWEVENTHANDLER_CLASS_H
