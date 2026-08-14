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

#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_DEFAULT_LOGGER_IMPLEMENTATION_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_DEFAULT_LOGGER_IMPLEMENTATION_CLASS_H

#include <memory>
#include <string>

#include "src/log/ILogger.h"
#include "src/log/default-logger/real-default-logger/RealDefaultLogger.h"
#include "src/log/severity-macro-consts.h"

/**
 * @brief The default logger quick implementation encapsulation namespace.
 */
namespace default_logger
{

/**
 * @brief The default logger static proxy class. It holds no logging code by
 * itself and forwards every static method call into the logger::ILogger
 * implementation instance returned by the DefaultLogger::real_logger getter.
 *
 * The instance is held by the abstract logger::ILogger interface pointer and
 * not by a particular logger class one, so any implementation of that interface
 * is accepted by the DefaultLogger::real_logger setter. The
 * default_logger::RealDefaultLogger implementation is created only as the
 * default one.
 */
class DefaultLogger
{
 public:
  /**
   * @brief The real logger instance holder type. It points to the abstract
   * logger::ILogger interface, so any implementation of it fits.
   */
  using RealLoggerPtr = logger::ILoggerPtr;

  inline static constexpr const unsigned short LVL_ERROR =
      logger::ILogger::LVL_ERROR;
  inline static constexpr const unsigned short LVL_WARNING =
      logger::ILogger::LVL_WARNING;
  inline static constexpr const unsigned short LVL_INFO =
      logger::ILogger::LVL_INFO;
  inline static constexpr const unsigned short LVL_DEBUG =
      logger::ILogger::LVL_DEBUG;
  inline static constexpr const unsigned short LVL_TRACE =
      logger::ILogger::LVL_TRACE;

  inline static constexpr const char* const defaultLogDateFormat =
      RealDefaultLogger::defaultLogDateFormat;

  inline static const std::string& default_log_name =
      RealDefaultLogger::default_log_name;

  virtual ~DefaultLogger() = default;
  DefaultLogger() = default;

  /**
   * @brief Perform a logging message.
   *
   * @param loglvl The level of the log message. The lower the value - more
   * urgent the message.
   * @param msg The actual message for the log file or just stdout stream.
   */
  static void log(const unsigned short& loglvl, const std::string& msg);

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
  static void log(const unsigned short& loglvl, const char* const filePath,
                  const int& fileLine, const std::string& msg);

  /**
   * @brief Sets the new logfile file path.
   *
   * @param filepath The filepath where the log messages will be appended. If
   * empty string - nothing will be performed.
   */
  static void logfile(const std::string& filepath);

  /**
   * @brief Sets if the default logger should print each log message into the
   * stdout.
   *
   * @param toPrintValue New printing boolean value. If the true boolean value
   * given - all the log messages will be duplicated into the stdout.
   */
  static void print(const bool toPrintValue);

  /**
   * @brief Sets the maximum level of log message storage or printing. Above
   * given level all the log messages will be discarded.
   *
   * @param nlvl The new maximum level of log messages. Defaults to
   * DefaultLogger::LVL_INFO
   */
  static void level(const unsigned short& nlvl);

  /**
   * @brief Searches for the string representation of the given log level.
   *
   * @param glvl The numerical log level for it's representation to search.
   *
   * @return Returns non-empty string representation of the given lvl.
   */
  static const std::string& lvl_repr(const unsigned short& glvl);

  /**
   * @brief Init the default logger class.
   *
   * @param filepath The file path where all the generated by the implemented
   * application messages will be stored in the filesystem. If non empty
   * filepath given and fail to open the file by path - exception is generated.
   * @param nlvl The new maximum log messages level. See the
   * DefaultLogger::level method.
   * @param toPrintValue Defines if default logger should print messages to the
   * stdout stream. See the DefaultLogger::print for more details.
   */
  static void init(const std::string& filepath =
                       RealDefaultLogger::get_default_full_log_path(),
                   const unsigned short& nlvl = MAX_LOG_LEVEL,
                   const bool toPrintValue = true);

  /**
   * @brief Returns the real logger instance which stands behind all the
   * DefaultLogger static methods. The very first call creates the default
   * default_logger::RealDefaultLogger one.
   *
   * Pass the returned instance into the `lib`-style projects through their
   * logging initializer macroses to make them log into the very same logger
   * instance.
   *
   * @return Returns the never null real logger instance.
   */
  static RealLoggerPtr real_logger();

  /**
   * @brief Sets the real logger instance which will receive all the further
   * DefaultLogger static method calls. Call it before any logging attempt,
   * usually right at the `lib`-style project initialization.
   *
   * Any logger::ILogger implementation is accepted, so the whole logging
   * behaviour is replaceable without touching the logging macroses.
   *
   * @param newRealLogger The new real logger instance. If the null pointer
   * given - nothing will be performed.
   */
  static void real_logger(const RealLoggerPtr& newRealLogger);

  /**
   * @brief Tells if the real logger instance was handed over from the outside
   * through the DefaultLogger::real_logger setter.
   *
   * A `lib`-style project must not apply its own default logging settings onto
   * an adopted instance, because those settings are owned by the code which
   * uses that library.
   *
   * @return Returns true if the real logger instance was given from the
   * outside and false if it is the own default one.
   */
  static bool real_logger_adopted();

 private:
  /**
   * @brief Provides an access to the real logger instance holder itself.
   *
   * @return Returns the modifiable real logger instance holder.
   */
  static RealLoggerPtr& real_logger_holder();

  inline static bool adopted{false};
};

}  // namespace default_logger

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_DEFAULT_LOGGER_IMPLEMENTATION_CLASS_H
