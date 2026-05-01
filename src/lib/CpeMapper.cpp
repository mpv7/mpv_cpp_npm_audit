#include "audit/CpeMapper.h"
#include <algorithm>
#include <cctype>

namespace audit {

CpeMapper::CpeMapper() {
    cpeMap_["fmt"] = "cpe:2.3:a:fmt:fmt";
    cpeMap_["spdlog"] = "cpe:2.3:a:gabime:spdlog";
    cpeMap_["nlohmann_json"] = "cpe:2.3:a:nlohmann:json";
    cpeMap_["nlohmann-json"] = "cpe:2.3:a:nlohmann:json";
    cpeMap_["json"] = "cpe:2.3:a:nlohmann:json";
    cpeMap_["curl"] = "cpe:2.3:a:haxx:curl";
    cpeMap_["libcurl"] = "cpe:2.3:a:haxx:curl";
    cpeMap_["openssl"] = "cpe:2.3:a:openssl:openssl";
    cpeMap_["boost"] = "cpe:2.3:a:boost:boost";
    cpeMap_["zlib"] = "cpe:2.3:a:zlib:zlib";
    cpeMap_["protobuf"] = "cpe:2.3:a:google:protobuf";
    cpeMap_["gtest"] = "cpe:2.3:a:google:googletest";
    cpeMap_["doctest"] = "cpe:2.3:a:doctest:doctest";
}

std::string CpeMapper::mapToCpe(const std::string& libName) const {
    auto it = cpeMap_.find(libName);
    if (it != cpeMap_.end()) {
        return it->second;
    }
    
    std::string lowerName = "";
    int index = 0;
    while (lowerName.size() < libName.size()) {
        lowerName += std::tolower(static_cast<unsigned char>(libName[index]));
        index++;
    }

    auto new_end = std::remove_if(lowerName.begin(), lowerName.end(),
                                  [](unsigned char c) { return !std::isalnum(c); });
    lowerName.erase(new_end, lowerName.end());

    if (!lowerName.empty()) {
        return "cpe:2.3:a:" + lowerName + ":" + lowerName;
    }
    
    return "";
}

void CpeMapper::addMapping(const std::string& libName, const std::string& cpe) {
    cpeMap_[libName] = cpe;
}

} // namespace audit