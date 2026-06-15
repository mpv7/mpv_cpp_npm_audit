#include "audit/HtmlReporter.h"
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <filesystem>  // <-- Важно для абсолютных путей
#include <iostream>

#ifdef _WIN32
    #define OPEN_CMD "start"
#elif __APPLE__
    #define OPEN_CMD "open"
#else
    #define OPEN_CMD "xdg-open"
#endif

namespace audit {

void HtmlReporter::generate(const std::vector<Library>& libraries, const std::string& outputPath) {
    std::ofstream file(outputPath);
    
    // Получаем текущее время правильно
    std::time_t now = std::time(nullptr);
    std::tm* tm_info = std::localtime(&now);
    char time_buffer[64];
    std::strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", tm_info);
    
    file << R"(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>C++ Audit Report</title>
    <style>
        body { font-family: system-ui, -apple-system, sans-serif; margin: 20px; background: #f8f9fa; color: #212529; line-height: 1.6; }
        h1 { color: #212529; border-bottom: 3px solid #0d6efd; padding-bottom: 10px; }
        .meta { color: #6c757d; margin-bottom: 20px; }
        table { border-collapse: collapse; width: 100%; background: #fff; box-shadow: 0 2px 4px rgba(0,0,0,0.1); border-radius: 8px; overflow: hidden; }
        th, td { padding: 12px 15px; border: 1px solid #dee2e6; text-align: left; }
        th { background: #0d6efd; color: #fff; font-weight: 600; text-transform: uppercase; font-size: 0.85em; }
        tr:nth-child(even) { background: #f8f9fa; }
        tr.ok { background: #d1e7dd; }
        tr.outdated { background: #f8d7da; }
        tr.notfound { background: #fff3cd; }
        tr.error { background: #e2e3e5; }
        .badge { display: inline-block; padding: 4px 10px; border-radius: 20px; font-size: 12px; font-weight: 600; text-transform: uppercase; }
        .badge-ok { background: #198754; color: #fff; }
        .badge-out { background: #dc3545; color: #fff; }
        .badge-not { background: #ffc107; color: #000; }
        .badge-err { background: #6c757d; color: #fff; }
        .vuln-list { margin: 0; padding-left: 20px; font-size: 13px; }
        .vuln-list li { margin-bottom: 5px; }
        .summary { background: #fff; padding: 20px; border-radius: 8px; box-shadow: 0 2px 4px rgba(0,0,0,0.1); margin-bottom: 20px; }
        .summary h2 { margin-top: 0; color: #0d6efd; }
        .stat { display: inline-block; margin-right: 20px; padding: 10px 15px; background: #f8f9fa; border-radius: 5px; }
        .stat-value { font-size: 24px; font-weight: bold; }
        .stat-label { font-size: 12px; color: #6c757d; }
    </style>
</head>
<body>
    <h1>🔍 C++ Dependencies Audit Report</h1>
    <p class="meta">Generated: )" << time_buffer << R"(</p>
    
    <div class="summary">
        <h2>📊 Summary</h2>)";

    // Подсчитываем статистику
    int total = libraries.size();
    int safe = 0, vulnerable = 0, notfound = 0, errors = 0;
    
    for (const auto& lib : libraries) {
        switch (lib.getStatus()) {
            case AuditStatus::Ok: safe++; break;
            case AuditStatus::Outdated: vulnerable++; break;
            case AuditStatus::NotFound: notfound++; break;
            case AuditStatus::Error: errors++; break;
        }
    }
    
    file << R"(
        <div class="stat">
            <div class="stat-value">)" << total << R"(</div>
            <div class="stat-label">Total</div>
        </div>
        <div class="stat" style="background: #d1e7dd;">
            <div class="stat-value" style="color: #198754;">)" << safe << R"(</div>
            <div class="stat-label">Safe</div>
        </div>
        <div class="stat" style="background: #f8d7da;">
            <div class="stat-value" style="color: #dc3545;">)" << vulnerable << R"(</div>
            <div class="stat-label">Vulnerable</div>
        </div>
        <div class="stat" style="background: #fff3cd;">
            <div class="stat-value" style="color: #856404;">)" << notfound << R"(</div>
            <div class="stat-label">Not in NVD</div>
        </div>
        <div class="stat" style="background: #e2e3e5;">
            <div class="stat-value" style="color: #495057;">)" << errors << R"(</div>
            <div class="stat-label">Errors</div>
        </div>
    </div>
    
    <table>
        <thead>
            <tr>
                <th>Library</th>
                <th>Version</th>
                <th>Status</th>
                <th>Vulnerabilities</th>
                <th>Recommended Fix</th>
            </tr>
        </thead>
        <tbody>)";

    for (const auto& lib : libraries) {
        std::string rowClass, badgeClass, statusText;
        
        switch (lib.getStatus()) {
            case AuditStatus::Ok: rowClass="ok"; badgeClass="badge-ok"; statusText="✅ OK"; break;
            case AuditStatus::Outdated: rowClass="outdated"; badgeClass="badge-out"; statusText="⚠️ VULNERABLE"; break;
            case AuditStatus::NotFound: rowClass="notfound"; badgeClass="badge-not"; statusText="❓ Not Found"; break;
            case AuditStatus::Error: rowClass="error"; badgeClass="badge-err"; statusText="❌ Error"; break;
        }
        
        file << "<tr class=\"" << rowClass << "\">";
        file << "<td><strong>" << lib.getName() << "</strong></td>";
        file << "<td>" << lib.getVersion() << "</td>";
        file << "<td><span class=\"badge " << badgeClass << "\">" << statusText << "</span></td>";
        file << "<td>";
        
        if (lib.getVulnerabilities().empty()) {
            file << "No known vulnerabilities";
        } else {
            file << "<ul class=\"vuln-list\">";
            for (const auto& v : lib.getVulnerabilities()) {
                file << "<li><strong>" << v.id << "</strong> (CVSS: " << v.cvssScore << ") " << v.description << "</li>";
            }
            file << "</ul>";
        }
        
        file << "</td><td>";
        if (lib.isVulnerable()) file << "➜ Update to <b>" << lib.getSuggestedFixVersion() << "</b>";
        else file << "—";
        file << "</td></tr>";
    }
    
    file << R"(
        </tbody>
    </table>
    
    <p style="margin-top: 20px; color: #6c757d; font-size: 12px;">
        Generated by <strong>mpm_audit</strong> — C++ Dependency Vulnerability Scanner
    </p>
</body>
</html>)";
    
    file.close();
    std::cout << "   Report saved successfully." << std::endl;
}

void HtmlReporter::openInBrowser(const std::string& filePath) {
    try {
        // Получаем полный абсолютный путь, чтобы избежать ошибок с относительными путями
        std::string absolutePath = std::filesystem::absolute(filePath).string();
        std::string cmd;

#ifdef _WIN32
        // В Windows команда 'start' требует заголовок в кавычках, если путь тоже в кавычках
        // Синтаксис: start "Заголовок" "Путь_к_файлу"
        cmd = std::string(OPEN_CMD) + " \"\" \"" + absolutePath + "\"";
#elif __APPLE__
        cmd = std::string(OPEN_CMD) + " \"" + absolutePath + "\"";
#else
        cmd = std::string(OPEN_CMD) + " \"" + absolutePath + "\"";
#endif

        std::system(cmd.c_str());
    } catch (const std::exception& e) {
        std::cerr << "   [Warning] Could not open browser: " << e.what() << std::endl;
    }
}

} // namespace audit