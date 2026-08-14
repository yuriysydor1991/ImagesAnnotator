#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <fstream>
#include <functional>
#include <memory>
#include <set>
#include <string>
#include <thread>
#include <type_traits>

#include "src/log/ILogger.h"
#include "src/log/default-logger/DefaultLogger.h"
#include "src/log/default-logger/real-default-logger/RealDefaultLogger.h"

using namespace testing;
using namespace default_logger;

class CTEST_DefaultLogger : public Test
{
 public:
  inline static const std::string test_default_log_file =
      CTEST_DefaultLogger_DATA_DIR "/CTEST_DefaultLogger.log";
  inline static const unsigned short& default_log_level =
      DefaultLogger::LVL_TRACE;

  CTEST_DefaultLogger() { clear_default_log_file(); }
  ~CTEST_DefaultLogger() { clear_default_log_file(); }

  void default_log_lvl_init(const unsigned short& lvl)
  {
    DefaultLogger::init(test_default_log_file, lvl, true);
  }

  void default_log_init(const bool print = true)
  {
    DefaultLogger::init(test_default_log_file, default_log_level, print);
  }

  std::string get_default_log_file_contents()
  {
    std::fstream log{test_default_log_file.c_str(), std::fstream::in};

    std::string rt;
    char c{0};

    while (log.get(c)) {
      rt += c;
    }

    log.close();

    return rt;
  }

  void clear_default_log_file()
  {
    std::fstream log{test_default_log_file.c_str(),
                     std::fstream::out | std::fstream::trunc};

    log.close();
  }

  const std::set<unsigned short> levelsSet{
      DefaultLogger::LVL_ERROR, DefaultLogger::LVL_WARNING,
      DefaultLogger::LVL_INFO, DefaultLogger::LVL_DEBUG,
      DefaultLogger::LVL_TRACE};
};

TEST_F(CTEST_DefaultLogger, nonempty_defaultLogDateFormat)
{
  EXPECT_FALSE(std::string{DefaultLogger::defaultLogDateFormat}.empty());
}

TEST_F(CTEST_DefaultLogger, nonempty_default_log_name)
{
  EXPECT_FALSE(DefaultLogger::default_log_name.empty());
}

TEST_F(CTEST_DefaultLogger, no_equal_levels)
{
  EXPECT_GE(levelsSet.size(), 5U);

  for (auto iter = levelsSet.begin(); iter != levelsSet.end(); ++iter) {
    auto cval = *iter;
    auto next = iter;
    for (auto jter = ++next; jter != levelsSet.end(); ++jter) {
      EXPECT_NE(*jter, cval);
    }
  }
}

TEST_F(CTEST_DefaultLogger, first_error_log)
{
  static const std::string test_str = "first log test 0123-";

  default_log_init();

  DefaultLogger::log(DefaultLogger::LVL_ERROR, test_str);

  const auto logs = get_default_log_file_contents();

  EXPECT_FALSE(logs.empty());

  EXPECT_THAT(logs, EndsWith(test_str + "\n"));
}

TEST_F(CTEST_DefaultLogger, trace_with_max_debug_log_absent)
{
  default_log_lvl_init(DefaultLogger::LVL_DEBUG);

  DefaultLogger::log(DefaultLogger::LVL_TRACE, "Expect absent");

  const auto logs = get_default_log_file_contents();

  EXPECT_TRUE(logs.empty());
  EXPECT_EQ(logs, std::string{});
}

TEST_F(CTEST_DefaultLogger, debug_with_max_info_log_absent)
{
  default_log_lvl_init(DefaultLogger::LVL_INFO);

  DefaultLogger::log(DefaultLogger::LVL_DEBUG, "Expect absent");

  const auto logs = get_default_log_file_contents();

  EXPECT_TRUE(logs.empty());
  EXPECT_EQ(logs, std::string{});
}

TEST_F(CTEST_DefaultLogger, info_with_max_warning_log_absent)
{
  default_log_lvl_init(DefaultLogger::LVL_WARNING);

  DefaultLogger::log(DefaultLogger::LVL_INFO, "Expect absent");

  const auto logs = get_default_log_file_contents();

  EXPECT_TRUE(logs.empty());
  EXPECT_EQ(logs, std::string{});
}

TEST_F(CTEST_DefaultLogger, warning_with_max_error_log_absent)
{
  default_log_lvl_init(DefaultLogger::LVL_ERROR);

  DefaultLogger::log(DefaultLogger::LVL_WARNING, "Expect absent");

  const auto logs = get_default_log_file_contents();

  EXPECT_TRUE(logs.empty());
  EXPECT_EQ(logs, std::string{});
}

TEST_F(CTEST_DefaultLogger, warning_with_max_info_log_present)
{
  static const std::string expect = "expect present";

  default_log_lvl_init(DefaultLogger::LVL_INFO);

  DefaultLogger::log(DefaultLogger::LVL_WARNING, expect);

  const auto logs = get_default_log_file_contents();

  EXPECT_FALSE(logs.empty());
  EXPECT_THAT(logs, EndsWith(expect + "\n"));
}

TEST_F(CTEST_DefaultLogger, error_with_max_warning_log_present)
{
  static const std::string expect = "expect present";

  default_log_lvl_init(DefaultLogger::LVL_WARNING);

  DefaultLogger::log(DefaultLogger::LVL_ERROR, expect);

  const auto logs = get_default_log_file_contents();

  EXPECT_FALSE(logs.empty());
  EXPECT_THAT(logs, EndsWith(expect + "\n"));
}

namespace
{

/**
 * @brief The real logger substitution which records the very last log call
 * instead of the actual logging.
 */
class RecordingRealLogger : public RealDefaultLogger
{
 public:
  using RealDefaultLogger::log;

  void log(const unsigned short& loglvl, const std::string& msg) override
  {
    lastLvl = loglvl;
    lastMsg = msg;
  }

  unsigned short lastLvl{0U};
  std::string lastMsg;
};

/**
 * @brief The logger::ILogger implementation which is not a RealDefaultLogger
 * descendant at all. It records the very last call of every interface method
 * instead of the actual logging, the very same way a log4cpp or a boost::log
 * wrapper would forward those calls into it's own logging library.
 */
class RecordingLogger : public logger::ILogger
{
 public:
  inline static const std::string lvlRepr{"REC"};

  void log(const unsigned short& loglvl, const std::string& msg) override
  {
    lastLvl = loglvl;
    lastMsg = msg;
  }

  void log(const unsigned short& loglvl, const char* const filePath,
           const int& fileLine, const std::string& msg) override
  {
    lastFilePath = filePath;
    lastFileLine = fileLine;

    log(loglvl, msg);
  }

  void logfile(const std::string& filepath) override { lastLogfile = filepath; }

  void print(const bool toPrintValue) override { lastPrint = toPrintValue; }

  void level(const unsigned short& nlvl) override { lastLevel = nlvl; }

  const std::string& lvl_repr(const unsigned short& glvl) override
  {
    lastReprLvl = glvl;

    return lvlRepr;
  }

  void init(const std::string& filepath, const unsigned short& nlvl,
            const bool toPrintValue) override
  {
    logfile(filepath);
    level(nlvl);
    print(toPrintValue);
  }

  unsigned short lastLvl{0U};
  std::string lastMsg;
  std::string lastFilePath;
  int lastFileLine{0};
  std::string lastLogfile;
  bool lastPrint{false};
  unsigned short lastLevel{0U};
  unsigned short lastReprLvl{0U};
};

}  // namespace

/**
 * @brief The DefaultLogger real logger instance holder fixture. It restores the
 * original real logger instance, so the rest of the tests stay unaffected.
 */
class CTEST_DefaultLoggerRealLogger : public Test
{
 public:
  ~CTEST_DefaultLoggerRealLogger()
  {
    DefaultLogger::real_logger(originalRealLogger);
  }

  const DefaultLogger::RealLoggerPtr originalRealLogger{
      DefaultLogger::real_logger()};
};

TEST_F(CTEST_DefaultLoggerRealLogger, real_logger_is_never_null)
{
  EXPECT_NE(DefaultLogger::real_logger(), nullptr);
}

TEST_F(CTEST_DefaultLoggerRealLogger, real_logger_instance_stays_the_same)
{
  EXPECT_EQ(DefaultLogger::real_logger(), DefaultLogger::real_logger());
}

TEST_F(CTEST_DefaultLoggerRealLogger, null_real_logger_is_ignored)
{
  DefaultLogger::real_logger(nullptr);

  EXPECT_EQ(DefaultLogger::real_logger(), originalRealLogger);
}

TEST_F(CTEST_DefaultLoggerRealLogger, given_real_logger_is_marked_as_adopted)
{
  DefaultLogger::real_logger(std::make_shared<RecordingRealLogger>());

  EXPECT_TRUE(DefaultLogger::real_logger_adopted());
}

TEST_F(CTEST_DefaultLoggerRealLogger, given_real_logger_receives_the_logs)
{
  static const std::string expect = "the given real logger message";

  const auto givenLogger = std::make_shared<RecordingRealLogger>();

  DefaultLogger::real_logger(givenLogger);

  EXPECT_EQ(DefaultLogger::real_logger(), givenLogger);

  DefaultLogger::log(DefaultLogger::LVL_ERROR, expect);

  EXPECT_EQ(givenLogger->lastLvl, DefaultLogger::LVL_ERROR);
  EXPECT_EQ(givenLogger->lastMsg, expect);
}

TEST_F(CTEST_DefaultLoggerRealLogger, given_real_logger_receives_the_file_logs)
{
  static const std::string expect = "the given real logger file message";

  const auto givenLogger = std::make_shared<RecordingRealLogger>();

  DefaultLogger::real_logger(givenLogger);

  DefaultLogger::log(DefaultLogger::LVL_ERROR, __FILE__, __LINE__, expect);

  EXPECT_EQ(givenLogger->lastLvl, DefaultLogger::LVL_ERROR);
  EXPECT_THAT(givenLogger->lastMsg, EndsWith(expect));
}

TEST_F(CTEST_DefaultLoggerRealLogger, real_logger_is_held_by_the_interface_ptr)
{
  EXPECT_TRUE(std::is_abstract_v<logger::ILogger>);
  EXPECT_TRUE((std::is_same_v<DefaultLogger::RealLoggerPtr,
                              std::shared_ptr<logger::ILogger>>));
  EXPECT_TRUE((std::is_base_of_v<logger::ILogger, RealDefaultLogger>));
}

TEST_F(CTEST_DefaultLoggerRealLogger, given_interface_logger_is_accepted)
{
  const auto givenLogger = std::make_shared<RecordingLogger>();

  DefaultLogger::real_logger(givenLogger);

  EXPECT_EQ(DefaultLogger::real_logger(), givenLogger);
  EXPECT_TRUE(DefaultLogger::real_logger_adopted());
}

TEST_F(CTEST_DefaultLoggerRealLogger, given_interface_logger_receives_the_logs)
{
  static const std::string expect = "the given interface logger message";

  const auto givenLogger = std::make_shared<RecordingLogger>();

  DefaultLogger::real_logger(givenLogger);

  DefaultLogger::log(DefaultLogger::LVL_ERROR, expect);

  EXPECT_EQ(givenLogger->lastLvl, DefaultLogger::LVL_ERROR);
  EXPECT_EQ(givenLogger->lastMsg, expect);
}

TEST_F(CTEST_DefaultLoggerRealLogger,
       given_interface_logger_receives_the_file_logs)
{
  static const std::string expect = "the given interface logger file message";
  static constexpr const char* const expectedFile = "the-caller-file.cpp";
  static constexpr const int expectedLine = 42;

  const auto givenLogger = std::make_shared<RecordingLogger>();

  DefaultLogger::real_logger(givenLogger);

  DefaultLogger::log(DefaultLogger::LVL_WARNING, expectedFile, expectedLine,
                     expect);

  EXPECT_EQ(givenLogger->lastLvl, DefaultLogger::LVL_WARNING);
  EXPECT_EQ(givenLogger->lastMsg, expect);
  EXPECT_EQ(givenLogger->lastFilePath, expectedFile);
  EXPECT_EQ(givenLogger->lastFileLine, expectedLine);
}

TEST_F(CTEST_DefaultLoggerRealLogger,
       given_interface_logger_receives_the_settings)
{
  static const std::string expect = "the-given-interface-logger.log";

  const auto givenLogger = std::make_shared<RecordingLogger>();

  DefaultLogger::real_logger(givenLogger);

  DefaultLogger::init(expect, DefaultLogger::LVL_DEBUG, false);

  EXPECT_EQ(givenLogger->lastLogfile, expect);
  EXPECT_EQ(givenLogger->lastLevel, DefaultLogger::LVL_DEBUG);
  EXPECT_FALSE(givenLogger->lastPrint);
}

TEST_F(CTEST_DefaultLoggerRealLogger, given_interface_logger_gives_the_lvl_repr)
{
  const auto givenLogger = std::make_shared<RecordingLogger>();

  DefaultLogger::real_logger(givenLogger);

  EXPECT_EQ(DefaultLogger::lvl_repr(DefaultLogger::LVL_TRACE),
            RecordingLogger::lvlRepr);
  EXPECT_EQ(givenLogger->lastReprLvl, DefaultLogger::LVL_TRACE);
}

TEST_F(CTEST_DefaultLogger, multithread_logs)
{
  static constexpr const unsigned int iters_per_thread = 1000U;

  default_log_init(false);

  std::set<std::shared_ptr<std::thread>> ths;

  const auto concurencyLog = []() {
    for (auto jter = 0U; jter < iters_per_thread; ++jter) {
      DefaultLogger::log(DefaultLogger::LVL_ERROR,
                         "Concurency thread log msg #" + std::to_string(jter));
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

  auto logs = get_default_log_file_contents();

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
