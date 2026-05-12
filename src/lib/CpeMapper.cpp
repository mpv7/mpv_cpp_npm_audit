#include "audit/CpeMapper.h"
#include <algorithm>
#include <cctype>

namespace audit {

CpeMapper::CpeMapper() {
    // ✅ Полный формат: 13 компонентов (part:vendor:product + 10 джокеров)
    cpeMap_["fmt"] = "cpe:2.3:a:fmt:fmt:*:*:*:*:*:*:*:*";
    cpeMap_["spdlog"] = "cpe:2.3:a:gabime:spdlog:*:*:*:*:*:*:*:*";
    cpeMap_["nlohmann_json"] = "cpe:2.3:a:nlohmann:json:*:*:*:*:*:*:*:*";
    cpeMap_["nlohmann-json"] = "cpe:2.3:a:nlohmann:json:*:*:*:*:*:*:*:*";
    cpeMap_["json"] = "cpe:2.3:a:nlohmann:json:*:*:*:*:*:*:*:*";
    cpeMap_["curl"] = "cpe:2.3:a:haxx:curl:*:*:*:*:*:*:*:*";
    cpeMap_["libcurl"] = "cpe:2.3:a:haxx:curl:*:*:*:*:*:*:*:*";
    cpeMap_["openssl"] = "cpe:2.3:a:openssl:openssl:*:*:*:*:*:*:*:*";
    cpeMap_["boost"] = "cpe:2.3:a:boost:boost:*:*:*:*:*:*:*:*";
    cpeMap_["zlib"] = "cpe:2.3:a:zlib:zlib:*:*:*:*:*:*:*:*";
    cpeMap_["protobuf"] = "cpe:2.3:a:google:protobuf:*:*:*:*:*:*:*:*";
    cpeMap_["gtest"] = "cpe:2.3:a:google:googletest:*:*:*:*:*:*:*:*";
    cpeMap_["doctest"] = "cpe:2.3:a:doctest:doctest:*:*:*:*:*:*:*:*";
}

std::string CpeMapper::mapToCpe(const std::string& libName) const {
    auto it = cpeMap_.find(libName);
    if (it != cpeMap_.end()) {
        return it->second;
    }
    
    // Авто-генерация с полным форматом
    std::string normalized = libName;
    std::transform(normalized.begin(), normalized.end(), normalized.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    normalized.erase(std::remove_if(normalized.begin(), normalized.end(),
                   [](unsigned char c) { return !std::isalnum(c); }),
                   normalized.end());
    
    if (!normalized.empty()) {
        // ✅ Возвращаем полный CPE с 10 джокерами
        return "cpe:2.3:a:" + normalized + ":" + normalized + ":*:*:*:*:*:*:*:*";
    }
    
    return "";
}

void CpeMapper::addMapping(const std::string& libName, const std::string& cpe) {
    cpeMap_[libName] = cpe;
}

} // namespace audit