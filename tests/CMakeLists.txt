# Fetch doctest
include(FetchContent)
FetchContent_Declare(
        doctest
        GIT_REPOSITORY https://github.com/doctest/doctest
        GIT_TAG v2.4.11
)
FetchContent_MakeAvailable(doctest)

# Test executable
add_executable(tests
        test_cmake_parser.cpp
        test_version_comparator.cpp
        test_nvd_client.cpp
)

target_include_directories(tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries(tests
        ${PROJECT_NAME}
        doctest::doctest
)

# Add to CTest
add_test(NAME cpp-audit-tests COMMAND tests)