#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

TEST_CASE("Dummy test - infrastructure works") {
    CHECK(1 == 1);
    CHECK(2 + 2 == 4);
}

TEST_CASE("String test") {
    std::string hello = "Hello";
    CHECK(hello.size() == 5);
}