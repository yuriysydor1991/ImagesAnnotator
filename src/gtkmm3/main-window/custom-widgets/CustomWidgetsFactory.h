#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM_WINDOWLOADER_CUSTOMWIDGETS_FACTORY_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM_WINDOWLOADER_CUSTOMWIDGETS_FACTORY_CLASS_H

#include <memory>
#include <vector>

#include "src/annotator-events/events/ImageRecord.h"
#include "src/annotator-events/events/ImagesPathsDBProvider.h"
#include "src/gtkmm3/main-window/custom-widgets/ImagePathLabel.h"

namespace templateGtkmm3::window::custom_widgets
{

/**
 * @brief Class to hold the window loader routines from the UI xml files.
 */
class CustomWidgetsFactory
{
 public:
  using ImageRecord = events::events::ImageRecord;
  using eImagesDB = events::events::ImagesPathsDBProvider::ImagesDB;
  using ImagesVisualDB = std::vector<std::shared_ptr<ImagePathLabel>>;

  virtual ~CustomWidgetsFactory() = default;
  CustomWidgetsFactory() = default;

  virtual std::shared_ptr<ImagePathLabel> create_image_db_label(
      std::shared_ptr<ImageRecord> ir);

  virtual ImagesVisualDB create_images_visual_db(eImagesDB& queue);
};

}  // namespace templateGtkmm3::window::custom_widgets

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM_WINDOWLOADER_CUSTOMWIDGETS_FACTORY_CLASS_H
