#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM3_CUSTOM_WIDGETS_ALLANNOTATIONSLABEL_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM3_CUSTOM_WIDGETS_ALLANNOTATIONSLABEL_CLASS_H

#include <memory>

#include "src/gtkmm3/gtkmm_includes.h"
#include "src/gtkmm3/main-window/WindowDataContext.h"

namespace templateGtkmm3::window::custom_widgets
{

/**
 * @brief Subclass for all the annotations display in the LibBox.
 */
class AllAnnotationsLabel
    : public Gtk::Label,
      public std::enable_shared_from_this<AllAnnotationsLabel>
{
 public:
  virtual ~AllAnnotationsLabel() = default;
  AllAnnotationsLabel(const std::string& ntext);
};

using AllAnnotationsLabelPtr = std::shared_ptr<AllAnnotationsLabel>;
using AllAnnotationsLabelSet = std::vector<AllAnnotationsLabel>;

}  // namespace templateGtkmm3::window::custom_widgets

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_GTKMM3_CUSTOM_WIDGETS_ALLANNOTATIONSLABEL_CLASS_H
