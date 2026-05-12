#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include "audit/Library.h"

namespace audit {

class CmakeParser {
public:
    explicit CmakeParser(const std::filesystem::path& projectPath);
    std::vector<Library> parseFetchContent();

private:
    std::string extractVersionFromTag(const std::string& tag);
    std::filesystem::path cmakePath_;
};

} // namespace audit