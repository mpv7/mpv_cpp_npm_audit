#pragma once

#include "Library.h"
#include <string>
#include <vector>

namespace audit {

class HtmlReporter {
public:
    static void generate(const std::vector<Library>& libraries, const std::string& outputPath);
    static void openInBrowser(const std::string& filePath);
};

}