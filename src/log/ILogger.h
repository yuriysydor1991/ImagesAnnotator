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

#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_ILOGGER_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_ILOGGER_CLASS_H

#include <memory>
#include <string>

#include "src/log/severity-macro-consts.h"

/**
 * @brief The implementation independent part of the logging subsystem
 * encapsulation namespace.
 */
namespace logger
{

/**
 * @brief An abstract class to define an interface for the logger
 * implementation variants. Implement it to replace the logging behaviour
 * without touching the src/log/log.h logging macroses.
 *
 * The default_logger::DefaultLogger static proxy holds a pointer to this
 * interface and not to a particular logger class, so an implementation is free
 * to wrap the log4cpp, the boost::log or any other logging library instead of
 * deriving from the default_logger::RealDefaultLogger. Hand a new instance over
 * to the default_logger::DefaultLogger::real_logger setter, or through the
 * LOG_INIT_REAL_LOGGER macro, to make the whole binary log through it.
 */
class ILogger
{
 public:
  /**
   * @brief The logger instance holder type.
   */
  using ILoggerPtr = std::shared_ptr<ILogger>;

  inline static constexpr const unsigned short LVL_ERROR = MACRO_LVL_ERROR;
  inline static constexpr const unsigned short LVL_WARNING = MACRO_LVL_WARNING;
  inline static constexpr const unsigned short LVL_INFO = MACRO_LVL_INFO;
  inline static constexpr const unsigned short LVL_DEBUG = MACRO_LVL_DEBUG;
  inline static constexpr const unsigned short LVL_TRACE = MACRO_LVL_TRACE;

  virtual ~ILogger() = default;
  ILogger() = default;

  /**
   * @brief Perform a logging message.
   *
   * @param loglvl The level of the log message. The lower the value - more
   * urgent the message.
   * @param msg The actual message for the log file or just stdout stream.
   */
  virtual void log(const unsigned short& loglvl, const std::string& msg) = 0;

  /**
   * @brief Perform a logging message.
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
                   const int& fileLine, const std::string& msg) = 0;

  /**
   * @brief Sets the new logfile file path.
   *
   * @param filepath The filepath where the log messages will be appended. If
   * empty string - nothing will be performed.
   */
  virtual void logfile(const std::string& filepath) = 0;

  /**
   * @brief Sets if the logger should print each log message into the stdout.
   *
   * @param toPrintValue New printing boolean value. If the true boolean value
   * given - all the log messages will be duplicated into the stdout.
   */
  virtual void print(const bool toPrintValue) = 0;

  /**
   * @brief Sets the maximum level of log message storage or printing. Above
   * given level all the log messages will be discarded.
   *
   * @param nlvl The new maximum level of log messages.
   */
  virtual void level(const unsigned short& nlvl) = 0;

  /**
   * @brief Searches for the string representation of the given log level.
   *
   * @param glvl The numerical log level for it's representation to search.
   *
   * @return Returns non-empty string representation of the given lvl.
   */
  virtual const std::string& lvl_repr(const unsigned short& glvl) = 0;

  /**
   * @brief Init the logger instance.
   *
   * @param filepath The file path where all the generated by the implemented
   * application messages will be stored in the filesystem. If non empty
   * filepath given and fail to open the file by path - exception is generated.
   * @param nlvl The new maximum log messages level. See the ILogger::level
   * method.
   * @param toPrintValue Defines if the logger should print messages to the
   * stdout stream. See the ILogger::print for more details.
   */
  virtual void init(const std::string& filepath, const unsigned short& nlvl,
                    const bool toPrintValue) = 0;
};

using ILoggerPtr = ILogger::ILoggerPtr;

}  // namespace logger

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_ILOGGER_CLASS_H
