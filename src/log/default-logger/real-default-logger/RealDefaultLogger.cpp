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

#include "src/log/default-logger/real-default-logger/RealDefaultLogger.h"

#include <array>
#include <chrono>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>

namespace default_logger
{

void RealDefaultLogger::log(const unsigned short& loglvl,
                            const std::string& msg)
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

    if (loglvl <= LVL_WARNING) {
      alogfile.flush();
    }
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

void RealDefaultLogger::log(const unsigned short& loglvl,
                            const char* const filePath, const int& fileLine,
                            const std::string& msg)
{
  std::filesystem::path fullPath{filePath};

  std::string filename = fullPath.filename().string();

  log(loglvl, filename + ":" + std::to_string(fileLine) + " : " + msg);
}

void RealDefaultLogger::logfile(const std::string& filepath)
{
  if (filepath.empty()) {
    return;
  }

  if (alogfile.is_open()) {
    alogfile.close();
  }

  alogfile.open(filepath.c_str(), std::fstream::app);

  if (!alogfile.is_open()) {
    throw std::runtime_error{"Fail to open the log file at " + filepath};
  }
}

void RealDefaultLogger::print(const bool toPrintValue)
{
  toPrintMsgs.store(toPrintValue);
}

void RealDefaultLogger::level(const unsigned short& nlvl) { lvl = nlvl; }

void RealDefaultLogger::init(const std::string& filepath,
                             const unsigned short& nlvl,
                             const bool toPrintValue)
{
  logfile(filepath);
  level(nlvl);
  print(toPrintValue);
}

inline void RealDefaultLogger::insert_current_timestamp(std::ostringstream& oss)
{
  using namespace std::chrono;

  const auto now = system_clock::now();

  const time_t now_time_t = system_clock::to_time_t(now);
  std::tm timeHolder = *std::localtime(&now_time_t);

  oss << std::put_time(&timeHolder, defaultLogDateFormat);

#ifdef ENABLE_LOGS_MICROSECONDS_TIME
  static constexpr const char microsecFiller = '0';
  static constexpr const unsigned int microsecWidth = 6U;

  const auto timeSinceEpoch = now.time_since_epoch();
  auto seconds = duration_cast<std::chrono::seconds>(timeSinceEpoch);
  auto microseconds =
      duration_cast<std::chrono::microseconds>(timeSinceEpoch - seconds);

  oss << '.' << std::setfill(microsecFiller) << std::setw(microsecWidth)
      << microseconds.count();
#endif  // ENABLE_LOGS_MICROSECONDS_TIME
}

const std::string& RealDefaultLogger::lvl_repr(const unsigned short& glvl)
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

std::string RealDefaultLogger::get_full_log_path(const std::string& logname)
{
  namespace fs = std::filesystem;

  static const fs::path default_log_path = DEFAULT_LOG_FILE_PATH;

  const fs::path logpath = default_log_path / logname;

  return logpath.string();
}

std::string RealDefaultLogger::get_default_full_log_path()
{
  return get_full_log_path(default_log_name);
}

}  // namespace default_logger
