# Исполняемый файл для тестов
add_executable(tests
    test_utils.cpp
    test_library.cpp
    test_cmake_parser.cpp
    test_Vulnerability_Analyzer.cpp
)

# Пути к заголовкам проекта и doctest.h
target_include_directories(tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# Линковка с библиотекой проекта
target_link_libraries(tests
    ${PROJECT_NAME}_lib
)

# Дополнительные библиотеки для Windows (curl и т.д.)
if(WIN32)
    target_link_libraries(tests
        ws2_32 wldap32 crypt32 advapi32 normaliz
    )
endif()

# Регистрация теста для CTest
add_test(NAME cpp-audit-tests COMMAND tests)