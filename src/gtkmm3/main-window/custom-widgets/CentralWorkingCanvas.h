#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM3_CUSTOM_WIDGETS_CENTRALWORKINGCANVAS_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM3_CUSTOM_WIDGETS_CENTRALWORKINGCANVAS_CLASS_H

#include <memory>

#include "src/annotator-events/events/ImageRecord.h"
#include "src/gtkmm3/gtkmm_includes.h"
#include "src/gtkmm3/main-window/custom-widgets/ImagePathLabel.h"

namespace templateGtkmm3
{
class MainWindowContext;
}

namespace templateGtkmm3::window::custom_widgets
{

/**
 * @brief Subclass of the DrawingArea to allow draw annotations over an image.
 */
class CentralWorkingCanvas
    : public Gtk::DrawingArea,
      public std::enable_shared_from_this<CentralWorkingCanvas>
{
 public:
  using ImageRecord = events::events::ImageRecord;

  virtual ~CentralWorkingCanvas() = default;
  explicit CentralWorkingCanvas(std::shared_ptr<ImagePathLabel>& nci);

  virtual bool set_pixbuf(Glib::RefPtr<Gdk::Pixbuf>& npb);

  virtual void clear();

 private:
  // Called when the widget needs to redraw
  virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

  std::shared_ptr<ImagePathLabel>& current_image;
  Glib::RefPtr<Gdk::Pixbuf> pixbuf;
};

}  // namespace templateGtkmm3::window::custom_widgets

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM3_CUSTOM_WIDGETS_CENTRALWORKINGCANVAS_CLASS_H
