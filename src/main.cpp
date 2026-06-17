#include <chrono>
#include <filesystem>
#include <iostream>
#include <thread>
#include <vector>

#include "audit/CmakeParser.h"
#include "audit/HtmlReporter.h"
#include "audit/NvdClient.h"
#include "audit/Utils.h"
#include "audit/VulnerabilityAnalyzer.h"

void printUsage(const char* prog) {
  std::cout << "Usage: " << prog << " <project-path> [--api-key KEY]\n";
  std::cout << "Get API key: https://nvd.nist.gov/developers/request-an-api-key\n";
}

int main(int argc, char* argv[]) {
  try {
    std::string projectPath, apiKey;
    for (int i = 1; i < argc; ++i) {
      std::string a = argv[i];
      if (a == "--help") {
        printUsage(argv[0]);
        return 0;
      }
      if (a == "--api-key" && i + 1 < argc) {
        apiKey = argv[++i];
      } else if (projectPath.empty()) {
        projectPath = a;
      }
    }
    if (projectPath.empty() || !audit::isDirectory(projectPath)) {
      std::cerr << "Error: Valid project path required.\n";
      printUsage(argv[0]);
      return 1;
    }

    std::cout << "Scanning: " << projectPath << "\n";
    audit::CmakeParser parser(projectPath);
    auto libs = parser.parseFetchContent();
    if (libs.empty()) {
      std::cout << "No FetchContent deps found.\n";
      return 0;
    }
    std::cout << "Found " << libs.size() << " libraries.\n";

    audit::NvdClient nvd(apiKey, 30);
    for (size_t i = 0; i < libs.size(); ++i) {
      auto& lib = libs[i];
      std::cout << "   [" << (i + 1) << "/" << libs.size() << "] " << lib.getName() << " (" << lib.getVersion()
                << ")... ";
      std::cout.flush();
      try {
        auto resp = nvd.fetchVulnerabilities(lib.getName());
        audit::VulnerabilityAnalyzer::analyze(lib, resp);
        if (lib.isVulnerable()) {
          std::cout << "VULNERABLE -> fix: " << lib.getSuggestedFixVersion() << "\n";
        
        } else if (lib.getStatus() == audit::AuditStatus::NotFound) {
          std::cout << "Not found in NVD\n";
        } else if (lib.getStatus() == audit::AuditStatus::Error) {
          std::cout << "Error analyzing\n";
        }else {
          std::cout << "OK\n";
        }
      } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
        lib.setStatus(audit::AuditStatus::Error);
      }
      if (i + 1 < libs.size()) std::this_thread::sleep_for(std::chrono::seconds(apiKey.empty() ? 7 : 1));
    }

    std::string report = "audit_report.html";
    audit::HtmlReporter::generate(libs, report);
    std::cout << "Report: " << report << "\n";
    audit::HtmlReporter::openInBrowser(report);

    int v = 0, s = 0, e = 0;
    for (auto& l : libs) {
      if (l.isVulnerable())
        v++;
      else if (l.getStatus() == audit::AuditStatus::Error)
        e++;
      else
        s++;
    }
    std::cout << "\n----------\nSafe: " << s << "\nVuln: " << v << "\nErrors: " << e << "\n----------\n";
    return (v || e) ? 1 : 0;
  } catch (const std::exception& e) {
    std::cerr << "Fatal: " << e.what() << "\n";
    return 2;
  }
}