#include "doctest.h"
#include "audit/CmakeParser.h"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

// Создаём временный CMakeLists.txt для тестов
static fs::path createTempCmake(const std::string& content) {
    fs::path tempDir = fs::temp_directory_path() / "cmake_test"; // C:\Users\Михаил\AppData\Local\Temp\cmake_test
    fs::create_directories(tempDir);
    fs::path cmakeFile = tempDir / "CMakeLists.txt";
    std::ofstream file(cmakeFile);
    file << content;
    file.close();
    return tempDir;
}

static void cleanupTempCmake(const fs::path& dir) {
    fs::remove_all(dir);
}

TEST_CASE("CmakeParser - multiple libraries") {
    std::string cmakeContent = R"(
        include(FetchContent)
        FetchContent_Declare(
            fmt
            GIT_REPOSITORY https://github.com/fmtlib/fmt
            GIT_TAG 10.1.0
        )
        FetchContent_Declare(
            spdlog
            GIT_REPOSITORY https://github.com/gabime/spdlog
            GIT_TAG v1.12.0
        )
    )";
    
    auto tempDir = createTempCmake(cmakeContent);
    audit::CmakeParser parser(tempDir);
    auto libs = parser.parseFetchContent();
    
    CHECK(libs.size() == 2);
    CHECK(libs[0].getName() == "fmt");
    CHECK(libs[1].getName() == "spdlog");
    CHECK(libs[1].getVersion() == "1.12.0");
    CHECK_FALSE(libs.empty());
    
    cleanupTempCmake(tempDir);
}