#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM3_CUSTOM_WIDGETS_IMAGEPATHLABEL_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM3_CUSTOM_WIDGETS_IMAGEPATHLABEL_CLASS_H

#include <memory>

#include "src/annotator-events/events/ImageRecord.h"
#include "src/gtkmm3/gtkmm_includes.h"
#include "src/gtkmm3/main-window/WindowDataContext.h"

namespace templateGtkmm3::window::custom_widgets
{

/**
 * @brief Subclass for images db path display in the LibBox.
 */
class ImagePathLabel : public Gtk::Label,
                       public std::enable_shared_from_this<ImagePathLabel>
{
 public:
  using ImageRecord = events::events::ImageRecord;

  virtual ~ImagePathLabel() = default;
  explicit ImagePathLabel(std::shared_ptr<ImageRecord> nr);

  virtual std::shared_ptr<ImageRecord> get_image_rec();

 private:
  std::shared_ptr<ImageRecord> myrec;
};

}  // namespace templateGtkmm3::window::custom_widgets

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM3_CUSTOM_WIDGETS_IMAGEPATHLABEL_CLASS_H
