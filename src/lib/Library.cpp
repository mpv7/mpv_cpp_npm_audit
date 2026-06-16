#include "audit/Library.h"

namespace audit {

Library::Library(const std::string& name, const std::string& version) : name_(name), version_(version) {}

const std::string& Library::getName() const noexcept { return name_; }
const std::string& Library::getVersion() const noexcept { return version_; }
const std::vector<Vulnerability>& Library::getVulnerabilities() const noexcept { return vulnerabilities_; }
AuditStatus Library::getStatus() const noexcept { return status_; }
const std::string& Library::getSuggestedFixVersion() const noexcept { return suggestedFixVersion_; }

void Library::addVulnerability(const Vulnerability& vuln) { vulnerabilities_.push_back(vuln); }
void Library::setStatus(AuditStatus status) { status_ = status; }
void Library::setSuggestedFixVersion(const std::string& version) { suggestedFixVersion_ = version; }
bool Library::isVulnerable() const noexcept { return status_ == AuditStatus::Outdated; }

}  // namespace audit