#ifndef IMAGES_ANNOTATOR_PROJECT_WX_TESTS_WX_INIT_FIXTURE_H
#define IMAGES_ANNOTATOR_PROJECT_WX_TESTS_WX_INIT_FIXTURE_H

#include <gtest/gtest.h>

#include "src/gui/wxwidgets/wx_includes.h"

namespace
{

// A trivial concrete wxApp so wxWidgets has an application object to start.
class WxTestApp : public wxApp
{
 public:
  bool OnInit() override { return true; }
};

}  // namespace

/**
 * @brief Base fixture for the wx widget component tests - the wx analogue of
 * the GTKmm GtkInitFixture.
 *
 * Constructing wx windows requires an initialized wxApp and a usable display.
 * When none is available (a headless build agent) the tests self-skip instead
 * of crashing, so the suite stays green.
 */
class WxInitFixture : public ::testing::Test
{
 protected:
  void SetUp() override
  {
    static const bool wx_ok = []() {
      static char appname[] = "wxtests";
      static char* argv[] = {appname, nullptr};
      static int argc = 1;

      wxApp::SetInstance(new WxTestApp());

      // wxEntryStart opens the display; it returns false (does not abort) on a
      // headless agent, which is what lets the suite self-skip.
      return wxEntryStart(argc, argv);
    }();

    if (!wx_ok) {
      GTEST_SKIP() << "wxWidgets could not initialize (no display available)";
    }
  }
};

#endif  // IMAGES_ANNOTATOR_PROJECT_WX_TESTS_WX_INIT_FIXTURE_H
