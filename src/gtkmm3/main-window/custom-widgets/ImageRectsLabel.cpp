#include "src/gtkmm3/main-window/custom-widgets/ImageRectsLabel.h"

#include <cassert>
#include <memory>

#include "src/annotator-events/events/ImageRecord.h"
#include "src/gtkmm3/gtkmm_includes.h"
#include "src/gtkmm3/main-window/WindowDataContext.h"
#include "src/log/log.h"

namespace templateGtkmm3::window::custom_widgets
{

void ImageRectsLabel::set(ImageRecordRectPtr nptr)
{
  myrec = nptr;

  assert(myrec != nullptr);
  assert(!myrec->name.empty());

  if (myrec == nullptr) {
    LOGE("No valid image record provided");
    return;
  }

  if (myrec->name.empty()) {
    LOGW("Image db record contains no path");
    return;
  }

  LOGT("My name: " << myrec->name);

  set_text(myrec->name);

  set_ellipsize(Pango::ELLIPSIZE_START);
  set_single_line_mode(true);
}

ImageRectsLabel::ImageRecordRectPtr ImageRectsLabel::get() { return myrec; }

}  // namespace templateGtkmm3::window::custom_widgets
