#include "src/gtkmm3/window/custom-widgets/ImagePathLabel.h"

#include <cassert>
#include <memory>

#include "src/annotator-events/events/ImageRecord.h"
#include "src/gtkmm3/gtkmm_includes.h"
#include "src/gtkmm3/window/WindowDataContext.h"
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

  set_text(myrec->path);
}

}  // namespace templateGtkmm3::window::custom_widgets
