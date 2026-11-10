#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstddef>
#include <memory>
#include <string>
#include <tuple>

#include "FakeWindowLoader.h"
#include "GtkInitFixture.h"
#include "src/annotator-events/ImagesAnnotatorEventController.h"
#include "src/annotator-events/events/ImageRecord.h"
#include "src/annotator-events/events/ImageRecordSegment.h"
#include "src/annotator-events/events/ImagesPathsDBProvider.h"
#include "src/app/ApplicationContext.h"
#include "src/gui/gtkmm3/MainWindowContext.h"
#include "src/gui/gtkmm3/main-window/WindowDataContext.h"
#include "src/gui/gtkmm3/main-window/WindowEventsHandler.h"
#include "src/gui/gtkmm3/main-window/custom-widgets/AllAnnotationsLabel.h"
#include "src/gui/gtkmm3/main-window/custom-widgets/CentralWorkingCanvas.h"
#include "src/gui/gtkmm3/main-window/custom-widgets/CustomWidgetsFactory.h"
#include "src/gui/gtkmm3/main-window/custom-widgets/ImagePathLabel.h"
#include "src/gui/gtkmm3/main-window/custom-widgets/ImageRectsLabel.h"
#include "src/gui/gtkmm3/main-window/event-handlers/AboutEventsHandler.h"
#include "src/gui/gtkmm3/main-window/event-handlers/AnnotationsEventsHandler.h"
#include "src/gui/gtkmm3/main-window/event-handlers/ExportEventsHandler.h"
#include "src/gui/gtkmm3/main-window/event-handlers/ImageListEventsHandler.h"
#include "src/gui/gtkmm3/main-window/event-handlers/ImageNavigationEventsHandler.h"
#include "src/gui/gtkmm3/main-window/event-handlers/ImportEventsHandler.h"
#include "src/gui/gtkmm3/main-window/event-handlers/KeyboardEventsHandler.h"
#include "src/gui/gtkmm3/main-window/event-handlers/MouseDrawEventsHandler.h"
#include "src/gui/gtkmm3/main-window/event-handlers/ProjectFileEventsHandler.h"
#include "src/gui/gtkmm3/main-window/event-handlers/WebPageEventsHandler.h"
#include "src/gui/gtkmm3/main-window/event-handlers/WindowOps.h"
#include "src/gui/gtkmm3/main-window/event-handlers/WindowSystemEventsHandler.h"
#include "src/gui/gtkmm3/main-window/event-handlers/ZoomEventsHandler.h"
#include "src/helpers/SegmentPathHelper.h"

using namespace testing;
using imagesAnnotatorGtkmm3::MainWindowContext;
using imagesAnnotatorGtkmm3::window::WindowDataContext;
using imagesAnnotatorGtkmm3::window::WindowEventsHandler;
using imagesAnnotatorGtkmm3::window::custom_widgets::AllAnnotationsLabel;
using imagesAnnotatorGtkmm3::window::custom_widgets::CentralWorkingCanvas;
using imagesAnnotatorGtkmm3::window::custom_widgets::CustomWidgetsFactory;
using imagesAnnotatorGtkmm3::window::custom_widgets::ImagePathLabel;
using imagesAnnotatorGtkmm3::window::custom_widgets::ImageRectsLabel;
namespace eh = imagesAnnotatorGtkmm3::window::event_handlers;

/**
 * @brief Component tests for the decomposed window event handlers.
 *
 * Each handler's subscribe() connects its GTK signals to the widgets exposed by
 * the WindowLoader. The fixture provides a MainWindowContext backed by the
 * FakeWindowLoader test double (real widgets, no UI resource loading) so the
 * wiring can be exercised against genuine GTK signals. Self-skips headlessly.
 */
class CTEST_WindowEventHandlers : public GtkInitFixture
{
 protected:
  std::shared_ptr<MainWindowContext> mwctx;
  std::shared_ptr<eh::WindowOps> ops;

  void SetUp() override
  {
    GtkInitFixture::SetUp();  // GTEST_SKIP()s when no display is available

    // GTEST_SKIP() in the base SetUp only returns from the base method, so bail
    // out here too before touching any GTK widget on a headless agent.
    if (IsSkipped()) {
      return;
    }

    mwctx = std::make_shared<MainWindowContext>();
    mwctx->wloader = std::make_shared<FakeWindowLoader>();
    ops = std::make_shared<eh::WindowOps>(mwctx);
  }

  // Drives every gated button into the insensitive baseline (no images, no
  // selection) so a later EXPECT_TRUE verifies a real false -> true transition
  // rather than the buttons' default-sensitive construction state.
  void drive_all_insensitive()
  {
    mwctx->imagesVDB.clear();
    mwctx->current_image.reset();
    mwctx->currentVisualRect.reset();
    ops->update_action_buttons_sensitivity();
  }

  // Appends `count` visible rows to the images list (and a matching imagesVDB
  // entry so the "images present" gate holds), then selects row `selected`
  // (pass -1 to leave nothing selected).
  void populate_images(int count, int selected)
  {
    auto* lb = mwctx->wloader->get_images_list();
    lb->set_visible(true);
    for (int i = 0; i < count; ++i) {
      mwctx->imagesVDB.push_back(std::make_shared<ImagePathLabel>(
          events::events::ImageRecord::create("a.png", "/d")));
      lb->append(*Gtk::make_managed<Gtk::Label>("img"));
      lb->get_row_at_index(i)->set_visible(true);
    }
    if (selected >= 0) {
      lb->select_row(*lb->get_row_at_index(selected));
    }
  }

  // Makes a current image whose preview scale is `scale` (and registers it in
  // imagesVDB so the "images present" gate holds).
  void set_current_image_scale(double scale)
  {
    auto label = std::make_shared<ImagePathLabel>(
        events::events::ImageRecord::create("a.png", "/d"));
    label->get_image_rec()->imageScale = scale;
    mwctx->imagesVDB.push_back(label);
    mwctx->current_image = label;
  }
};

TEST_F(CTEST_WindowEventHandlers, about_subscribe_returns_true)
{
  eh::AboutEventsHandler handler(ops);

  EXPECT_TRUE(handler.subscribe());
}

TEST_F(CTEST_WindowEventHandlers, keyboard_subscribe_returns_true)
{
  eh::KeyboardEventsHandler handler(ops);

  EXPECT_TRUE(handler.subscribe());
}

TEST_F(CTEST_WindowEventHandlers, zoom_subscribe_returns_true)
{
  // The zoom handler attaches the scroll wiring to the central canvas, so it
  // must exist before subscribing.
  std::shared_ptr<ImagePathLabel> ci;
  mwctx->centralCanvas = std::make_shared<CentralWorkingCanvas>(ci);

  eh::ZoomEventsHandler handler(ops);

  EXPECT_TRUE(handler.subscribe());
}

TEST_F(CTEST_WindowEventHandlers, image_navigation_subscribe_returns_true)
{
  eh::ImageNavigationEventsHandler handler(ops);

  EXPECT_TRUE(handler.subscribe());
}

TEST_F(CTEST_WindowEventHandlers, image_list_subscribe_returns_true)
{
  eh::ImageListEventsHandler handler(ops);

  EXPECT_TRUE(handler.subscribe());
}

TEST_F(CTEST_WindowEventHandlers, annotations_subscribe_returns_true)
{
  eh::AnnotationsEventsHandler handler(ops);

  EXPECT_TRUE(handler.subscribe());
}

TEST_F(CTEST_WindowEventHandlers, project_file_subscribe_returns_true)
{
  eh::ProjectFileEventsHandler handler(ops);

  EXPECT_TRUE(handler.subscribe());
}

TEST_F(CTEST_WindowEventHandlers, export_subscribe_returns_true)
{
  eh::ExportEventsHandler handler(ops);

  EXPECT_TRUE(handler.subscribe());
}

TEST_F(CTEST_WindowEventHandlers, import_subscribe_returns_true)
{
  eh::ImportEventsHandler handler(ops);

  EXPECT_TRUE(handler.subscribe());
}

TEST_F(CTEST_WindowEventHandlers, web_page_subscribe_returns_true)
{
  eh::WebPageEventsHandler handler(ops);

  EXPECT_TRUE(handler.subscribe());
}

TEST_F(CTEST_WindowEventHandlers, window_system_subscribe_returns_true)
{
  eh::WindowSystemEventsHandler handler(ops);

  EXPECT_TRUE(handler.subscribe());
}

TEST_F(CTEST_WindowEventHandlers, mouse_draw_subscribe_returns_true)
{
  std::shared_ptr<ImagePathLabel> ci;
  mwctx->centralCanvas = std::make_shared<CentralWorkingCanvas>(ci);

  eh::MouseDrawEventsHandler handler(ops);

  EXPECT_TRUE(handler.subscribe());
}

TEST_F(CTEST_WindowEventHandlers, action_buttons_insensitive_when_no_images)
{
  // No images loaded: the navigation / view-manipulation and the annotation
  // management buttons act on nothing, so they must be insensitive.
  mwctx->imagesVDB.clear();

  ops->update_action_buttons_sensitivity();

  EXPECT_FALSE(mwctx->wloader->get_prev_file_button()->get_sensitive());
  EXPECT_FALSE(mwctx->wloader->get_next_file_button()->get_sensitive());
  EXPECT_FALSE(mwctx->wloader->get_current_image_zoom_in()->get_sensitive());
  EXPECT_FALSE(mwctx->wloader->get_current_image_zoom_out()->get_sensitive());
  EXPECT_FALSE(mwctx->wloader->get_delete_image_record()->get_sensitive());
  EXPECT_FALSE(
      mwctx->wloader->get_copy_annotation_name_button()->get_sensitive());
  EXPECT_FALSE(
      mwctx->wloader->get_copy_current_annotation_button()->get_sensitive());
  EXPECT_FALSE(mwctx->wloader->get_delete_current_image_selected_annotation()
                   ->get_sensitive());

  // The list-populating entry-point buttons stay usable.
  EXPECT_TRUE(mwctx->wloader->get_images_folder_open_button()->get_sensitive());
  EXPECT_TRUE(
      mwctx->wloader->get_annotations_db_open_button()->get_sensitive());
}

TEST_F(CTEST_WindowEventHandlers, delete_buttons_sensitive_when_images_present)
{
  // Delete image / delete annotation are gated only on images being present.
  drive_all_insensitive();
  ASSERT_FALSE(mwctx->wloader->get_delete_image_record()->get_sensitive());

  set_current_image_scale(1.0);
  ops->update_action_buttons_sensitivity();

  EXPECT_TRUE(mwctx->wloader->get_delete_image_record()->get_sensitive());
  EXPECT_TRUE(mwctx->wloader->get_delete_current_image_selected_annotation()
                  ->get_sensitive());
}

// --- Zoom in / out bound on the current image scale ---

TEST_F(CTEST_WindowEventHandlers, zoom_out_insensitive_at_min_scale)
{
  set_current_image_scale(events::events::ImageRecord::minScale);
  ops->update_action_buttons_sensitivity();

  EXPECT_FALSE(mwctx->wloader->get_current_image_zoom_out()->get_sensitive());
  EXPECT_TRUE(mwctx->wloader->get_current_image_zoom_in()->get_sensitive());
}

TEST_F(CTEST_WindowEventHandlers, zoom_in_insensitive_at_max_scale)
{
  set_current_image_scale(events::events::ImageRecord::maxScale);
  ops->update_action_buttons_sensitivity();

  EXPECT_FALSE(mwctx->wloader->get_current_image_zoom_in()->get_sensitive());
  EXPECT_TRUE(mwctx->wloader->get_current_image_zoom_out()->get_sensitive());
}

TEST_F(CTEST_WindowEventHandlers, zoom_buttons_sensitive_at_intermediate_scale)
{
  drive_all_insensitive();
  ASSERT_FALSE(mwctx->wloader->get_current_image_zoom_in()->get_sensitive());

  set_current_image_scale(1.0);
  ops->update_action_buttons_sensitivity();

  EXPECT_TRUE(mwctx->wloader->get_current_image_zoom_in()->get_sensitive());
  EXPECT_TRUE(mwctx->wloader->get_current_image_zoom_out()->get_sensitive());
}

TEST_F(CTEST_WindowEventHandlers,
       zoom_buttons_insensitive_without_current_image)
{
  // Images loaded but none selected as current: nothing to zoom.
  mwctx->imagesVDB.push_back(std::make_shared<ImagePathLabel>(
      events::events::ImageRecord::create("a.png", "/d")));
  mwctx->current_image.reset();

  ops->update_action_buttons_sensitivity();

  EXPECT_FALSE(mwctx->wloader->get_current_image_zoom_in()->get_sensitive());
  EXPECT_FALSE(mwctx->wloader->get_current_image_zoom_out()->get_sensitive());
}

// --- Copy annotation buttons bound on a selected current annotation ---

TEST_F(CTEST_WindowEventHandlers,
       copy_annotation_buttons_insensitive_without_selected_annotation)
{
  set_current_image_scale(1.0);  // image present, but no annotation selected
  mwctx->currentVisualRect.reset();

  ops->update_action_buttons_sensitivity();

  EXPECT_FALSE(
      mwctx->wloader->get_copy_annotation_name_button()->get_sensitive());
  EXPECT_FALSE(
      mwctx->wloader->get_copy_current_annotation_button()->get_sensitive());
}

TEST_F(CTEST_WindowEventHandlers,
       copy_annotation_button_sensitive_with_selected_annotation_only)
{
  // A selected current-image annotation but no all-annotations selection: the
  // duplicate ("Copy annotation") button is usable, but "Copy annotation name"
  // (which needs a name to copy) is not.
  drive_all_insensitive();
  ASSERT_FALSE(
      mwctx->wloader->get_copy_current_annotation_button()->get_sensitive());

  set_current_image_scale(1.0);
  mwctx->currentVisualRect = std::make_shared<ImageRectsLabel>();
  mwctx->current_annotation_name.reset();

  ops->update_action_buttons_sensitivity();

  EXPECT_TRUE(
      mwctx->wloader->get_copy_current_annotation_button()->get_sensitive());
  EXPECT_FALSE(
      mwctx->wloader->get_copy_annotation_name_button()->get_sensitive());
}

TEST_F(CTEST_WindowEventHandlers,
       copy_annotation_name_button_sensitive_with_all_annotations_selection)
{
  // With both a selected current-image annotation and an all-annotations
  // selection, "Copy annotation name" becomes usable too.
  drive_all_insensitive();
  ASSERT_FALSE(
      mwctx->wloader->get_copy_annotation_name_button()->get_sensitive());

  set_current_image_scale(1.0);
  mwctx->currentVisualRect = std::make_shared<ImageRectsLabel>();
  mwctx->current_annotation_name = std::make_shared<AllAnnotationsLabel>("cat");

  ops->update_action_buttons_sensitivity();

  EXPECT_TRUE(
      mwctx->wloader->get_copy_annotation_name_button()->get_sensitive());
  EXPECT_TRUE(
      mwctx->wloader->get_copy_current_annotation_button()->get_sensitive());
}

// --- Previous / Next bound on the selected image's position ---

TEST_F(CTEST_WindowEventHandlers, navigation_insensitive_on_single_image)
{
  populate_images(1, 0);  // one row, selected

  ops->update_action_buttons_sensitivity();

  EXPECT_FALSE(mwctx->wloader->get_prev_file_button()->get_sensitive());
  EXPECT_FALSE(mwctx->wloader->get_next_file_button()->get_sensitive());
}

TEST_F(CTEST_WindowEventHandlers, previous_insensitive_on_first_image)
{
  populate_images(2, 0);  // first of two selected

  ops->update_action_buttons_sensitivity();

  EXPECT_FALSE(mwctx->wloader->get_prev_file_button()->get_sensitive());
  EXPECT_TRUE(mwctx->wloader->get_next_file_button()->get_sensitive());
}

TEST_F(CTEST_WindowEventHandlers, next_insensitive_on_last_image)
{
  populate_images(2, 1);  // last of two selected

  ops->update_action_buttons_sensitivity();

  EXPECT_TRUE(mwctx->wloader->get_prev_file_button()->get_sensitive());
  EXPECT_FALSE(mwctx->wloader->get_next_file_button()->get_sensitive());
}

TEST_F(CTEST_WindowEventHandlers, navigation_sensitive_on_middle_image)
{
  drive_all_insensitive();
  ASSERT_FALSE(mwctx->wloader->get_prev_file_button()->get_sensitive());

  populate_images(3, 1);  // middle of three selected

  ops->update_action_buttons_sensitivity();

  EXPECT_TRUE(mwctx->wloader->get_prev_file_button()->get_sensitive());
  EXPECT_TRUE(mwctx->wloader->get_next_file_button()->get_sensitive());
}

TEST_F(CTEST_WindowEventHandlers, navigation_insensitive_when_neighbors_hidden)
{
  // The middle row is selected but its only neighbours are hidden by the search
  // filter, so navigation would be a no-op: both directions go insensitive.
  // This is what distinguishes the visibility-aware scan from plain index
  // adjacency.
  populate_images(3, 1);
  auto* lb = mwctx->wloader->get_images_list();
  lb->get_row_at_index(0)->set_visible(false);
  lb->get_row_at_index(2)->set_visible(false);

  ops->update_action_buttons_sensitivity();

  EXPECT_FALSE(mwctx->wloader->get_prev_file_button()->get_sensitive());
  EXPECT_FALSE(mwctx->wloader->get_next_file_button()->get_sensitive());
}

TEST_F(CTEST_WindowEventHandlers, navigation_sensitive_when_nothing_selected)
{
  // Rows present but none selected (the state right after the list is
  // populated): either direction jumps to the first / last visible row, so both
  // stay usable.
  drive_all_insensitive();
  ASSERT_FALSE(mwctx->wloader->get_next_file_button()->get_sensitive());

  populate_images(2, -1);  // no selection

  ops->update_action_buttons_sensitivity();

  EXPECT_TRUE(mwctx->wloader->get_prev_file_button()->get_sensitive());
  EXPECT_TRUE(mwctx->wloader->get_next_file_button()->get_sensitive());
}

namespace
{

/// @brief The images provider double the window operations refresh the title,
/// the status bar and the all annotations list from.
class FakeImagesProvider : public events::events::ImagesPathsDBProvider
{
 public:
  events::events::ImageRecordsSet& get_images_db() override { return images; }
  std::string get_db_path() override { return "project.imgajz2"; }
  AnnotationsList get_available_annotations() override { return {}; }
  bool changed() override { return true; }
  bool delete_image_record([[maybe_unused]] const std::string& path) override
  {
    return false;
  }

 private:
  events::events::ImageRecordsSet images;
};

/// @brief Exposes the protected event handlers of the mouse draw handler.
class TestableMouseDrawEventsHandler : public eh::MouseDrawEventsHandler
{
 public:
  using eh::MouseDrawEventsHandler::handle_focus_out;
  using eh::MouseDrawEventsHandler::handle_key_release;
  using eh::MouseDrawEventsHandler::handle_motion_end;
  using eh::MouseDrawEventsHandler::handle_motion_event;
  using eh::MouseDrawEventsHandler::handle_motion_start;
  using eh::MouseDrawEventsHandler::MouseDrawEventsHandler;
};

/// @brief The image pixel space position of a segment path point
std::tuple<double, double> point_of(
    const events::events::ImageRecordSegmentPtr& segment,
    const std::size_t& index)
{
  return helpers::SegmentPathHelper::image_points(*segment).at(index);
}

}  // namespace

/**
 * @brief Drives the drawing of a segment by the Shift-held clicks and the
 * dragging of its points through the MouseDrawEventsHandler over a 200x200
 * image shown unscaled. Self-skips headlessly.
 */
class CTEST_SegmentEditing : public CTEST_WindowEventHandlers
{
 protected:
  int argc{0};
  char** argv{nullptr};
  std::shared_ptr<TestableMouseDrawEventsHandler> handler;
  events::events::ImageRecordPtr ir;

  void SetUp() override
  {
    CTEST_WindowEventHandlers::SetUp();

    if (IsSkipped()) {
      return;
    }

    // Every member the window operations validate the context of
    mwctx->wctx = std::make_shared<WindowDataContext>();
    mwctx->mweHandler = std::make_shared<WindowEventsHandler>();
    mwctx->actx = std::make_shared<app::ApplicationContext>(argc, argv);
    mwctx->actx->eventer =
        std::make_shared<events::ImagesAnnotatorEventController>(mwctx->actx);
    mwctx->cwFactory = std::make_shared<CustomWidgetsFactory>();
    mwctx->images_provider = std::make_shared<FakeImagesProvider>();

    ir = events::events::ImageRecord::create("a.png", "/d");
    ir->imageScale = 1.0;

    mwctx->current_image = std::make_shared<ImagePathLabel>(ir);
    mwctx->imagesVDB.push_back(mwctx->current_image);

    mwctx->centralCanvas =
        std::make_shared<CentralWorkingCanvas>(mwctx->current_image);

    auto pixbuf = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, 8, 200, 200);

    mwctx->centralCanvas->set_pixbuf(pixbuf);

    // The cursor of the pointer motions is set on the GDK window
    gtk_widget_realize(GTK_WIDGET(mwctx->wloader->get_window()->gobj()));

    handler = std::make_shared<TestableMouseDrawEventsHandler>(ops);
  }

  events::events::ImageRecordSegmentPtr segment_at(const std::size_t& index)
  {
    return helpers::SegmentPathHelper::as_segment(ir->rects.at(index));
  }

  void draw_triangle()
  {
    click(20, 20, true);
    click(80, 20, true);
    click(50, 70, true);
    release_shift();
  }

  static guint state_of(const bool& shift)
  {
    return shift ? static_cast<guint>(GDK_SHIFT_MASK) : 0U;
  }

  void press(const double& x, const double& y, const bool& shift)
  {
    GdkEventButton event{};
    event.type = GDK_BUTTON_PRESS;
    event.button = 1;
    event.x = x;
    event.y = y;
    event.state = state_of(shift);

    handler->handle_motion_start(&event);
  }

  void release(const double& x, const double& y, const bool& shift)
  {
    GdkEventButton event{};
    event.type = GDK_BUTTON_RELEASE;
    event.button = 1;
    event.x = x;
    event.y = y;
    event.state = state_of(shift);

    handler->handle_motion_end(&event);
  }

  void click(const double& x, const double& y, const bool& shift)
  {
    press(x, y, shift);
    release(x, y, shift);
  }

  void move(const double& x, const double& y, const bool& shift)
  {
    GdkEventMotion event{};
    event.type = GDK_MOTION_NOTIFY;
    event.x = x;
    event.y = y;
    event.state = state_of(shift);

    handler->handle_motion_event(&event);
  }

  void release_shift()
  {
    GdkEventKey event{};
    event.type = GDK_KEY_RELEASE;
    event.keyval = GDK_KEY_Shift_L;

    handler->handle_key_release(&event);
  }

  void lose_focus() { handler->handle_focus_out(nullptr); }
};

TEST_F(CTEST_SegmentEditing, shift_clicks_draw_the_path_the_shift_release_loops)
{
  click(20, 20, true);
  click(80, 20, true);
  click(50, 70, true);

  ASSERT_EQ(ir->rects.size(), 1U);

  auto segment = segment_at(0U);

  ASSERT_NE(segment, nullptr);
  EXPECT_EQ(ops->segment, segment);
  EXPECT_EQ(ir->current_rect, segment);
  EXPECT_EQ(mwctx->centralCanvas->get_drawn_segment(), segment);
  EXPECT_EQ(segment->points.size(), 3U);

  release_shift();

  EXPECT_EQ(ops->segment, nullptr);
  EXPECT_EQ(mwctx->centralCanvas->get_drawn_segment(), nullptr);
  ASSERT_EQ(ir->rects.size(), 1U);
  EXPECT_EQ(ir->current_rect, segment);
  EXPECT_EQ(segment->outline().size(), 3U);
  EXPECT_EQ(segment->x, 20);
  EXPECT_EQ(segment->y, 20);
  EXPECT_EQ(segment->width, 60);
  EXPECT_EQ(segment->height, 50);
}

TEST_F(CTEST_SegmentEditing, the_shift_release_drops_a_path_of_two_points)
{
  click(20, 20, true);
  click(80, 20, true);

  ASSERT_EQ(ir->rects.size(), 1U);

  release_shift();

  EXPECT_EQ(ops->segment, nullptr);
  EXPECT_TRUE(ir->rects.empty());
  EXPECT_EQ(ir->current_rect, nullptr);
}

TEST_F(CTEST_SegmentEditing, the_next_shift_clicks_draw_another_segment)
{
  draw_triangle();

  click(120, 120, true);
  click(180, 120, true);
  click(150, 170, true);
  release_shift();

  ASSERT_EQ(ir->rects.size(), 2U);
  EXPECT_NE(segment_at(0U), segment_at(1U));
  EXPECT_EQ(segment_at(1U)->x, 120);
  EXPECT_EQ(ir->current_rect, segment_at(1U));
}

TEST_F(CTEST_SegmentEditing, a_click_without_shift_finishes_the_drawn_segment)
{
  click(20, 20, true);
  click(80, 20, true);
  click(50, 70, true);

  click(150, 150, false);

  EXPECT_EQ(ops->segment, nullptr);
  ASSERT_EQ(ir->rects.size(), 2U);
  EXPECT_EQ(segment_at(0U)->outline().size(), 3U);
  EXPECT_EQ(segment_at(1U), nullptr);  // the rectangle the click draws
}

TEST_F(CTEST_SegmentEditing, a_motion_without_shift_finishes_the_drawn_segment)
{
  click(20, 20, true);
  click(80, 20, true);
  click(50, 70, true);

  move(100, 100, true);

  EXPECT_NE(ops->segment, nullptr);

  move(100, 100, false);

  EXPECT_EQ(ops->segment, nullptr);
  EXPECT_EQ(ir->rects.size(), 1U);
}

TEST_F(CTEST_SegmentEditing, losing_the_focus_finishes_the_drawn_segment)
{
  click(20, 20, true);
  click(80, 20, true);
  click(50, 70, true);

  lose_focus();

  EXPECT_EQ(ops->segment, nullptr);
  EXPECT_EQ(mwctx->centralCanvas->get_drawn_segment(), nullptr);
  EXPECT_EQ(ir->rects.size(), 1U);
}

TEST_F(CTEST_SegmentEditing, the_shift_clicks_stay_inside_the_image)
{
  click(-30, 20, true);
  click(280, 20, true);
  click(50, 270, true);
  release_shift();

  ASSERT_EQ(ir->rects.size(), 1U);

  auto segment = segment_at(0U);

  EXPECT_EQ(point_of(segment, 0U), std::make_tuple(0.0, 20.0));
  EXPECT_EQ(point_of(segment, 1U), std::make_tuple(200.0, 20.0));
  EXPECT_EQ(point_of(segment, 2U), std::make_tuple(50.0, 200.0));
}

TEST_F(CTEST_SegmentEditing, dragging_a_point_moves_it_alone)
{
  draw_triangle();

  auto segment = segment_at(0U);

  ir->current_rect.reset();

  press(81, 21, false);

  EXPECT_EQ(ops->pointSegment, segment);
  EXPECT_EQ(ops->pointIndex, 1U);
  EXPECT_EQ(ir->current_rect, segment);
  EXPECT_FALSE(ops->dragging);

  move(120, 90, false);
  release(120, 90, false);

  EXPECT_EQ(ops->pointSegment, nullptr);
  ASSERT_EQ(ir->rects.size(), 1U);
  EXPECT_EQ(point_of(segment, 0U), std::make_tuple(20.0, 20.0));
  EXPECT_EQ(point_of(segment, 1U), std::make_tuple(120.0, 90.0));
  EXPECT_EQ(point_of(segment, 2U), std::make_tuple(50.0, 70.0));
  EXPECT_EQ(segment->x, 20);
  EXPECT_EQ(segment->y, 20);
  EXPECT_EQ(segment->width, 100);
  EXPECT_EQ(segment->height, 70);
}

TEST_F(CTEST_SegmentEditing, a_dragged_point_stays_inside_the_image)
{
  draw_triangle();

  press(50, 70, false);
  move(260, -40, false);
  release(260, -40, false);

  EXPECT_EQ(point_of(segment_at(0U), 2U), std::make_tuple(200.0, 0.0));
}

TEST_F(CTEST_SegmentEditing, a_press_away_from_the_points_draws_a_rectangle)
{
  draw_triangle();

  press(50, 35, false);

  EXPECT_EQ(ops->pointSegment, nullptr);
  EXPECT_TRUE(ops->dragging);

  release(60, 45, false);

  EXPECT_EQ(ir->rects.size(), 2U);
}

TEST_F(CTEST_SegmentEditing, closing_the_project_forgets_the_drawn_segment)
{
  click(20, 20, true);

  ops->reset_segment_editing();

  EXPECT_EQ(ops->segment, nullptr);
  EXPECT_EQ(ops->segmentImage, nullptr);
  EXPECT_EQ(mwctx->centralCanvas->get_drawn_segment(), nullptr);
}
