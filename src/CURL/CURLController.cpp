#include "src/CURL/CURLController.h"

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

  LOGT("Trying to download the data for " << url);

  CURLcode res = curl_easy_perform(curl);

  if (res != CURLE_OK) {
    LOGE("CURL error: " << curl_easy_strerror(res));
  }

  LOGT("Fetched " << cbuff.size() << " bytes total");

  return cbuff;
}

CURLControllerPtr CURLController::create()
{
  return std::make_shared<CURLController>();
}

}  // namespace curli
