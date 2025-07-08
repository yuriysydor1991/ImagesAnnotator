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

#include "src/log/simple-logger/SimpleLogger.h"

#include <array>
#include <chrono>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

namespace simple_logger
{

void SimpleLogger::log(const unsigned short& loglvl, const std::string& msg)
{
  if (loglvl > lvl) {
    return;
  }

  std::lock_guard<std::mutex> alogfile_m_guard{alogfile_m};

  std::ostringstream finalLog;

  insert_current_timestamp(finalLog);

  finalLog << " " << lvl_repr(loglvl) << " " << std::this_thread::get_id()
           << " " << msg << std::endl;

  const std::string finalLogStr = finalLog.str();

  if (alogfile.is_open()) {
    alogfile << finalLogStr;
  }

  if (!toPrintMsgs.load()) {
    return;
  }

  if (loglvl <= LVL_WARNING) {
    std::cerr << finalLogStr;
  } else {
    std::cout << finalLogStr;
  }
}

void SimpleLogger::log(const unsigned short& loglvl, const char* const filePath,
                       const int& fileLine, const std::string& msg)
{
  std::filesystem::path fullPath{filePath};

  std::string filename = fullPath.filename().string();

  log(loglvl, filename + ":" + std::to_string(fileLine) + " : " + msg);
}

void SimpleLogger::logfile(const std::string& filepath)
{
  if (filepath.empty()) {
    return;
  }

  alogfile.open(filepath.c_str(), std::fstream::app);

  if (!alogfile.is_open()) {
    throw std::runtime_error{"Fail to open the log file at " + filepath};
  }
}

void SimpleLogger::print(const bool toPrintValue)
{
  toPrintMsgs.store(toPrintValue);
}

void SimpleLogger::level(const unsigned short& nlvl) { lvl = nlvl; }

void SimpleLogger::init(const std::string& filepath, const unsigned short& nlvl,
                        const bool toPrintValue)
{
  logfile(filepath);
  level(nlvl);
  print(toPrintValue);
}

inline void SimpleLogger::insert_current_timestamp(std::ostringstream& oss)
{
  static constexpr const char microsecFiller = '0';
  static constexpr const unsigned int microsecWidth = 6U;

  using namespace std::chrono;

  const auto now = system_clock::now();

  const time_t now_time_t = system_clock::to_time_t(now);
  std::tm timeHolder = *std::localtime(&now_time_t);

  const auto timeSinceEpoch = now.time_since_epoch();

  auto seconds = duration_cast<std::chrono::seconds>(timeSinceEpoch);
  auto microseconds =
      duration_cast<std::chrono::microseconds>(timeSinceEpoch - seconds);

  oss << std::put_time(&timeHolder, defaultLogDateFormat);
  oss << '.' << std::setfill(microsecFiller) << std::setw(microsecWidth)
      << microseconds.count();
}

const std::string& SimpleLogger::lvl_repr(const unsigned short& glvl)
{
  static constexpr const unsigned short maxLvls = 6U;
  static const std::array<const std::string, maxLvls> reprs{
      std::string{"UNK"}, std::string{"ERR"}, std::string{"WRN"},
      std::string{"INF"}, std::string{"DBG"}, std::string{"TRA"},
  };

  if (glvl >= maxLvls) {
    return reprs[0U];
  }

  return reprs[glvl];
}

}  // namespace simple_logger
