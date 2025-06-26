#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_COMPONENTTYPEALIASES_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_COMPONENTTYPEALIASES_CLASS_H

#include <memory>

#include "src/gtkmm3/main-window/WindowDataContext.h"
#include "src/gtkmm3/main-window/WindowLoader.h"
#include "src/gtkmm3/main-window/custom-widgets/CustomWidgetsFactory.h"

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
};

}  // namespace templateGtkmm3

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_COMPONENTTYPEALIASES_CLASS_H
