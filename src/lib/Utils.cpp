#include "audit/Utils.h"

#include <algorithm>
#include <cctype>
#include <filesystem>

namespace audit {

std::string trim(const std::string& str) {
  auto start = std::find_if_not(str.begin(), str.end(), ::isspace);
  auto end = std::find_if_not(str.rbegin(), str.rend(), ::isspace).base();
  return (start < end) ? std::string(start, end) : std::string();
}

std::vector<std::string> split(const std::string& str, char delimiter) {
  std::vector<std::string> tokens;
  std::string token;
  for (char c : str) {
    if (c == delimiter) {
      tokens.push_back(token);
      token.clear();
    } else {
      token += c;
    }
  }
  tokens.push_back(token);
  return tokens;
}

std::string replace(const std::string& str, const std::string& from, const std::string& to) {
  if (from.empty()) return str;
  std::string result;
  size_t start = 0, pos;
  while ((pos = str.find(from, start)) != std::string::npos) {
    result.append(str, start, pos - start);
    result.append(to);
    start = pos + from.length();
  }
  result.append(str, start);
  return result;
}

bool isValidSemanticVersion(const std::string& version) {
  if (version.empty()) return false;
  auto parts = split(version, '.');
  if (parts.empty() || parts.size() > 3) return false;
  for (const auto& p : parts) {
    if (p.empty() || !std::all_of(p.begin(), p.end(), ::isdigit)) return false;
    if (p.size() > 1 && p[0] == '0') return false;
  }
  return true;
}

bool fileExists(const std::string& path) { return std::filesystem::exists(path); }
bool isDirectory(const std::string& path) { return std::filesystem::is_directory(path); }

}  // namespace audit