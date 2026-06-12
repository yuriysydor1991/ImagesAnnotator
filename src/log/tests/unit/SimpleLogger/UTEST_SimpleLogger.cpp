#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "src/log/simple-logger/SimpleLogger.h"

using namespace testing;
using simple_logger::SimpleLogger;

TEST(UTEST_SimpleLogger, lvl_repr_maps_each_level_to_its_tag)
{
  EXPECT_EQ(SimpleLogger::lvl_repr(SimpleLogger::LVL_ERROR), "ERR");
  EXPECT_EQ(SimpleLogger::lvl_repr(SimpleLogger::LVL_WARNING), "WRN");
  EXPECT_EQ(SimpleLogger::lvl_repr(SimpleLogger::LVL_INFO), "INF");
  EXPECT_EQ(SimpleLogger::lvl_repr(SimpleLogger::LVL_DEBUG), "DBG");
  EXPECT_EQ(SimpleLogger::lvl_repr(SimpleLogger::LVL_TRACE), "TRA");
}

TEST(UTEST_SimpleLogger, lvl_repr_returns_unknown_for_out_of_range_levels)
{
  EXPECT_EQ(SimpleLogger::lvl_repr(0U), "UNK");
  EXPECT_EQ(SimpleLogger::lvl_repr(99U), "UNK");
}

TEST(UTEST_SimpleLogger, error_and_warning_are_printed_to_stderr)
{
  SimpleLogger::level(SimpleLogger::LVL_TRACE);
  SimpleLogger::print(true);

  internal::CaptureStderr();
  SimpleLogger::log(SimpleLogger::LVL_ERROR, "an error message");
  SimpleLogger::log(SimpleLogger::LVL_WARNING, "a warning message");
  const std::string err = internal::GetCapturedStderr();

  EXPECT_NE(err.find("an error message"), std::string::npos);
  EXPECT_NE(err.find("ERR"), std::string::npos);
  EXPECT_NE(err.find("a warning message"), std::string::npos);
  EXPECT_NE(err.find("WRN"), std::string::npos);
}

TEST(UTEST_SimpleLogger, info_is_printed_to_stdout_with_its_tag)
{
  SimpleLogger::level(SimpleLogger::LVL_TRACE);
  SimpleLogger::print(true);

  internal::CaptureStdout();
  SimpleLogger::log(SimpleLogger::LVL_INFO, "an info message");
  const std::string out = internal::GetCapturedStdout();

  EXPECT_NE(out.find("an info message"), std::string::npos);
  EXPECT_NE(out.find("INF"), std::string::npos);
}

TEST(UTEST_SimpleLogger, messages_above_the_threshold_are_dropped)
{
  SimpleLogger::level(SimpleLogger::LVL_WARNING);
  SimpleLogger::print(true);

  internal::CaptureStdout();
  SimpleLogger::log(SimpleLogger::LVL_INFO, "filtered info message");
  const std::string out = internal::GetCapturedStdout();

  EXPECT_EQ(out.find("filtered info message"), std::string::npos);
}
