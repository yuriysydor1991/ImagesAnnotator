#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_COMPONENTTYPEALIASES_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_COMPONENTTYPEALIASES_CLASS_H

#include "src/annotator-events/events/ImageRecord.h"
#include "src/gtkmm3/main-window/custom-widgets/CentralWorkingCanvas.h"
#include "src/gtkmm3/main-window/custom-widgets/CustomWidgetsFactory.h"

namespace templateGtkmm3::window::custom_widgets
{
class CustomWidgetsFactory;
}

namespace templateGtkmm3
{

class ComponentTypesAliases
{
 public:
  virtual ~ComponentTypesAliases() = default;
  ComponentTypesAliases() = default;

  using CWFactory = window::custom_widgets::CustomWidgetsFactory;
  using ImagesVisualDB = CWFactory::ImagesVisualDB;
  using ImagePathLabel = window::custom_widgets::ImagePathLabel;
  using CentralWorkingCanvas =
      templateGtkmm3::window::custom_widgets::CentralWorkingCanvas;
  using ImageRecord = events::events::ImageRecord;
};

}  // namespace templateGtkmm3

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_COMPONENTTYPEALIASES_CLASS_H
