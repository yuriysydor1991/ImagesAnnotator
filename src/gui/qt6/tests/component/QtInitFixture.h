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

#ifndef IMAGES_ANNOTATOR_PROJECT_QT6_TESTS_QT_INIT_FIXTURE_H
#define IMAGES_ANNOTATOR_PROJECT_QT6_TESTS_QT_INIT_FIXTURE_H

#include <gtest/gtest.h>

#include "src/gui/qt6/qt_includes.h"

/**
 * @brief Test fixture that brings up the single process-wide QApplication the
 * Qt widget component tests need.
 *
 * Qt widgets (and QPixmap) require a QApplication and a platform plugin. When
 * the agent has a display the tests use it; otherwise the fixture selects Qt's
 * "offscreen" platform plugin, which ships in the very same package as QtGui
 * and is therefore available wherever these tests can be linked at all. That is
 * why this suite - unlike its GTKmm and wxWidgets counterparts, which self-skip
 * on a headless machine - always really runs.
 *
 * Exactly one QApplication may exist per process, so it is a function-local
 * static shared by every test in the binary.
 */
class QtInitFixture : public ::testing::Test
{
 protected:
  void SetUp() override { qt_app(); }

 private:
  static QApplication& qt_app()
  {
    static QApplication& app = []() -> QApplication& {
      if (!qEnvironmentVariableIsSet("DISPLAY") &&
          !qEnvironmentVariableIsSet("WAYLAND_DISPLAY")) {
        qputenv("QT_QPA_PLATFORM", "offscreen");
      }

      // argc/argv have to outlive the QApplication, hence the statics.
      static char appname[] = "qttests";
      static char* argv[] = {appname, nullptr};
      static int argc = 1;

      static QApplication instance{argc, argv};

      return instance;
    }();

    return app;
  }
};

#endif  // IMAGES_ANNOTATOR_PROJECT_QT6_TESTS_QT_INIT_FIXTURE_H
