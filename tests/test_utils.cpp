#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "audit/Utils.h"

namespace audit{
    TEST_CASE("test trim"){
        CHECK(trim("  version is empty    ") == "version is empty");
        CHECK_FALSE(trim(" version is 12.2.1   ") == "version is 12.2.1   ");
    }

    TEST_CASE("test split"){
        std::vector<std::string> test = split("1,2,3", ',');
        CHECK(test[0] == "1");
        CHECK(test[2] == "3");
        CHECK_FALSE(test.size() > 3);
    }
    TEST_CASE("test startsWith"){
        CHECK(startsWith("hello world", "hello"));
        CHECK(startsWith("hello", "he"));
        CHECK_FALSE(startsWith("hello", "world"));
    }

    TEST_CASE("test endsWith"){
        CHECK(endsWith("test.txt", ".txt"));
        CHECK(endsWith("example", "ample"));
        CHECK_FALSE(endsWith("abc", "abcd"));
    }

    TEST_CASE("test contains"){
        CHECK(contains("C++ programming", "++"));
        CHECK_FALSE(contains("abc", "d"));
    }

    TEST_CASE("test replace") {
        CHECK(replace("hello world", "world", "C++") == "hello C++");
        CHECK(replace("a b a", "a", "x") == "x b x");
        CHECK(replace("abc", "d", "x") == "abc");
        CHECK_FALSE(replace("abc", "b", "x") == "abc");
    }

    TEST_CASE("test isValidSemanticVersion"){
        CHECK(isValidSemanticVersion("5"));
        CHECK(isValidSemanticVersion("0.0.1"));
        CHECK(isValidSemanticVersion("10.20.30"));
        CHECK_FALSE(isValidSemanticVersion("v1.2"));
        CHECK_FALSE(isValidSemanticVersion("1.2.3-alpha"));
        CHECK_FALSE(isValidSemanticVersion("01.2.3"));
    }

    /// НАПИСАТЬ ДЛЯ ДИРРЕКТОРИЙ И ПАПКИ ТЕСТЫ ПРИДЕТСЯ НАЙТИ ЧТО ТО ТИПО МОК ТЕСТОВ В ПИТОНЕ ИЛИ НА КОМПЕ СОЗДАВАТЬ
}