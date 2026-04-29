#include "audit/Libruary.h"

namespace audit {


Library::Library(const std::string& name, const std::string& version) {
    name_ = name;
    version_ = version;

}


const std::string& Library::getName() const noexcept {
    return name_;
}

const std::string& Library::getVersion() const noexcept {
    return version_;
}
const std::string& Library::getCpe() const noexcept {
    return cpe_;
}
const std::vector<Vulnerability>& Library::getVulnerabilities() const noexcept {
    return vulnerabilities_;
}

AuditStatus Library::getStatus() const noexcept {
    return status_;
}


void Library::setCpe(const std::string& cpe) {
    cpe_ = cpe;
}

void Library::addVulnerability(const Vulnerability& vuln) {
    vulnerabilities_.push_back(vuln);
}

void Library::setStatus(AuditStatus status) {
    status_=status;
}


bool Library::isVulnerable() const noexcept {
    return status_ == AuditStatus::Outdated;
}

std::string Library::getSuggestedFixVersion() const {
    return "unknown";
} // ДОДЕЛАТЬ КОГДА РЕАЛИЗУЮ ОБРАЩЕНИЕ В NVD

} // namespace audit