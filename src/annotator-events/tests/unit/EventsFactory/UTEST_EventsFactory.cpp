#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "src/annotator-events/events/EventsFactory.h"
#include "src/annotator-events/events/ImageRecord.h"
#include "src/annotator-events/events/ImageRecordRect.h"

using namespace testing;
using namespace events::events;

namespace
{

std::shared_ptr<EventsFactory> factory()
{
  return std::make_shared<EventsFactory>();
}

}  // namespace

TEST(UTEST_EventsFactory, instance_returns_a_stable_singleton)
{
  auto a = EventsFactory::instance();
  auto b = EventsFactory::instance();

  ASSERT_NE(a, nullptr);
  EXPECT_EQ(a.get(), b.get());
}

TEST(UTEST_EventsFactory, create_annotations_dir_changed_carries_the_path)
{
  auto e = factory()->create_annotations_dir_changed("/db.json");

  ASSERT_NE(e, nullptr);
  EXPECT_EQ(e->annotations_dir, "/db.json");
}

TEST(UTEST_EventsFactory, create_annotations_dir_changed_rejects_empty_path)
{
  EXPECT_EQ(factory()->create_annotations_dir_changed(""), nullptr);
}

TEST(UTEST_EventsFactory, create_image_dir_changed_carries_the_path)
{
  auto e = factory()->create_image_dir_changed("/imgs");

  ASSERT_NE(e, nullptr);
  EXPECT_EQ(e->images_dir, "/imgs");
}

TEST(UTEST_EventsFactory, create_image_dir_changed_rejects_empty_path)
{
  EXPECT_EQ(factory()->create_image_dir_changed(""), nullptr);
}

TEST(UTEST_EventsFactory, create_image_dir_object_request_is_created)
{
  EXPECT_NE(factory()->create_image_dir_object_request(), nullptr);
}

TEST(UTEST_EventsFactory, create_images_dir_provider_changed_rejects_null)
{
  EXPECT_EQ(factory()->create_images_dir_provider_changed(nullptr), nullptr);
}

TEST(UTEST_EventsFactory, create_current_image_changed_carries_the_record)
{
  auto ir = ImageRecord::create("a.png", "/d");

  auto e = factory()->create_current_image_changed(ir);

  ASSERT_NE(e, nullptr);
  EXPECT_EQ(e->new_current_image, ir);
}

TEST(UTEST_EventsFactory, create_current_image_changed_rejects_null)
{
  EXPECT_EQ(factory()->create_current_image_changed(nullptr), nullptr);
}

TEST(UTEST_EventsFactory, create_image_rect_record_uses_default_name_without_owner)
{
  auto r = factory()->create_image_rect_record(nullptr);

  ASSERT_NE(r, nullptr);
  EXPECT_EQ(r->name, EventsFactory::default_name_record);
}

TEST(UTEST_EventsFactory, create_image_rect_record_numbers_name_by_owner_rects)
{
  auto ir = ImageRecord::create("a.png", "/d");
  ir->rects.emplace_back(std::make_shared<ImageRecordRect>());
  ir->rects.emplace_back(std::make_shared<ImageRecordRect>());

  auto r = factory()->create_image_rect_record(ir);

  ASSERT_NE(r, nullptr);
  EXPECT_EQ(r->name,
            EventsFactory::default_numbered_name_record_prefix + "2");
}

TEST(UTEST_EventsFactory, create_image_rect_record_with_explicit_geometry)
{
  auto r = factory()->create_image_rect_record("dog", 1, 2, 3, 4);

  ASSERT_NE(r, nullptr);
  EXPECT_EQ(r->name, "dog");
  EXPECT_EQ(r->x, 1);
  EXPECT_EQ(r->y, 2);
  EXPECT_EQ(r->width, 3);
  EXPECT_EQ(r->height, 4);
}

TEST(UTEST_EventsFactory, create_image_record_carries_path_and_abs_dir)
{
  auto ir = factory()->create_image_record("rel.png", "/abs");

  ASSERT_NE(ir, nullptr);
  EXPECT_EQ(ir->path, "rel.png");
  EXPECT_EQ(ir->abs_dir_path, "/abs");
}

TEST(UTEST_EventsFactory, create_store_request_carries_the_db_path)
{
  EXPECT_EQ(factory()->create_store_request("/db.json")->dbpath, "/db.json");
  EXPECT_EQ(factory()->create_store_request()->dbpath, "");
}

TEST(UTEST_EventsFactory, create_close_event_is_created)
{
  EXPECT_NE(factory()->create_close_event(), nullptr);
}

TEST(UTEST_EventsFactory, create_plain_txt_2_folder_export_carries_folder)
{
  EXPECT_EQ(
      factory()->create_plain_txt_2_folder_export("/out")->dst_folder_path,
      "/out");
}

TEST(UTEST_EventsFactory, create_yolo4_export_request_carries_folder)
{
  EXPECT_EQ(factory()->create_yolo4_export_request("/out")->dst_folder_path,
            "/out");
}

TEST(UTEST_EventsFactory, create_pytorch_vision_export_request_carries_folder)
{
  EXPECT_EQ(
      factory()->create_pytorch_vision_export_request("/out")->dst_folder_path,
      "/out");
}

TEST(UTEST_EventsFactory, create_delete_current_image_request_carries_path)
{
  EXPECT_EQ(factory()
                ->create_delete_current_image_request("/d/a.png")
                ->image_full_path,
            "/d/a.png");
}

TEST(UTEST_EventsFactory, create_images_web_page_load_event_carries_url)
{
  EXPECT_EQ(
      factory()->create_images_web_page_load_event("http://x")->web_page_url,
      "http://x");
}

TEST(UTEST_EventsFactory, create_display_error_event_carries_description)
{
  EXPECT_EQ(factory()->create_display_error_event("boom")->description, "boom");
}
