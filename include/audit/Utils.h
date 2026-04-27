#pragma once

#include "string"
#include "vector"

namespace audit{

    std:: string trim(const std::string& str);
    std::vector<std::string> split(const std::string& str, char delimiter);

    bool startsWith(const std::string& str, const std::string& prefix);
    bool endsWith(const std::string& str, const std::string& suffix);
    bool contains(const std::string& str, const std::string& substring);
    std::string replace(const std::string& str, const std::string& from, const std::string& to);

    bool isValidSemanticVersion(const std::string& version);

    bool fileExists(const std::string& path);
    bool isDirectory(const std::string& path);

};