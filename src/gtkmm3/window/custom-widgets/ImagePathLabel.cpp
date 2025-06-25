#include "src/gtkmm3/window/custom-widgets/ImagePathLabel.h"

#include <cassert>
#include <memory>

#include "src/annotator-events/events/ImageRecord.h"
#include "src/gtkmm3/gtkmm_includes.h"
#include "src/gtkmm3/window/WindowDataContext.h"

namespace templateGtkmm3::window::custom_widgets
{

ImagePathLabel::ImagePathLabel(const std::shared_ptr<ImageRecord> nr)
    : myrec{nr}
{
  assert(myrec != nullptr);
}

}  // namespace templateGtkmm3::window::custom_widgets
