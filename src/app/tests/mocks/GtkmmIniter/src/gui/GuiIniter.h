#ifndef IMAGES_ANNOTATOR_PROJECT_GUI_GUIINITER_FACADE_MOCK_H
#define IMAGES_ANNOTATOR_PROJECT_GUI_GUIINITER_FACADE_MOCK_H

#include <gmock/gmock.h>

#include <functional>
#include <memory>

#include "src/app/ApplicationContext.h"

// Test double for the toolkit-neutral gui::GtkmmIniter facade (see the real
// src/gui/GuiIniter.h). The app unit tests link no GTKmm libraries, so this
// mock deliberately pulls in neither gtkmm3 nor gtkmm4.
namespace gui
{

class GtkmmIniter
{
 public:
  virtual ~GtkmmIniter() = default;
  GtkmmIniter()
  {
    if (onMockCreate) {
      onMockCreate(*this);
    }
  }

  inline static std::function<void(GtkmmIniter&)> onMockCreate;

  MOCK_METHOD(int, run, (std::shared_ptr<app::ApplicationContext>));
  MOCK_METHOD(void, deinit, ());
};

}  // namespace gui

#endif  // IMAGES_ANNOTATOR_PROJECT_GUI_GUIINITER_FACADE_MOCK_H
