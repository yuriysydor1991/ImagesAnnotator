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

#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_LOGGER_SUBSYSTEM_DECLARATIONS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_LOGGER_SUBSYSTEM_DECLARATIONS_H

#include <sstream>

#include "src/log/severity-macro-consts.h"
#include "src/log/simple-logger/SimpleLogger.h"

#ifndef LOG_INIT
/**
 * @brief The logging init macros. Use them in the main function or
 * as by default is used in the app::ApplicationFactory::execute method.
 */
#define LOG_INIT(filepath, logLvl, printMessages) \
  simple_logger::SimpleLogger::init(filepath, logLvl, printMessages);
#endif  // LOG_INIT

#ifndef LOG_INIT_DEFAULTS
#define LOG_INIT_DEFAULTS() simple_logger::SimpleLogger::init();
#endif  // LOG_INIT_DEFAULTS

#ifndef LOG_BODY
/**
 * @brief The internal logger macro to define the general logging code body.
 */
#define LOG_BODY(LOGLVL, msg)                                    \
  {                                                              \
    std::stringstream logMessageContainer;                       \
    logMessageContainer << msg;                                  \
    simple_logger::SimpleLogger::log(LOGLVL, __FILE__, __LINE__, \
                                     logMessageContainer.str()); \
  }
#endif  // LOG_BODY

#ifndef LOGE
/**
 * @brief Perform the error logging.
 *
 * @param msg The logging message which may use the << operator
 * and each of the log elements MUST be converted into the std::string.
 */
#define LOGE(msg) LOG_BODY(simple_logger::SimpleLogger::LVL_ERROR, msg)
#endif  // LOGE

#ifndef LOGI
#if MAX_LOG_LEVEL >= MACRO_LOG_LEVEL_INFO
/**
 * @brief Perform the info logging.
 *
 * @param msg The logging message which may use the << operator
 * and each of the log elements MUST be converted into the std::string.
 */
#define LOGI(msg) LOG_BODY(simple_logger::SimpleLogger::LVL_INFO, msg)
#else
#define LOGI(msg)
#endif  // MAX_LOG_LEVEL
#endif  // LOGI

#ifndef LOGW
#if MAX_LOG_LEVEL >= MACRO_LOG_LEVEL_WARNING
/**
 * @brief Perform the warning logging.
 *
 * @param msg The logging message which may use the << operator
 * and each of the log elements MUST be converted into the std::string.
 */
#define LOGW(msg) LOG_BODY(simple_logger::SimpleLogger::LVL_WARNING, msg)
#else
#define LOGW(msg)
#endif  // MAX_LOG_LEVEL
#endif  // LOGW

#ifndef LOGD
#if MAX_LOG_LEVEL >= MACRO_LOG_LEVEL_DEBUG
/**
 * @brief Perform the debug logging.
 *
 * @param msg The logging message which may use the << operator
 * and each of the log elements MUST be converted into the std::string.
 */
#define LOGD(msg) LOG_BODY(simple_logger::SimpleLogger::LVL_DEBUG, msg)
#else
#define LOGD(msg)
#endif  // MAX_LOG_LEVEL
#endif  // LOGD

#ifndef LOGT
#if MAX_LOG_LEVEL >= MACRO_LOG_LEVEL_TRACE
/**
 * @brief Perform the trace logging.
 *
 * @param msg The logging message which may use the << operator
 * and each of the log elements MUST be converted into the std::string.
 */
#define LOGT(msg) LOG_BODY(simple_logger::SimpleLogger::LVL_TRACE, msg)
#else
#define LOGT(msg)
#endif  // MAX_LOG_LEVEL
#endif  // LOGT

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_LOGGER_SUBSYSTEM_DECLARATIONS_H
