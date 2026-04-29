# Создаём тестовый исполняемый файл
add_executable(tests
    test_utils.cpp
    test_library.cpp
    test_cmake_parser.cpp
)

# Подключаем заголовки проекта и doctest.h из корня
target_include_directories(tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# Линкуем со статической библиотекой
target_link_libraries(tests
    ${PROJECT_NAME}_lib
)

# Добавляем тест в CTest
add_test(NAME cpp-audit-tests COMMAND tests)

# Для Windows (если нужны доп. библиотеки)
if(WIN32)
    target_link_libraries(tests
        ws2_32 wldap32 crypt32 advapi32 normaliz
    )
endif()