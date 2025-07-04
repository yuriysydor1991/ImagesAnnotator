#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_MAINWINDOWCONTEXT_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_MAINWINDOWCONTEXT_CLASS_H

#include <memory>

#include "src/annotator-events/events/ImagesPathsDBProvider.h"
#include "src/app/ApplicationContext.h"
#include "src/gtkmm3/ComponentTypesAliases.h"
#include "src/gtkmm3/main-window/WindowDataContext.h"
#include "src/gtkmm3/main-window/WindowEventsHandler.h"
#include "src/gtkmm3/main-window/WindowLoader.h"
#include "src/gtkmm3/main-window/custom-widgets/CentralWorkingCanvas.h"
#include "src/gtkmm3/main-window/custom-widgets/CustomWidgetsFactory.h"
#include "src/gtkmm3/main-window/custom-widgets/ImagePathLabel.h"
#include "src/gtkmm3/main-window/custom-widgets/ImageRectsLabel.h"

namespace templateGtkmm3::window
{
class WindowEventsHandler;
class WindowLoader;
}  // namespace templateGtkmm3::window

namespace templateGtkmm3::window::custom_widgets
{
class CentralWorkingCanvas;
}  // namespace templateGtkmm3::window::custom_widgets

namespace templateGtkmm3
{

class MainWindowContext : virtual public ComponentTypesAliases,
                          public std::enable_shared_from_this<MainWindowContext>
{
 public:
  virtual ~MainWindowContext() = default;

  std::shared_ptr<window::WindowDataContext> wctx;
  std::shared_ptr<window::WindowLoader> wloader;
  std::shared_ptr<CWFactory> cwFactory;
  ImagesVisualDB imagesVDB;
  std::shared_ptr<app::ApplicationContext> actx;
  std::shared_ptr<window::WindowEventsHandler> mweHandler;
  std::shared_ptr<CentralWorkingCanvas> centralCanvas;

  std::shared_ptr<ImagePathLabel> current_image;
  Glib::RefPtr<Gdk::Pixbuf> current_image_original_pixbuf;
  ImageRectsLabelSet currentVisualRects;
  AnnotationsVisualList annotationsList;

  std::shared_ptr<events::events::ImagesPathsDBProvider> images_provider;
  ImageRectsLabelPtr currentVisualRect;

  static std::shared_ptr<MainWindowContext> build_context();

  static bool validate_context(std::shared_ptr<MainWindowContext> nmwctx);

  virtual void clear();

  virtual std::shared_ptr<ImageRecord> current_irecord();
};

}  // namespace templateGtkmm3

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_MAINWINDOWCONTEXT_CLASS_H
