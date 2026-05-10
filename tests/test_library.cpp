#include "doctest.h"
#include "audit/Library.h"

using namespace audit;

TEST_CASE("test getters"){
    Library lib("fmt", "10.1.0");
    lib.setCpe("cpe:2.3:a:fmt:fmt");

    CHECK(lib.getName()=="fmt");
    CHECK(lib.getVersion()=="10.1.0");
    CHECK(lib.getCpe()=="cpe:2.3:a:fmt:fmt");
    CHECK(lib.getVulnerabilities().empty());
    CHECK(lib.getStatus() == AuditStatus::Ok);

    lib.setStatus(AuditStatus::Outdated);
    CHECK_FALSE(lib.getStatus() == AuditStatus::Error);
}

TEST_CASE("test Vulnerability"){
    Library lib("spdlog", "1.12.0");

    Vulnerability vuln1;
    vuln1.id = "CVE-2023-1234";
    vuln1.description = "Buffer overflow";
    vuln1.cvssScore = 7.5;
    vuln1.fixedVersion = "1.12.1";

    lib.addVulnerability(vuln1);
    CHECK(lib.getVulnerabilities().size() == 1);
    CHECK(lib.getVulnerabilities()[0].id == "CVE-2023-1234");
    CHECK(lib.getVulnerabilities()[0].cvssScore == 7.5);

    lib.setStatus(AuditStatus::Ok);
    CHECK_FALSE(lib.isVulnerable());



}

