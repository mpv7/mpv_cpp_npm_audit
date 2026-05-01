#pragma once

#include <string>
#include <unordered_map>

namespace audit {

class CpeMapper {
public:
    CpeMapper();

    std::string mapToCpe(const std::string& libName) const;

    void addMapping(const std::string& libName, const std::string& cpe);

private:
    std::unordered_map<std::string, std::string> cpeMap_;
};

} // namespace audit