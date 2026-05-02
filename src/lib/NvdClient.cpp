#include "audit/NvdClient.h"
#include <curl/curl.h>
#include <stdexcept>

namespace audit {

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    size_t totalSize = size * nmemb;
    userp->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

NvdClient::NvdClient(const std::string& apiKey, int timeoutSeconds){
    apiKey_ = apiKey;
    timeoutSeconds_ = timeoutSeconds;
    curlHandle_ = curl_easy_init();
    if (!curlHandle_) {
        throw std::runtime_error("Failed to initialize CURL");
    }
}

NvdClient::~NvdClient() {
    if (curlHandle_) {
        curl_easy_cleanup(static_cast<CURL*>(curlHandle_));
    }
}

std::string NvdClient::buildCpeQueryUrl(const std::string& cpe) const {
    CURL* curl = static_cast<CURL*>(curlHandle_);
    
    std::string url = "https://services.nvd.nist.gov/rest/json/cves/2.0?cpeName=";
    url += curl_easy_escape(NULL, cpe.c_str(), 0);
    
    if (!apiKey_.empty()) {
        url += "&apiKey=" + apiKey_;
    }
    
    return url;
}

std::string NvdClient::performHttpGet(const std::string& url) {
    CURL* curl = static_cast<CURL*>(curlHandle_);
    std::string response;
    
    // Настройки запроса
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());//Устанавливает URL для запроса.
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback); //Указывает функцию, которая будет вызываться при получении данных от сервера.
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response); // куда сохраняются промежуточные данные от сервера
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeoutSeconds_);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // разрешаем следовать за редиректами
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "cpp-audit/1.0"); // Устанавливаем User-Agent для идентификации нашего клиента
    
    // Для Windows временно отключаем проверку SSL
    #ifdef _WIN32
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    #endif
    

    CURLcode res = curl_easy_perform(curl);
    
    if (res != CURLE_OK) {
        throw std::runtime_error("CURL error: " + std::string(curl_easy_strerror(res)));
    }
    
    // Проверяем HTTP статус
    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    
    if (http_code != 200) {
        throw std::runtime_error("HTTP error: " + std::to_string(http_code));
    }
    
    return response;
}









json NvdClient::fetchByCpe(const Library& library) {
    // Проверяем, что CPE не пустой
    if (library.getCpe().empty()) {
        throw std::runtime_error("CPE is empty for library: " + library.getName());
    }
    
    // Строим URL и выполняем запрос
    std::string url = buildCpeQueryUrl(library.getCpe());
    std::string response = performHttpGet(url);
    
    // Парсим JSON
    try {
        return json::parse(response);
    } catch (const json::parse_error& e) {
        throw std::runtime_error("Failed to parse JSON response: " + std::string(e.what()));
    }
}

// ============================================
// ПОИСК УЯЗВИМОСТЕЙ ПО КЛЮЧЕВОМУ СЛОВУ
// ============================================
json NvdClient::fetchByKeyword(const std::string& keyword) {
    // Строим URL и выполняем запрос
    std::string url = buildKeywordQueryUrl(keyword);
    std::string response = performHttpGet(url);
    
    // Парсим JSON
    try {
        return json::parse(response);
    } catch (const json::parse_error& e) {
        throw std::runtime_error("Failed to parse JSON response: " + std::string(e.what()));
    }
}


// Пример структуры ответа от NVD API:{
                    //"vulnerabilities": [
                      //  {
                       // "cve": {
                       //     "id": "CVE-2024-1234",
                      //      "description": "...",
                     //       "fixedVersion": "10.2.0"  // ← где-то внутри
                    //    }
                   //     }
                   // ]
                   // }
} // namespace audit