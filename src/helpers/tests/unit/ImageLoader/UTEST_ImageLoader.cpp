#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <memory>
#include <string>

#include "src/annotator-events/events/ImageRecord.h"
#include "src/helpers/ImageLoader.h"

using namespace testing;
using events::events::ImageRecord;
using helpers::ImageLoader;

TEST(UTEST_ImageLoader, create_returns_a_valid_instance)
{
  EXPECT_NE(ImageLoader::create(), nullptr);
}

TEST(UTEST_ImageLoader, load_local_record_succeeds_without_downloading)
{
  auto loader = ImageLoader::create();
  auto ir = ImageRecord::create("a.png", "/local/dir");

  EXPECT_TRUE(loader->load(ir));
  EXPECT_TRUE(ir->tmppath.empty());
}

TEST(UTEST_ImageLoader, load_short_circuits_on_an_existing_tmp_path)
{
  const auto tmp =
      std::filesystem::path{testing::TempDir()} / "utest_imageloader_cache.bin";
  {
    std::ofstream f(tmp);
    f << "cached image bytes";
  }

  auto loader = ImageLoader::create();
  auto ir = ImageRecord::create("img.png", "http://host/");
  ir->tmppath = tmp;

  EXPECT_TRUE(loader->load(ir));

  std::filesystem::remove(tmp);
}

TEST(UTEST_ImageLoader, load_null_record_fails)
{
  auto loader = ImageLoader::create();

  EXPECT_FALSE(loader->load(nullptr));
}

TEST(UTEST_ImageLoader, get_cache_top_level_is_a_non_empty_absolute_path)
{
  const auto p = ImageLoader::get_cache_top_level();

  EXPECT_FALSE(p.empty());
  EXPECT_TRUE(p.is_absolute());
}
