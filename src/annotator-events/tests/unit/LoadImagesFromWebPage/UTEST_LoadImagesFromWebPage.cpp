#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "src/annotator-events/events/LoadImagesFromWebPage.h"

using namespace testing;
using events::events::LoadImagesFromWebPage;

TEST(UTEST_LoadImagesFromWebPage, constructor_stores_the_value)
{
  LoadImagesFromWebPage e{"http://x/page"};

  EXPECT_EQ(e.web_page_url, "http://x/page");
}

TEST(UTEST_LoadImagesFromWebPage, create_returns_a_populated_event)
{
  auto e = LoadImagesFromWebPage::create("http://x/page");

  ASSERT_NE(e, nullptr);
  EXPECT_EQ(e->web_page_url, "http://x/page");
}
