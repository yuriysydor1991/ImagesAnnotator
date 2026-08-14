/**
 * The "Simplified BSD License"
 *
 * Copyright (c) 2025, Yurii Sydor (yuriysydor1991@gmail.com)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   - Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_REAL_DEFAULT_LOGGER_IMPLEMENTATION_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_REAL_DEFAULT_LOGGER_IMPLEMENTATION_CLASS_H

#include <atomic>
#include <fstream>
#include <mutex>
#include <sstream>
#include <string>

#include "project-global-decls.h"
#include "src/log/ILogger.h"
#include "src/log/severity-macro-consts.h"

/**
 * @brief The default logger quick implementation encapsulation namespace.
 */
namespace default_logger
{

/**
 * @brief The real default logger implementation which holds the actual logging
 * code and the whole logger state. It logs into a plain text file and,
 * optionally, duplicates every message into the stdout or the stderr stream.
 *
 * The default_logger::DefaultLogger static methods are just a proxy which
 * forwards every call into an instance of the logger::ILogger interface, which
 * this class implements and which the DefaultLogger creates by default.
 * Implement the logger::ILogger on your own, or inherit this class, and pass a
 * new instance into the default_logger::DefaultLogger::real_logger setter to
 * replace the logging behaviour without touching the logging macroses. Replace
 * it with log4cpp or similar if necessary.
 */
class RealDefaultLogger : public logger::ILogger
{
 public:
  inline static constexpr const char* const defaultLogDateFormat =
      "%Y-%m-%d %H:%M:%S";

  inline static const std::string default_log_name =
      project_decls::PROJECT_NAME + "-" + project_decls::PROJECT_BUILD_VERSION +
      ".log";

  ~RealDefaultLogger() override = default;
  RealDefaultLogger() = default;

  /**
   * @brief Appends the message into the opened log file and prints it into the
   * stdout or the stderr stream. See the logger::ILogger interface.
   *
   * @param loglvl The level of the log message. The lower the value - more
   * urgent the message.
   * @param msg The actual message for the log file or just stdout stream.
   */
  virtual void log(const unsigned short& loglvl,
                   const std::string& msg) override;

  /**
   * @brief Prepends the message with the source file name and line and logs it
   * through the RealDefaultLogger::log two argument overload. See the
   * logger::ILogger interface.
   *
   * @param loglvl The level of the log message. The lower the value - more
   * urgent the message.
   * @param filePath The file path of the source of the log message. Usually
   * it's the __FILE__ macro value.
   * @param fileLine The file line of the source of the log message. Usually
   * it's the __LINE__ macro value.
   * @param msg The actual message for the log file or just stdout stream.
   */
  virtual void log(const unsigned short& loglvl, const char* const filePath,
                   const int& fileLine, const std::string& msg) override;

  /**
   * @brief Closes the previously opened log file, if any, and opens the given
   * one in the append mode. See the logger::ILogger interface.
   *
   * @param filepath The filepath where the log messages will be appended. If
   * empty string - nothing will be performed.
   */
  virtual void logfile(const std::string& filepath) override;

  /**
   * @brief Sets if the logger should print each log message into the stdout.
   * See the logger::ILogger interface.
   *
   * @param toPrintValue New printing boolean value. If the true boolean value
   * given - all the log messages will be duplicated into the stdout.
   */
  virtual void print(const bool toPrintValue) override;

  /**
   * @brief Sets the maximum level of log message storage or printing. Above
   * given level all the log messages will be discarded. See the logger::ILogger
   * interface.
   *
   * @param nlvl The new maximum level of log messages. Defaults to
   * RealDefaultLogger::LVL_INFO
   */
  virtual void level(const unsigned short& nlvl) override;

  /**
   * @brief Searches for the string representation of the given log level. See
   * the logger::ILogger interface.
   *
   * @param glvl The numerical log level for it's representation to search.
   *
   * @return Returns non-empty string representation of the given lvl.
   */
  virtual const std::string& lvl_repr(const unsigned short& glvl) override;

  /**
   * @brief Init the real logger instance. See the logger::ILogger interface.
   *
   * @param filepath The file path where all the generated by the implemented
   * application messages will be stored in the filesystem. If non empty
   * filepath given and fail to open the file by path - exception is generated.
   * @param nlvl The new maximum log messages level. See the
   * RealDefaultLogger::level method.
   * @param toPrintValue Defines if the logger should print messages to the
   * stdout stream. See the RealDefaultLogger::print for more details.
   */
  virtual void init(const std::string& filepath, const unsigned short& nlvl,
                    const bool toPrintValue) override;

  /**
   * @brief Builds the full log file path for the given log file name.
   *
   * @param logname The log file name to append to the default log directory.
   *
   * @return Returns the full log file path.
   */
  static std::string get_full_log_path(const std::string& logname);

  /**
   * @brief Builds the full log file path for the
   * RealDefaultLogger::default_log_name log file name.
   *
   * @return Returns the default full log file path.
   */
  static std::string get_default_full_log_path();

 private:
  /**
   * @brief Method to define current log message timestamp.
   * Not a thread safe.
   *
   * @param oss The stringstream which will accept the current time
   * stamp for the log message.
   */
  static void insert_current_timestamp(std::ostringstream& oss);

  std::atomic_bool toPrintMsgs{true};
  std::fstream alogfile;
  std::mutex alogfile_m;
  unsigned short lvl{MAX_LOG_LEVEL};
};

}  // namespace default_logger

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_REAL_DEFAULT_LOGGER_IMPLEMENTATION_CLASS_H
