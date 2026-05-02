#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include "audit/Library.h"

namespace audit {

using json = nlohmann::json;

class NvdClient {
public:
    NvdClient(const std::string& apiKey = "", int timeoutSeconds = 10);
    ~NvdClient();

    json fetchByCpe(const Library& library);
    json fetchByKeyword(const std::string& keyword);

private:
    std::string performHttpGet(const std::string& url);
    std::string buildCpeQueryUrl(const std::string& cpe) const;
    std::string buildKeywordQueryUrl(const std::string& keyword) const;

    std::string apiKey_;
    int timeoutSeconds_;
    void* curlHandle_;
};

} // namespace audit