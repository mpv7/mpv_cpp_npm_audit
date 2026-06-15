cmake_minimum_required(VERSION 3.15)
project(cpp-audit VERSION 1.0.0 LANGUAGES CXX)

# C++ стандарт
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Генерация compile_commands.json для IDE
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Директории вывода артефактов
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Опции сборки
option(BUILD_TESTS "Build unit tests" ON)

# ЗАВИСИМОСТИ
find_package(CURL REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

# БИБЛИОТЕКА (ЯДРО ПРОЕКТА)
add_library(${PROJECT_NAME}_lib STATIC
    src/lib/Library.cpp
    src/lib/Utils.cpp
    src/lib/CmakeParser.cpp
    src/lib/VulnerabilityAnalyzer.cpp
    src/lib/NvdClient.cpp
    src/lib/HtmlReporter.cpp
)

target_include_directories(${PROJECT_NAME}_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}_lib PUBLIC
    CURL::libcurl
    nlohmann_json::nlohmann_json
)

if(WIN32)
    target_compile_definitions(${PROJECT_NAME}_lib PRIVATE CURL_STATICLIB)
    target_link_libraries(${PROJECT_NAME}_lib PRIVATE
        ws2_32 wldap32 crypt32 advapi32 normaliz
    )
endif()

# ИСПОЛНЯЕМЫЙ ФАЙЛ
add_executable(${PROJECT_NAME}
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${PROJECT_NAME}_lib
)

# ТЕСТЫ
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# ДОКУМЕНТАЦИЯ (автоматически, если найден Doxygen)
find_package(Doxygen QUIET)
if(DOXYGEN_FOUND)
    set(DOXYGEN_IN  ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
    )

    # Автоматически собирать документацию при сборке основной программы
    add_dependencies(${PROJECT_NAME} docs)
else()
    message(STATUS "Doxygen not found - documentation will not be generated")
endif()