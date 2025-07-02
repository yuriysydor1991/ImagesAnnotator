#include "src/gtkmm3/main-window/custom-widgets/CustomWidgetsFactory.h"

#include <cassert>
#include <memory>
#include <vector>

#include "src/annotator-events/events/ImageRecord.h"
#include "src/annotator-events/events/ImagesPathsDBProvider.h"
#include "src/gtkmm3/MainWindowContext.h"
#include "src/gtkmm3/main-window/custom-widgets/ImagePathLabel.h"
#include "src/log/log.h"

namespace templateGtkmm3::window::custom_widgets
{

std::shared_ptr<ImagePathLabel> CustomWidgetsFactory::create_image_db_label(
    std::shared_ptr<ImageRecord> ir)
{
  assert(ir != nullptr);

  if (ir == nullptr) {
    LOGE("No valid image record pointer provided");
    return {};
  }

  return std::make_shared<ImagePathLabel>(ir);
}

CustomWidgetsFactory::ImagesVisualDB
CustomWidgetsFactory::create_images_visual_db(ImageRecordsSet& queue)
{
  ImagesVisualDB vdb;

  vdb.reserve(queue.size());

  for (auto& ir : queue) {
    vdb.emplace_back(create_image_db_label(ir));
  }

  return vdb;
}

std::shared_ptr<CentralWorkingCanvas>
CustomWidgetsFactory::create_working_canvas(
    std::shared_ptr<ImagePathLabel>& nci)
{
  auto canvas = std::make_shared<CentralWorkingCanvas>(nci);

  canvas->add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK |
                     Gdk::POINTER_MOTION_MASK);

  return canvas;
}

}  // namespace templateGtkmm3::window::custom_widgets
