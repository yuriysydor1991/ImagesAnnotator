#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM3_CUSTOM_WIDGETS_IMAGEPATHLABEL_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM3_CUSTOM_WIDGETS_IMAGEPATHLABEL_CLASS_H

#include <memory>

#include "src/annotator-events/events/ImageRecord.h"
#include "src/gtkmm3/gtkmm_includes.h"
#include "src/gtkmm3/window/WindowDataContext.h"

namespace templateGtkmm3::window::custom_widgets
{

/**
 * @brief Subclass for images db path display in the LibBox.
 */
class ImagePathLabel : public Gtk::Label
{
 public:
  using ImageRecord = events::events::ImageRecord;

  virtual ~ImagePathLabel() = default;
  ImagePathLabel(std::shared_ptr<ImageRecord> nr);

 private:
  std::shared_ptr<ImageRecord> myrec;
};

}  // namespace templateGtkmm3::window::custom_widgets

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM3_CUSTOM_WIDGETS_IMAGEPATHLABEL_CLASS_H
