#pragma once
#include <string>
#include <vector>
#include "audit/Library.h"

namespace audit {

class HtmlReporter {
public:
    static void generate(const std::vector<Library>& libraries, const std::string& outputPath);
    static void openInBrowser(const std::string& filePath);
};

} // namespace audit