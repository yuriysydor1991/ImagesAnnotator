#include "src/gtkmm3/window/WindowLoader.h"

#include <cassert>
#include <memory>

#include "src/gtkmm3/gtkmm_includes.h"
#include "src/gtkmm3/window/WindowDataContext.h"
#include "src/log/log.h"

namespace templateGtkmm3::window
{

bool WindowLoader::load_window(std::shared_ptr<WindowDataContext> wctx)
{
  assert(wctx != nullptr);

  if (wctx == nullptr) {
    LOGE("No context was given");
    return false;
  }

  LOGD("Creating the builder from the path " << wctx->UI_res_path);

  builder = Gtk::Builder::create_from_resource(wctx->UI_res_path);

  if (!builder) {
    LOGE("Failed to create the builder");
    return false;
  }

  LOGT("Trying to load the objects");

  auto& id2widgetsList = wctx->get_ids();

  for (const auto& id : id2widgetsList) {
    Gtk::Widget* tptr{nullptr};

    builder->get_widget(id, tptr);

    assert(tptr != nullptr);

    if (tptr == nullptr) {
      LOGE("Fail to load widget by id: " << id);
      continue;
    }

    wctx->set_ptr(id, tptr);
  }

  return wctx->all_widget_pointers_valid();
}

}  // namespace templateGtkmm3::window
