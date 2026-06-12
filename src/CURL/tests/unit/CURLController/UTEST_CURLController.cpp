#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "src/CURL/CURLController.h"

using namespace testing;
using curli::CURLController;

TEST(UTEST_CURLController, is_url_detects_the_scheme_separator)
{
  EXPECT_TRUE(CURLController::is_url("http://example.com"));
  EXPECT_TRUE(CURLController::is_url("https://example.com/a/b"));
  EXPECT_TRUE(CURLController::is_url("ftp://host/file"));

  EXPECT_FALSE(CURLController::is_url("/local/abs/path.png"));
  EXPECT_FALSE(CURLController::is_url("relative/path.png"));
  EXPECT_FALSE(CURLController::is_url(""));
}

TEST(UTEST_CURLController, get_url_hostname_extracts_the_host)
{
  EXPECT_EQ(CURLController::get_url_hostname("http://example.com/a/b"),
            "example.com");
  EXPECT_EQ(CURLController::get_url_hostname("https://sub.host.org/x?y=1"),
            "sub.host.org");
}

TEST(UTEST_CURLController, get_absolute_url_resolves_a_relative_path)
{
  EXPECT_EQ(
      CURLController::get_absolute_url("http://example.com/dir/", "img.png"),
      "http://example.com/dir/img.png");
}

TEST(UTEST_CURLController, get_absolute_url_resolves_a_root_relative_path)
{
  EXPECT_EQ(CURLController::get_absolute_url("http://example.com/a/b", "/c.png"),
            "http://example.com/c.png");
}

TEST(UTEST_CURLController, get_absolute_url_keeps_an_absolute_rel_url)
{
  EXPECT_EQ(CURLController::get_absolute_url("http://example.com/a/b",
                                            "http://other.com/z.png"),
            "http://other.com/z.png");
}
