if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.30)
    set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
endif()

include(FetchContent)

FetchContent_Declare(
        doctest
        GIT_REPOSITORY https://github.com/doctest/doctest
        GIT_TAG v2.4.11
        GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(doctest)

add_executable(tests
        testsss.cpp
)

target_link_libraries(tests
        cpp-audit_lib
        doctest::doctest
)

add_test(NAME cpp-audit-tests COMMAND tests)