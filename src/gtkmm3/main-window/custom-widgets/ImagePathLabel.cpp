#include "src/gtkmm3/main-window/custom-widgets/ImagePathLabel.h"

#include <cassert>
#include <memory>

#include "src/annotator-events/events/ImageRecord.h"
#include "src/gtkmm3/gtkmm_includes.h"
#include "src/gtkmm3/main-window/WindowDataContext.h"
#include "src/log/log.h"

namespace templateGtkmm3::window::custom_widgets
{

ImagePathLabel::ImagePathLabel(const std::shared_ptr<ImageRecord> nr)
    : myrec{nr}
{
  assert(myrec != nullptr);
  assert(!myrec->path.empty());

  if (myrec == nullptr) {
    LOGE("No valid image record provided");
    return;
  }

  if (myrec->path.empty()) {
    LOGW("Image db record contains no path");
    return;
  }

  LOGT("My path: " << myrec->path);

  set_text(myrec->get_full_path());

  set_ellipsize(Pango::ELLIPSIZE_START);
  set_single_line_mode(true);
}

std::shared_ptr<ImagePathLabel::ImageRecord> ImagePathLabel::get_image_rec()
{
  return myrec;
}

bool ImagePathLabel::mark_as_has_records()
{
  get_style_context()->add_class(has_records_css_class);

  return true;
}

bool ImagePathLabel::remove_has_records_mark()
{
  get_style_context()->remove_class(has_records_css_class);

  return true;
}

}  // namespace templateGtkmm3::window::custom_widgets
