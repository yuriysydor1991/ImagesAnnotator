#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <memory>
#include <string>

#include "src/annotator-business/dbs/ImagesLoaders/ImagesWebPageLoader.h"

using namespace testing;
using iannotator::dbs::images::ImagesWebPageLoader;

TEST(UTEST_ImagesWebPageLoader, create_returns_a_valid_instance)
{
  EXPECT_NE(ImagesWebPageLoader::create(), nullptr);
}

TEST(UTEST_ImagesWebPageLoader, load_with_empty_path_returns_no_records)
{
  ImagesWebPageLoader loader;

  EXPECT_TRUE(loader.load("").empty());
}

TEST(UTEST_ImagesWebPageLoader, load_extracts_image_urls_from_the_page)
{
  const std::filesystem::path html =
      std::filesystem::path{testing::TempDir()} / "utest_webpage.html";

  {
    std::ofstream f(html);
    f << "<html><body>\n"
         "<img src=\"a.png\">\n"
         "<img data-src='sub/b.jpg'>\n"
         "</body></html>\n";
  }

  const std::string url = "file://" + html.string();

  ImagesWebPageLoader loader;

  auto recs = loader.load(url);

  ASSERT_EQ(recs.size(), 2U);
  EXPECT_EQ(recs[0]->path, "a.png");
  EXPECT_EQ(recs[0]->abs_dir_path, url);
  EXPECT_EQ(recs[1]->path, "sub/b.jpg");
  EXPECT_EQ(recs[1]->abs_dir_path, url);

  std::filesystem::remove(html);
}
