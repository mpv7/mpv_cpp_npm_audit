#pragma once

#include <vector>
#include <string>

namespace audit {

struct Vulnerability {
    std::string id;
    std::string description;
    double cvssScore;
    std::string fixedVersion;
};

enum class AuditStatus {
    Ok,
    Outdated,
    NotFound,
    Error
};

class Library {
public:
    Library() = default;
    Library(const std::string& name, const std::string& version);

    const std::string& getName() const noexcept;
    const std::string& getVersion() const noexcept;
    const std::string& getCpe() const noexcept;
    const std::vector<Vulnerability>& getVulnerabilities() const noexcept;
    AuditStatus getStatus() const noexcept;

    void setCpe(const std::string& cpe);
    void addVulnerability(const Vulnerability& vuln);
    void setStatus(AuditStatus status);

    bool isVulnerable() const noexcept;
    std::string getSuggestedFixVersion() const;

private:
    std::string name_;
    std::string version_;
    std::string cpe_;
    std::vector<Vulnerability> vulnerabilities_;
    AuditStatus status_ = AuditStatus::Ok;
};

}