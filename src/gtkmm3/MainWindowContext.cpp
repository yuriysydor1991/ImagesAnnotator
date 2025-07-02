#include "src/gtkmm3/MainWindowContext.h"

#include <cassert>
#include <memory>

#include "src/app/ApplicationContext.h"
#include "src/gtkmm3/ComponentTypesAliases.h"
#include "src/gtkmm3/main-window/WindowDataContext.h"
#include "src/gtkmm3/main-window/WindowLoader.h"
#include "src/gtkmm3/main-window/custom-widgets/CustomWidgetsFactory.h"
#include "src/log/log.h"

namespace templateGtkmm3
{

std::shared_ptr<MainWindowContext> MainWindowContext::build_context()
{
  auto nmwctx = std::make_shared<MainWindowContext>();

  nmwctx->wctx = std::make_shared<window::WindowDataContext>();
  nmwctx->cwFactory = std::make_shared<CWFactory>();
  nmwctx->wloader = std::make_shared<window::WindowLoader>();
  nmwctx->mweHandler = std::make_shared<window::WindowEventsHandler>();

  return nmwctx;
}

void MainWindowContext::clear()
{
  actx.reset();
  wctx.reset();
  mweHandler.reset();
  cwFactory.reset();
  wloader.reset();
  images_provider.reset();
}

std::shared_ptr<MainWindowContext::ImageRecord>
MainWindowContext::current_irecord()
{
  if (current_image == nullptr) {
    LOGT("No current image available");
    return {};
  }

  return current_image->get_image_rec();
}

bool MainWindowContext::validate_context(
    std::shared_ptr<MainWindowContext> nmwctx)
{
  assert(nmwctx != nullptr);
  assert(nmwctx->wctx != nullptr);
  assert(nmwctx->wloader != nullptr);
  assert(nmwctx->cwFactory != nullptr);
  assert(nmwctx->actx != nullptr);
  assert(nmwctx->actx->eventer != nullptr);
  assert(nmwctx->mweHandler != nullptr);

  if (nmwctx == nullptr) {
    LOGE("No valid context pointer provided");
    return false;
  }

  if (nmwctx->wctx == nullptr) {
    LOGE("No window context pointer provided");
    return false;
  }

  if (nmwctx->wloader == nullptr) {
    LOGE("No window builder instance provided");
    return false;
  }

  if (nmwctx->cwFactory == nullptr) {
    LOGE("No custom widgets factory pointer provided");
    return false;
  }

  if (!nmwctx->wloader->all_widget_are_valid()) {
    LOGE("Not all widgets are valid inside the window builder");
    return false;
  }

  if (nmwctx->actx == nullptr) {
    LOGE("Context does not contain a valid application context pointer");
    return false;
  }

  if (nmwctx->actx->eventer == nullptr) {
    LOGE("No valid iventer pointer provided in the context");
    return false;
  }

  if (nmwctx->mweHandler == nullptr) {
    LOGE("No valid main window event handler pointer available");
    return false;
  }

  return true;
}

}  // namespace templateGtkmm3
