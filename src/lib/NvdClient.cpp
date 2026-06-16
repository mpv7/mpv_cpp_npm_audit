#include "audit/NvdClient.h"

#include <curl/curl.h>

#include <iostream>
#include <stdexcept>

namespace audit {
using json = nlohmann::json;

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
  userp->append(static_cast<char*>(contents), size * nmemb);
  return size * nmemb;
}

NvdClient::NvdClient(const std::string& apiKey, int timeoutSeconds) : apiKey_(apiKey), timeoutSeconds_(timeoutSeconds) {
  curlHandle_ = curl_easy_init();
  if (!curlHandle_) throw std::runtime_error("Failed to init CURL");
}

NvdClient::~NvdClient() {
  if (curlHandle_) curl_easy_cleanup(static_cast<CURL*>(curlHandle_));
}

std::string NvdClient::buildQueryUrl(const std::string& keyword) const {
  CURL* curl = static_cast<CURL*>(curlHandle_);
  std::string url = "https://services.nvd.nist.gov/rest/json/cves/2.0?keywordSearch=";
  char* esc = curl_easy_escape(curl, keyword.c_str(), 0);
  if (esc) {
    url += esc;
    curl_free(esc);
  }
  return url;
}

std::string NvdClient::performHttpGet(const std::string& url) {
  CURL* curl = static_cast<CURL*>(curlHandle_);
  std::string response;

  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
  curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeoutSeconds_);
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
  curl_easy_setopt(curl, CURLOPT_USERAGENT, "mpm_audit/1.0");

  struct curl_slist* headers = nullptr;
  if (!apiKey_.empty()) {
    std::string hdr = "apiKey: " + apiKey_;
    headers = curl_slist_append(headers, hdr.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  }

  CURLcode res = curl_easy_perform(curl);
  if (headers) curl_slist_free_all(headers);

  if (res != CURLE_OK) {
    throw std::runtime_error("CURL error: " + std::string(curl_easy_strerror(res)));
  }

  long code = 0;
  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
  if (code != 200) {
    std::cerr << "[NVD] HTTP " << code << " for " << url << "\n";
    if (!response.empty() && response.size() < 500) {
      std::cerr << "[NVD] Body: " << response << "\n";
    }
    throw std::runtime_error("HTTP " + std::to_string(code));
  }
  return response;
}

json NvdClient::fetchVulnerabilities(const std::string& keyword) {
  std::string resp = performHttpGet(buildQueryUrl(keyword));
  try {
    return json::parse(resp);
  } catch (const json::parse_error& e) {
    throw std::runtime_error("JSON parse error: " + std::string(e.what()));
  }
}

}  // namespace audit