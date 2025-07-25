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

#include "src/helpers/ImageLoader.h"

#include <cassert>
#include <chrono>
#include <exception>
#include <filesystem>
#include <fstream>
#include <sstream>

#include "src/CURL/CURLController.h"
#include "src/log/log.h"

namespace helpers
{

ImageLoader::ImageLoader() : curl{curli::CURLController::create()}
{
  assert(curl != nullptr);
}

bool ImageLoader::load(ImageRecordPtr ir)
{
  assert(ir != nullptr);

  if (ir == nullptr) {
    LOGE("Invalid ImageRecord pointer provided");
    return false;
  }

  if (!has_urls(ir)) {
    return true;
  }

  const std::string url = get_ir_path(ir);

  assert(curli::CURLController::is_url(url));

  if (url.empty()) {
    LOGE("Fail to fetch full URL for given ir");
    return false;
  }

  assert(curl != nullptr);

  const auto& irdata = curl->download(url);

  if (!write_data(ir, irdata)) {
    LOGE("Failure to whrite the image record data into tmp file");
    return false;
  }

  return true;
}

bool ImageLoader::write_data(ImageRecordPtr ir, const download_buffer& irdata)
{
  assert(ir != nullptr);

  if (irdata.empty()) {
    LOGE("Fail to download the data");
    return false;
  }

  ir->tmppath = get_net_ir_filepath(ir);

  if (ir->tmppath.empty()) {
    LOGE("Failure to generate new ir fs path");
    return false;
  }

  try {
    std::fstream f{ir->tmppath.string().c_str(),
                   std::fstream::trunc | std::fstream::out};

    if (!f.is_open()) {
      LOGE("Failure to create the file: " << ir->tmppath.string());
      return false;
    }

    for (const auto& c : irdata) {
      f << c;
    }

    f.close();
  }
  catch (std::exception& e) {
    LOGE("Fail to write data to temporary file " << ir->tmppath.string()
                                                 << " reason: " << e.what());
    return false;
  }

  return true;
}

std::filesystem::path ImageLoader::get_net_ir_filepath(ImageRecordPtr ir)
{
  assert(ir != nullptr);

  const std::filesystem::path irdpath = get_directory_path(ir);

  if (!create_directories(irdpath)) {
    LOGE("Fail to create subdirectories");
    return {};
  }

  const std::filesystem::path irfpath = generate_new_filepath(ir, irdpath);

  if (!create_file(irfpath)) {
    LOGE("Failure to create the file " << irfpath.string());
    return {};
  }

  LOGT("Generated path: " << irfpath.string());

  return irfpath;
}

std::filesystem::path ImageLoader::generate_new_filepath(
    [[maybe_unused]] const ImageRecordPtr ir,
    const std::filesystem::path& irdpath)
{
  assert(ir != nullptr);
  assert(!irdpath.empty());

  const std::string imgTimestamp = get_fs_timestamp();
  std::filesystem::path irfpath;
  size_t fiter{0U};

  do {
    const std::string imgName =
        imgTimestamp + "-loaded-image" +
        (fiter == 0 ? std::string{} : std::string{"-"} + std::to_string(fiter));

    irfpath = irdpath / imgName;

    fiter++;
  } while (std::filesystem::is_regular_file(irfpath));

  return irfpath;
}

bool ImageLoader::create_directories(const std::filesystem::path& irdpath)
{
  assert(!irdpath.empty());

  try {
    if (!std::filesystem::is_directory(irdpath)) {
      if (!std::filesystem::create_directories(irdpath)) {
        LOGE("Failure to create temporary dir: " << irdpath.string());
        return false;
      }
    }
  }
  catch (const std::exception& e) {
    LOGE("Fail to create temporary dir " << irdpath.string()
                                         << " reason: " << e.what());
    return false;
  }

  return true;
}

std::filesystem::path ImageLoader::get_directory_path(ImageRecordPtr ir)
{
  assert(ir != nullptr);

  static const std::string firstCallFSTimestamp = get_fs_timestamp();

  const std::string url = get_ir_path(ir);
  assert(curli::CURLController::is_url(url));
  const std::string hostname = curli::CURLController::get_url_hostname(url);

  const std::filesystem::path irdpath = get_tmp_path() / app_folder_name /
                                        preloads_folder_name /
                                        firstCallFSTimestamp / hostname;
  return irdpath;
}

bool ImageLoader::create_file(const std::filesystem::path& irfpath)
{
  assert(!irfpath.empty());

  std::fstream f{irfpath.string().c_str(),
                 std::fstream::trunc | std::fstream::out};

  if (!f.is_open()) {
    LOGE("Failure to create the file: " << irfpath.string());
    return {};
  }

  f.close();

  return true;
}

std::filesystem::path ImageLoader::get_tmp_path()
{
  try {
    return std::filesystem::temp_directory_path();
  }
  catch (const std::filesystem::filesystem_error& e) {
    LOGE("Failed to get temp directory: " << e.what());
  }

  return {};
}

std::string ImageLoader::get_fs_timestamp()
{
  static constexpr const char microsecFiller = '0';
  static constexpr const unsigned int microsecWidth = 6U;

  using namespace std::chrono;

  std::ostringstream oss;

  const auto now = system_clock::now();

  const time_t now_time_t = system_clock::to_time_t(now);
  std::tm timeHolder = *std::localtime(&now_time_t);

  const auto timeSinceEpoch = now.time_since_epoch();

  auto seconds = duration_cast<std::chrono::seconds>(timeSinceEpoch);
  auto microseconds =
      duration_cast<std::chrono::microseconds>(timeSinceEpoch - seconds);

  oss << std::put_time(&timeHolder, defaultFSDateFormat);
  oss << '.' << std::setfill(microsecFiller) << std::setw(microsecWidth)
      << microseconds.count();

  return oss.str();
}

ImageLoaderPtr ImageLoader::create() { return std::make_shared<ImageLoader>(); }

}  // namespace helpers
