#include "doctest.h"
#include "audit/CpeMapper.h"

using namespace audit;


TEST_CASE("test CpeMapper - known libraries") {
    CpeMapper mapper;
    
    CHECK(mapper.mapToCpe("fmt") == "cpe:2.3:a:fmt:fmt");
    CHECK(mapper.mapToCpe("spdlog") == "cpe:2.3:a:gabime:spdlog");
    CHECK(mapper.mapToCpe("curl") == "cpe:2.3:a:haxx:curl");
    CHECK(mapper.mapToCpe("openssl") == "cpe:2.3:a:openssl:openssl");
    CHECK(mapper.mapToCpe("boost") == "cpe:2.3:a:boost:boost");
    CHECK(mapper.mapToCpe("zlib") == "cpe:2.3:a:zlib:zlib");
    CHECK(mapper.mapToCpe("protobuf") == "cpe:2.3:a:google:protobuf");
    CHECK(mapper.mapToCpe("gtest") == "cpe:2.3:a:google:googletest");
    CHECK(mapper.mapToCpe("doctest") == "cpe:2.3:a:doctest:doctest");
}

TEST_CASE("test CpeMapper - unknown library") {
    CpeMapper mapper;
    CHECK(mapper.mapToCpe("MyLib") == "cpe:2.3:a:mylib:mylib");

    std::string result = mapper.mapToCpe("lib@#$%");
    CHECK(result == "cpe:2.3:a:lib:lib");
}