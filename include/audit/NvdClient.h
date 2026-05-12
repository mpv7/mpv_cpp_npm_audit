#pragma once
#include <string>
#include <nlohmann/json.hpp>

namespace audit {

class NvdClient {
public:
    NvdClient(const std::string& apiKey = "", int timeoutSeconds = 30);
    ~NvdClient();

    NvdClient(const NvdClient&) = delete;
    NvdClient& operator=(const NvdClient&) = delete;

    nlohmann::json fetchVulnerabilities(const std::string& keyword);

private:
    std::string buildQueryUrl(const std::string& keyword) const;
    std::string performHttpGet(const std::string& url);

    std::string apiKey_;
    int timeoutSeconds_;
    void* curlHandle_;
};

} // namespace audit