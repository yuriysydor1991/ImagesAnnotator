#include "src/gtkmm3/GtkmmIniter.h"

#include <cassert>

#include "project-global-decls.h"
#include "src/log/log.h"

namespace templateGtkmm3
{

GtkmmIniter::GtkmmIniter() : GtkmmIniter(MainWindowContext::build_context())
{
  assert(mwctx != nullptr);
  assert(mwctx->wctx != nullptr);
  assert(mwctx->wloader != nullptr);
  assert(mwctx->cwFactory != nullptr);
}

GtkmmIniter::GtkmmIniter(std::shared_ptr<MainWindowContext> nmwctx)
    : mwctx{nmwctx}
{
  assert(mwctx != nullptr);
  assert(mwctx->wctx != nullptr);
  assert(mwctx->wloader != nullptr);
  assert(mwctx->cwFactory != nullptr);
}

bool GtkmmIniter::run(std::shared_ptr<app::ApplicationContext> ctx)
{
  assert(ctx != nullptr);
  assert(mwctx->wctx != nullptr);
  assert(mwctx->wloader != nullptr);

  if (ctx == nullptr) {
    return false;
  }

  mwctx->actx = ctx;

  auto app = Gtk::Application::create(mwctx->actx->argc, mwctx->actx->argv,
                                      mwctx->actx->app_dbus_url);

  if (!prepare_widgets()) {
    return false;
  }

  assert(mwctx->wloader->all_widget_are_valid());

  if (!mwctx->wloader->all_widget_are_valid()) {
    LOGE("Some of the widgets were not found");
    return false;
  }

  if (!subscribe_4_events()) {
    LOGE("Fail to subscribe for events");
    return false;
  }

  mwctx->wloader->get_window()->show_all_children();

  if (!mwctx->mweHandler->init(mwctx)) {
    LOGE("Failure to initialize the main window event handler instance");
    return false;
  }

  const auto rt = app->run(*mwctx->wloader->get_window());

  return rt == 0;
}

bool GtkmmIniter::subscribe_4_events()
{
  assert(mwctx->actx != nullptr);
  assert(mwctx->actx->eventer != nullptr);

  if (mwctx->actx == nullptr) {
    LOGE("No valid context object provided");
    return false;
  }

  if (mwctx->actx->eventer == nullptr) {
    LOGE("No valid context' eventer object provided");
    return false;
  }

  auto myshared = shared_from_this();
  std::shared_ptr<ImagesDirProviderChangedHandler> myptr = myshared;

  mwctx->actx->eventer->subscribe(myptr);

  /**
   * Sending requests for the initial objects.
   */
  auto efactory = mwctx->actx->eventer->get_events_factory();

  auto idbRequest = efactory->create_image_dir_object_request();

  mwctx->actx->eventer->submit(idbRequest);

  return true;
}

bool GtkmmIniter::prepare_widgets()
{
  assert(mwctx != nullptr);
  assert(mwctx->wctx != nullptr);
  assert(mwctx->wloader != nullptr);

  if (mwctx->wloader == nullptr) {
    LOGE("No loader available");
    return false;
  }

  if (!mwctx->wloader->load_window(mwctx)) {
    LOGE("Failure during the window load");
    return false;
  }

  assert(mwctx->wloader->get_window() != nullptr);
  assert(mwctx->wloader->all_widget_are_valid());

  if (!mwctx->wloader->all_widget_are_valid()) {
    LOGE("Window builder returned invalid status");
    return false;
  }

  return mwctx->wloader->get_window() != nullptr;
}

void GtkmmIniter::handle(std::shared_ptr<ImagesDirProviderChanged> event)
{
  assert(event != nullptr);
  assert(event->images_provider != nullptr);
  assert(mwctx->wloader != nullptr);
  assert(mwctx->wloader->get_images_list() != nullptr);
  assert(mwctx->cwFactory != nullptr);

  if (event == nullptr || event->images_provider == nullptr) {
    LOGE("No valid event object provided");
    return;
  }

  auto& imagesDB = event->images_provider->get_images_db();

  mwctx->imagesVDB = mwctx->cwFactory->create_images_visual_db(imagesDB);

  auto& imagesListView = *mwctx->wloader->get_images_list();

  auto children = imagesListView.get_children();

  for (auto* child : children) {
    imagesListView.remove(*child);
  }

  for (auto& r : mwctx->imagesVDB) {
    imagesListView.append(*r);
  }
}

void GtkmmIniter::deinit() { mwctx->clear(); }

}  // namespace templateGtkmm3
