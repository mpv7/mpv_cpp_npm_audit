#include "audit/CmakeParser.h"
#include <fstream>
#include <sstream>  
#include <regex>
#include <stdexcept>
#include "audit/Utils.h"
namespace audit {

    
    CmakeParser::CmakeParser(const std::filesystem::path& projectPath){
        cmakePath_ = projectPath / "CMakeLists.txt";

        if (!std::filesystem::exists(cmakePath_)) {
        throw std::runtime_error("CMakeLists.txt not found: " + cmakePath_.string());
    }
}
    std::vector<Library> CmakeParser::parseFetchContent() {
        std::ifstream file(cmakePath_);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open: " + cmakePath_.string());
        }

        // Читаем ВЕСЬ файл целиком
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    std::vector<Library> libraries;
    
   
    std::regex fetchPattern(
    R"(FetchContent_Declare\s*\(\s*(\w+)[\s\S]*?GIT_TAG\s+["']?([^"'\s\)]+)["']?)",
    std::regex::icase
);
    std::sregex_iterator iter(content.begin(), content.end(), fetchPattern);
    std::sregex_iterator end;
        
        for (; iter != end; ++iter) {
        std::smatch match = *iter;
        if (match.size() >= 3) {
            std::string name = match[1].str();
            std::string rawTag = match[2].str();
            
            // Убираем кавычки (на всякий случай)
            rawTag = replace(rawTag, "\"", "");
            rawTag = replace(rawTag, "'", "");
            
            std::string version = extractVersionFromTag(rawTag);
            
            Library lib(name, version);
            libraries.push_back(lib);
        }
    }
    
    return libraries;
}


std::string CmakeParser::extractVersionFromTag(const std::string& tag) {
    // Убираем пробелы
    std::string cleaned = trim(tag);
    
    // Убираем префиксы типа "v", "release/", "V"
    if (cleaned.size() > 1 && (cleaned[0] == 'v' || cleaned[0] == 'V')) {
        cleaned = cleaned.substr(1);
    }
    
    // Убираем суффиксы типа "-rc1", "-alpha"
    size_t dashPos = cleaned.find('-');
    if (dashPos != std::string::npos) {
        cleaned = cleaned.substr(0, dashPos);
    }
    
    // Проверяем, является ли семантической версией
    if (isValidSemanticVersion(cleaned)) {
        return cleaned;
    }
    
    return "";
};




}

