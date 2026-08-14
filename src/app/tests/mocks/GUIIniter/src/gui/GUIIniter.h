#ifndef IMAGES_ANNOTATOR_PROJECT_GUI_GUIINITER_FACADE_MOCK_H
#define IMAGES_ANNOTATOR_PROJECT_GUI_GUIINITER_FACADE_MOCK_H

#include <gmock/gmock.h>

#include <functional>
#include <memory>

#include "src/app/ApplicationContext.h"
#include "src/gui/IGUIIniter.h"

// Test double for the toolkit-neutral gui::GUIIniter facade (see the real
// src/gui/GUIIniter.h). The app unit tests link no GUI toolkit libraries, so
// this mock implements the toolkit-free IGUIIniter interface itself and
// deliberately pulls in neither GTKmm, wxWidgets nor Qt.
namespace gui
{

class GUIIniter : virtual public IGUIIniter
{
 public:
  virtual ~GUIIniter() = default;
  GUIIniter()
  {
    if (onMockCreate) {
      onMockCreate(*this);
    }
  }

  inline static std::function<void(GUIIniter&)> onMockCreate;

  MOCK_METHOD(bool, run, (std::shared_ptr<app::ApplicationContext>),
              (override));
  MOCK_METHOD(void, deinit, (), (override));
};

}  // namespace gui

#endif  // IMAGES_ANNOTATOR_PROJECT_GUI_GUIINITER_FACADE_MOCK_H
