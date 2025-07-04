#include "src/gtkmm3/main-window/custom-widgets/AllAnnotationsLabel.h"

#include <memory>

#include "src/annotator-events/events/ImageRecord.h"
#include "src/gtkmm3/gtkmm_includes.h"
#include "src/gtkmm3/main-window/WindowDataContext.h"
#include "src/log/log.h"

namespace templateGtkmm3::window::custom_widgets
{

AllAnnotationsLabel::AllAnnotationsLabel(const std::string& ntext)
    : Gtk::Label(ntext)
{
}

}  // namespace templateGtkmm3::window::custom_widgets
