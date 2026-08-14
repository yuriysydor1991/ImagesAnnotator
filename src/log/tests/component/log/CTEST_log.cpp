#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <fstream>
#include <memory>
#include <set>
#include <string>
#include <thread>

#include "src/log/log.h"
#include "src/log/severity-macro-consts.h"

using namespace testing;

/**
 * @brief Component test for the top-level log.h facilities (the LOG_INIT and
 * LOGx macros), exercised through the public facade only - it never names a
 * concrete logger implementation, so the same source can be reused on the
 * branches that swap the underlying logger (appLog4Cpp5, appBoostBeast, ...).
 *
 * The runtime level (the second LOG_INIT argument) is driven through the
 * branch-agnostic MACRO_LVL_* severities. Presence is asserted only for ERROR
 * and WARNING messages, because the facade exposes no flush and a logger is
 * only required to flush at the WARNING severity or above; the lower severities
 * are exercised through their absence when the runtime level drops them, which
 * needs no flush.
 */
class CTEST_log : public Test
{
 public:
  inline static const std::string test_log_file =
      CTEST_log_DATA_DIR "/CTEST_log.log";

  CTEST_log() { clear_log_file(); }
  ~CTEST_log() { clear_log_file(); }

  void init(const unsigned short& level, const bool print = true)
  {
    LOG_INIT(test_log_file, level, print);
  }

  std::string log_contents()
  {
    std::fstream log{test_log_file.c_str(), std::fstream::in};

    std::string rt;
    char c{0};

    while (log.get(c)) {
      rt += c;
    }

    log.close();

    return rt;
  }

  void clear_log_file()
  {
    std::fstream log{test_log_file.c_str(),
                     std::fstream::out | std::fstream::trunc};

    log.close();
  }
};

TEST_F(CTEST_log, error_message_is_written)
{
  static const std::string expect = "facade error message 0123-";

  init(MACRO_LVL_ERROR);

  LOGE(expect);

  const auto logs = log_contents();

  EXPECT_FALSE(logs.empty());
  EXPECT_THAT(logs, EndsWith(expect + "\n"));
}

TEST_F(CTEST_log, warning_is_dropped_when_runtime_level_is_error)
{
  init(MACRO_LVL_ERROR);

  LOGW("must be dropped at the error runtime level");

  EXPECT_TRUE(log_contents().empty());
}

TEST_F(CTEST_log, warning_is_written_when_runtime_level_is_warning)
{
  static const std::string expect = "facade warning is present";

  init(MACRO_LVL_WARNING);

  LOGW(expect);

  EXPECT_THAT(log_contents(), EndsWith(expect + "\n"));
}

TEST_F(CTEST_log, info_is_dropped_when_runtime_level_is_warning)
{
  init(MACRO_LVL_WARNING);

  LOGI("must be dropped at the warning runtime level");

  EXPECT_TRUE(log_contents().empty());
}

TEST_F(CTEST_log, debug_is_dropped_when_runtime_level_is_info)
{
  init(MACRO_LVL_INFO);

  LOGD("must be dropped at the info runtime level");

  EXPECT_TRUE(log_contents().empty());
}

TEST_F(CTEST_log, trace_is_dropped_when_runtime_level_is_debug)
{
  init(MACRO_LVL_DEBUG);

  LOGT("must be dropped at the debug runtime level");

  EXPECT_TRUE(log_contents().empty());
}

TEST_F(CTEST_log, multithread_error_logs)
{
  static constexpr const unsigned int iters_per_thread = 1000U;

  init(MACRO_LVL_ERROR, false);

  std::set<std::shared_ptr<std::thread>> ths;

  const auto concurencyLog = []() {
    for (auto jter = 0U; jter < iters_per_thread; ++jter) {
      LOGE("Concurency thread log msg #" + std::to_string(jter));
    }
  };

  for (auto iter = 0U; iter < std::thread::hardware_concurrency(); ++iter) {
    ths.insert(std::make_shared<std::thread>(concurencyLog));
  }

  for (auto& th : ths) {
    if (th->joinable()) {
      th->join();
    }
  }

  ths.clear();

  auto logs = log_contents();

  const unsigned int expectedLogsLines =
      std::thread::hardware_concurrency() * iters_per_thread;

  const auto newlines = std::count_if(logs.begin(), logs.end(),
                                      [](const char& c) { return c == '\n'; });

  EXPECT_EQ(newlines, expectedLogsLines);

  while (!logs.empty()) {
    auto lineEnd = std::find(logs.begin(), logs.end(), '\n');

    std::string line{logs.begin(), lineEnd};

    if (lineEnd < logs.end()) {
      lineEnd++;
    }

    logs.erase(logs.begin(), lineEnd);

    EXPECT_THAT(line, ContainsRegex(" Concurency thread log msg #[0-9]{1,3}$"));
  }
}
