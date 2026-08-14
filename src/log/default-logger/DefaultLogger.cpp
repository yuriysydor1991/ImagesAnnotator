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

#include "src/log/default-logger/DefaultLogger.h"

#include <memory>
#include <string>

namespace default_logger
{

void DefaultLogger::log(const unsigned short& loglvl, const std::string& msg)
{
  real_logger()->log(loglvl, msg);
}

void DefaultLogger::log(const unsigned short& loglvl,
                        const char* const filePath, const int& fileLine,
                        const std::string& msg)
{
  real_logger()->log(loglvl, filePath, fileLine, msg);
}

void DefaultLogger::logfile(const std::string& filepath)
{
  real_logger()->logfile(filepath);
}

void DefaultLogger::print(const bool toPrintValue)
{
  real_logger()->print(toPrintValue);
}

void DefaultLogger::level(const unsigned short& nlvl)
{
  real_logger()->level(nlvl);
}

const std::string& DefaultLogger::lvl_repr(const unsigned short& glvl)
{
  return real_logger()->lvl_repr(glvl);
}

void DefaultLogger::init(const std::string& filepath,
                         const unsigned short& nlvl, const bool toPrintValue)
{
  real_logger()->init(filepath, nlvl, toPrintValue);
}

DefaultLogger::RealLoggerPtr DefaultLogger::real_logger()
{
  return real_logger_holder();
}

void DefaultLogger::real_logger(const RealLoggerPtr& newRealLogger)
{
  if (!newRealLogger) {
    return;
  }

  real_logger_holder() = newRealLogger;
  adopted = true;
}

bool DefaultLogger::real_logger_adopted() { return adopted; }

DefaultLogger::RealLoggerPtr& DefaultLogger::real_logger_holder()
{
  static RealLoggerPtr realLogger{std::make_shared<RealDefaultLogger>()};

  return realLogger;
}

}  // namespace default_logger
