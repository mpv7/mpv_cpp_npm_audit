#include "Utils.h"
#include <filesystem>
#include <cctype>

namespace audit{
std::string trim(const std::string& str) {
  
    size_t start = 0;
    while (start < str.size() && str[start] == ' ') {
        ++start;
    }

    size_t end = str.size();
    while (end > start && str[end - 1] == ' ') {
        --end;
    }
    return str.substr(start, end - start);
}

std::vector<std::string> split(const std::string& str, char delimiter){
    std::string middle_result;
    std::vector<std::string> str_result;

    for (char c: str){
        if (c != delimiter){
            middle_result += c;
        } else {
            str_result.push_back(middle_result);
            middle_result.clear();

        }
    }
    str_result.push_back(middle_result);
    return str_result;
}

bool startsWith(const std::string& str, const std::string& prefix){
    if (prefix.size() > str.size()) return false;

    bool flag = true;
    for (int i=0; i<prefix.size();i ++){
        if (flag and str[i]==prefix[i]){
            flag=true;
        }else{
            return false;
        }

    }
    return flag;
}

bool endsWith(const std::string& str, const std::string& suffix){
    if (suffix.size() > str.size()) return false;

    bool flag = true;
    int index_suf = str.size()-suffix.size();
    int counter = 0;

    for (int i=index_suf; i< str.size(); i++){
        if (flag and str[i]==suffix[counter]){
            flag=true;
            counter += 1;
        }else{
            return false;
        }
    }
    return flag;
}

bool contains(const std::string& str, const std::string& substring) {
    return str.find(substring) != std::string::npos;
}

std::string replace(const std::string& str, const std::string& from, const std::string& to) {
    if (from.empty()) {
        return str;   
    }

    std::string result;
    size_t start = 0;     
    size_t pos;

    while ( (pos = str.find(from, start)) != std::string::npos ) {
        result.append(str, start, pos - start);

        result.append(to);
        
        start = pos + from.length();
    }

    result.append(str, start, std::string::npos);

    return result;
}


bool isValidSemanticVersion(const std::string& version) {
    if (version.empty()) return false;
    std::vector<std::string> parts = split(version, '.');
    if (parts.empty() || parts.size() > 3) return false;
    for (const auto& part : parts) {
        if (part.empty()) return false;
        for (char c : part) {
            if (!std::isdigit(c)) return false;
        }
        // запрещаем ведущие нули (кроме "0")
        if (part.size() > 1 && part[0] == '0') return false;
    }
    return true;
}

bool fileExists(const std::string& path){
    return std::filesystem::exists(path);
}

bool isDirectory(const std::string& path){
    return std::filesystem::is_directory(path);
}


}





