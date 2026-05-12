#pragma once
#include <string>
#include <vector>

namespace audit {

enum class AuditStatus { Ok, Outdated, NotFound, Error };

struct Vulnerability {
    std::string id;
    std::string description;
    std::string fixedVersion;
    double cvssScore = 0.0;
};

class Library {
public:
    Library() = default;
    Library(const std::string& name, const std::string& version);

    const std::string& getName() const noexcept;
    const std::string& getVersion() const noexcept;
    const std::vector<Vulnerability>& getVulnerabilities() const noexcept;
    AuditStatus getStatus() const noexcept;
    const std::string& getSuggestedFixVersion() const noexcept;

    void addVulnerability(const Vulnerability& vuln);
    void setStatus(AuditStatus status);
    void setSuggestedFixVersion(const std::string& version);
    bool isVulnerable() const noexcept;

private:
    std::string name_, version_, suggestedFixVersion_;
    std::vector<Vulnerability> vulnerabilities_;
    AuditStatus status_ = AuditStatus::Ok;
};

} // namespace audit