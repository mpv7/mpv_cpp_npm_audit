#include "audit/HtmlReporter.h"
#include <fstream>
#include <cstdlib>
#include <ctime>

#ifdef _WIN32
    #define OPEN_COMMAND "start"
#elif __APPLE__
    #define OPEN_COMMAND "open"
#else
    #define OPEN_COMMAND "xdg-open"
#endif

namespace audit {

void HtmlReporter::generate(const std::vector<Library>& libraries, const std::string& outputPath) {
    std::ofstream file(outputPath);
    
    file << "<!DOCTYPE html>\n";
    file << "<html>\n";
    file << "<head>\n";
    file << "    <meta charset=\"UTF-8\">\n";
    file << "    <title>C++ Audit Report</title>\n";
    file << "    <style>\n";
    file << "        body { font-family: Arial, sans-serif; margin: 20px; }\n";
    file << "        h1 { color: #333; }\n";
    file << "        table { border-collapse: collapse; width: 100%; margin-top: 20px; }\n";
    file << "        th, td { border: 1px solid #ddd; padding: 10px; text-align: left; }\n";
    file << "        th { background-color: #4CAF50; color: white; }\n";
    file << "        tr.ok { background-color: #d4edda; }\n";
    file << "        tr.outdated { background-color: #f8d7da; }\n";
    file << "        tr.notfound { background-color: #fff3cd; }\n";
    file << "        tr.error { background-color: #e2e3e5; }\n";
    file << "        .vuln-list { margin: 0; padding-left: 20px; }\n";
    file << "        .badge { display: inline-block; padding: 3px 8px; border-radius: 4px; font-size: 12px; }\n";
    file << "        .badge-ok { background-color: #28a745; color: white; }\n";
    file << "        .badge-outdated { background-color: #dc3545; color: white; }\n";
    file << "        .badge-notfound { background-color: #ffc107; color: #333; }\n";
    file << "        .badge-error { background-color: #6c757d; color: white; }\n";
    file << "    </style>\n";
    file << "</head>\n";
    file << "<body>\n";
    
    file << "    <h1>🔍 C++ Dependencies Audit Report</h1>\n";
    file << "    <p>Generated: " << std::time(nullptr) << "</p>\n";
    
    file << "    <table>\n";
    file << "        <tr>\n";
    file << "            <th>Library</th>\n";
    file << "            <th>Version</th>\n";
    file << "            <th>Status</th>\n";
    file << "            <th>Vulnerabilities</th>\n";
    file << "            <th>Suggested Fix</th>\n";
    file << "        </tr>\n";
    
    for (const auto& lib : libraries) {
        std::string rowClass;
        std::string badgeClass;
        std::string statusText;
        
        switch (lib.getStatus()) {
            case AuditStatus::Ok:
                rowClass = "ok";
                badgeClass = "badge-ok";
                statusText = "✅ OK";
                break;
            case AuditStatus::Outdated:
                rowClass = "outdated";
                badgeClass = "badge-outdated";
                statusText = "⚠️ Outdated";
                break;
            case AuditStatus::NotFound:
                rowClass = "notfound";
                badgeClass = "badge-notfound";
                statusText = "❓ Not found in NVD";
                break;
            case AuditStatus::Error:
                rowClass = "error";
                badgeClass = "badge-error";
                statusText = "❗ Error";
                break;
        }
        
        file << "        <tr class=\"" << rowClass << "\">\n";
        file << "            <td>" << lib.getName() << "</td>\n";
        file << "            <td>" << lib.getVersion() << "</td>\n";
        file << "            <td><span class=\"badge " << badgeClass << "\">" << statusText << "</span></td>\n";
        
        // Список уязвимостей
        file << "            <td>\n";
        if (lib.getVulnerabilities().empty()) {
            file << "                No known vulnerabilities\n";
        } else {
            file << "                <ul class=\"vuln-list\">\n";
            for (const auto& vuln : lib.getVulnerabilities()) {
                file << "                    <li>\n";
                file << "                        <strong>" << vuln.id << "</strong>";
                if (!vuln.description.empty()) {
                    file << " - " << vuln.description.substr(0, 150);
                    if (vuln.description.size() > 150) file << "...";
                }
                if (vuln.cvssScore > 0) {
                    file << " <span style=\"color: " << (vuln.cvssScore >= 7.0 ? "red" : vuln.cvssScore >= 4.0 ? "orange" : "green") << "\">";
                    file << "(CVSS: " << vuln.cvssScore << ")</span>";
                }
                file << "\n                    </li>\n";
            }
            file << "                </ul>\n";
        }
        file << "            </td>\n";
        
        // Suggested fix
        file << "            <td>";
        if (lib.isVulnerable()) {
            file << "→ Update to <strong>" << lib.getSuggestedFixVersion() << "</strong>";
        } else {
            file << "—";
        }
        file << "</td>\n";
        
        file << "        </tr>\n";
    }
    
    file << "    </table>\n";
    file << "</body>\n";
    file << "</html>\n";
    
    file.close();
}

void HtmlReporter::openInBrowser(const std::string& filePath) {
    std::string command = std::string(OPEN_COMMAND) + " " + filePath;
    std::system(command.c_str());
}

}