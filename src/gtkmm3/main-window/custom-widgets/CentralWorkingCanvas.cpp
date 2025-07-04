#include "src/gtkmm3/main-window/custom-widgets/CentralWorkingCanvas.h"

#include <cassert>
#include <memory>

#include "src/annotator-events/events/ImageRecord.h"
#include "src/gtkmm3/MainWindowContext.h"
#include "src/gtkmm3/gtkmm_includes.h"
#include "src/gtkmm3/main-window/WindowDataContext.h"
#include "src/log/log.h"

namespace templateGtkmm3::window::custom_widgets
{

CentralWorkingCanvas::CentralWorkingCanvas(std::shared_ptr<ImagePathLabel>& nci)
    : current_image{nci}
{
}

bool CentralWorkingCanvas::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
  if (!pixbuf) {
    return true;
  }

  Gdk::Cairo::set_source_pixbuf(cr, pixbuf, 0, 0);

  cr->paint();

  if (current_image == nullptr) {
    LOGT("No image provided");
    return true;
  }

  auto ir = current_image->get_image_rec();

  if (ir == nullptr) {
    LOGT("Current widget contains no image record");
    return true;
  }

  for (auto& rptr : ir->rects) {
    cr->set_line_width(2.0);

    if (ir->current_rect == rptr) {
      cr->set_source_rgb(0.9, 0.0, 0.0);
    } else {
      cr->set_source_rgb(0.0, 0.2, 0.8);
    }

    cr->rectangle((rptr->x * ir->imageScale), (rptr->y * ir->imageScale),
                  (rptr->width * ir->imageScale),
                  (rptr->height * ir->imageScale));
    cr->stroke();
  }

  return true;
}

bool CentralWorkingCanvas::set_pixbuf(Glib::RefPtr<Gdk::Pixbuf>& npb)
{
  assert(npb);

  if (!npb) {
    LOGE("No valid pixbuf pointer provided");
    return false;
  }

  pixbuf = npb;

  set_size_request(pixbuf->get_width(), pixbuf->get_height());

  queue_draw();

  return true;
}

}  // namespace templateGtkmm3::window::custom_widgets
