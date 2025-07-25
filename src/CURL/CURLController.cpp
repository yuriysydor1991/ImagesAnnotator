#include "src/CURL/CURLController.h"

#include <algorithm>
#include <cassert>
#include <functional>
#include <memory>
#include <vector>

#include "src/log/log.h"

namespace curli
{

namespace
{

size_t wcallback(void* contents, size_t size, size_t nmemb, void* userp)
{
  assert(contents != nullptr);
  assert(userp != nullptr);

  const auto givenSize = size * nmemb;

  if (givenSize == 0) {
    LOGD("No data fetch");
    return givenSize;
  }

  LOGT("Fetch " << givenSize << " bytes");

  CURLController* controller = static_cast<CURLController*>(userp);

  assert(controller != nullptr);

  if (controller == nullptr) {
    LOGE("Unknown dst type");
    return 0U;
  }

  char* rawb = static_cast<char*>(contents);

  auto& buff = controller->get();

  buff.reserve(buff.size() + givenSize);

  buff.insert(buff.end(), rawb, rawb + givenSize);

  return givenSize;
}

}  // namespace

CURLController::~CURLController() { curl_easy_cleanup(curl); }

CURLController::CURLController()
{
  static const CURLcode initedCode = curl_global_init(CURL_GLOBAL_ALL);

  if (initedCode != CURLE_OK) {
    LOGE("Fail to init CURL globally, code: "
         << static_cast<unsigned int>(initedCode));
    return;
  }

  LOGT("curl globally inited");

  cbuff.reserve(DEFAULT_BUFF_RESERVE);

  curl = curl_easy_init();

  assert(curl != nullptr);

  if (curl == nullptr) {
    LOGE("Fail to allocate curl easy context instance");
    return;
  }
}

CURLController::download_buffer& CURLController::get() { return cbuff; }

CURLController::download_buffer& CURLController::download(
    const std::string& url)
{
  cbuff.clear();

  assert(!url.empty());

  if (url.empty()) {
    LOGE("Invalid URL provided");
    return cbuff;
  }

  if (curl == nullptr) {
    LOGE("Fail to create download context");
    return cbuff;
  }

  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, wcallback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
  curl_easy_setopt(curl, CURLOPT_TIMEOUT, DEFAULT_TIMEOUT);
  curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, DEFAULT_CONNECTTIMEOUT);
  curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, DEFAULT_LOWSPEEDSECS);
  curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, DEFAULT_LOWSPEEDLIMIT);

  LOGT("Trying to download the data for " << url);

  CURLcode res = curl_easy_perform(curl);

  if (res != CURLE_OK) {
    LOGE("CURL error: " << curl_easy_strerror(res));
  }

  LOGT("Fetched " << cbuff.size() << " bytes total");

  return cbuff;
}

std::string CURLController::get_last_download_mime()
{
  assert(curl != nullptr);

  char* content_type{nullptr};

  curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &content_type);

  if (content_type == nullptr) {
    LOGD("Fail to get last download mime type");
    return {};
  }

  const std::string lastMime = content_type;

  return lastMime;
}

CURLControllerPtr CURLController::create()
{
  return std::make_shared<CURLController>();
}

std::string CURLController::get_absolute_url(const std::string& hostPath,
                                             const std::string& relPath)
{
  CURLU* base_url = curl_url();
  CURLU* resolved_url = curl_url();

  std::string finalURL =
      get_absolute_url(base_url, resolved_url, hostPath, relPath);

  curl_url_cleanup(base_url);
  curl_url_cleanup(resolved_url);

  return finalURL;
}

std::string CURLController::get_absolute_url(CURLU* base_url,
                                             CURLU* resolved_url,
                                             const std::string& hostPath,
                                             const std::string& relPath)
{
  std::string finalURL;

  if (resolved_url == nullptr) {
    LOGE("Fail to create resolved url context instance");
    return {};
  }

  if (base_url == nullptr) {
    LOGE("Fail to create base url context instance");
    return {};
  }

  if (curl_url_set(resolved_url, CURLUPART_URL, hostPath.c_str(), 0) !=
      CURLUE_OK) {
    LOGE("Failure to set the host path");
    return {};
  }

  if (curl_url_set(resolved_url, CURLUPART_URL, relPath.c_str(), 0) !=
      CURLUE_OK) {
    LOGE("Failure to set rel path");
    return {};
  }

  char* absolute_url{nullptr};

  if (curl_url_get(resolved_url, CURLUPART_URL, &absolute_url, 0) !=
          CURLUE_OK &&
      absolute_url != nullptr) {
    LOGE("URL resolution failed");
    return {};
  }

  assert(absolute_url != nullptr);

  finalURL = absolute_url;

  curl_free(absolute_url);

  return finalURL;
}

bool CURLController::is_url(const std::string& maybe)
{
  static const std::string uComp = "://";

  return std::search(maybe.cbegin(), maybe.cend(), uComp.cbegin(),
                     uComp.cend()) != maybe.cend();
}

std::string CURLController::get_url_hostname(const std::string& url)
{
  CURLU* resolved_url = curl_url();

  if (curl_url_set(resolved_url, CURLUPART_URL, url.c_str(), 0) != CURLUE_OK) {
    curl_url_cleanup(resolved_url);
    LOGE("Failure to set the host path");
    return {};
  }

  char* hostname_url{nullptr};

  if (curl_url_get(resolved_url, CURLUPART_HOST, &hostname_url, 0) !=
          CURLUE_OK &&
      hostname_url != nullptr) {
    curl_url_cleanup(resolved_url);
    LOGE("URL resolution failed");
    return {};
  }

  assert(hostname_url != nullptr);

  std::string hostname = hostname_url;

  curl_free(hostname_url);

  curl_url_cleanup(resolved_url);

  return hostname;
}

std::string CURLController::get_url_path(const std::string& url)
{
  CURLU* resolved_url = curl_url();

  if (curl_url_set(resolved_url, CURLUPART_URL, url.c_str(), 0) != CURLUE_OK) {
    curl_url_cleanup(resolved_url);
    LOGE("Failure to set the host path");
    return {};
  }

  char* path_url{nullptr};

  if (curl_url_get(resolved_url, CURLUPART_PATH, &path_url, 0) != CURLUE_OK &&
      path_url != nullptr) {
    curl_url_cleanup(resolved_url);
    LOGE("URL resolution failed");
    return {};
  }

  assert(path_url != nullptr);

  std::string path = path_url;

  curl_free(path_url);

  curl_url_cleanup(resolved_url);

  return path;
}

}  // namespace curli
