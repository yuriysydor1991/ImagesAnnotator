#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM3_CUSTOM_WIDGETS_IMAGERECTSLABEL_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM3_CUSTOM_WIDGETS_IMAGERECTSLABEL_CLASS_H

#include <memory>

#include "src/annotator-events/events/ImageRecordRect.h"
#include "src/gtkmm3/gtkmm_includes.h"
#include "src/gtkmm3/main-window/WindowDataContext.h"

namespace templateGtkmm3::window::custom_widgets
{

/**
 * @brief Subclass for images db path display in the LibBox.
 */
class ImageRectsLabel : public Gtk::Label,
                        public std::enable_shared_from_this<ImageRectsLabel>
{
 public:
  using ImageRecordRectPtr = events::events::ImageRecordRectPtr;

  virtual ~ImageRectsLabel() = default;
  ImageRectsLabel() = default;

  void set(ImageRecordRectPtr nptr);

  ImageRecordRectPtr get();

 private:
  ImageRecordRectPtr myrec;
};

using ImageRectsLabelPtr = std::shared_ptr<ImageRectsLabel>;
using ImageRectsLabelSet = std::vector<ImageRectsLabelPtr>;

}  // namespace templateGtkmm3::window::custom_widgets

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM3_CUSTOM_WIDGETS_IMAGERECTSLABEL_CLASS_H
