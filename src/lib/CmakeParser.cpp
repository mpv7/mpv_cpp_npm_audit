#include "audit/CmakeParser.h"

#include <fstream>
#include <regex>
#include <sstream>
#include <stdexcept>

#include "audit/Utils.h"

namespace audit {

CmakeParser::CmakeParser(const std::filesystem::path& projectPath) {
  cmakePath_ = projectPath / "CMakeLists.txt";
  if (!std::filesystem::exists(cmakePath_)) {
    throw std::runtime_error("CMakeLists.txt not found: " + cmakePath_.string());
  }
}

std::vector<Library> CmakeParser::parseFetchContent() {
  std::ifstream file(cmakePath_);
  if (!file.is_open()) throw std::runtime_error("Cannot open: " + cmakePath_.string());

  std::stringstream buffer;
  buffer << file.rdbuf();
  std::string content = buffer.str();
  std::vector<Library> libraries;

  std::regex pattern(R"(FetchContent_Declare\s*\(\s*(\w+)[^)]*?GIT_TAG\s+["']?([^"'\s\)]+)["']?)", std::regex::icase);

  for (std::sregex_iterator it(content.begin(), content.end(), pattern), end; it != end; ++it) {
    if (it->size() >= 3) { 
      std::string name = (*it)[1].str(); 
      std::string rawTag = replace(replace((*it)[2].str(), "\"", ""), "'", ""); 
      std::string version = extractVersionFromTag(rawTag);
      if (!version.empty()) {
        libraries.emplace_back(name, version);
      }
    }
  }
  return libraries;
}

std::string CmakeParser::extractVersionFromTag(const std::string& tag) {
  std::string cleaned = trim(tag);
  if (!cleaned.empty() && (cleaned[0] == 'v' || cleaned[0] == 'V')) {
    cleaned = cleaned.substr(1);
  }
  size_t dash = cleaned.find('-');
  if (dash != std::string::npos) cleaned = cleaned.substr(0, dash); //v1.2.3-alpha.1"; -> 1.2.3 dash = 6 
  return isValidSemanticVersion(cleaned) ? cleaned : ""; 
}

}  // namespace audit