#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include "audit/Library.h"

namespace audit {

/// @brief Парсит CMakeLists.txt для извлечения зависимостей, объявленных через FetchContent.
/// 
/// Сканирует файл CMakeLists.txt в указанной директории проекта, находит все вызовы
/// `FetchContent_Declare` и извлекает имена библиотек и версии из аргумента `GIT_TAG`.
class CmakeParser {
public:
    /// @brief Конструктор парсера для заданного проекта.
    /// @param projectPath Путь к корневой директории CMake-проекта (должен содержать CMakeLists.txt).
    /// @throws std::runtime_error Если файл CMakeLists.txt не найден по указанному пути.
    explicit CmakeParser(const std::filesystem::path& projectPath);

    /// @brief Анализирует CMakeLists.txt и возвращает список библиотек.
    /// 
    /// Находит все блоки `FetchContent_Declare`, извлекает первый аргумент как имя библиотеки,
    /// а значение `GIT_TAG` как версию. Версия очищается от ведущих 'v'/'V' и проверяется на соответствие
    /// семантическому формату.
    /// 
    /// @return std::vector<Library> Список библиотек (включаются только те, у которых корректная семантическая версия).
    std::vector<Library> parseFetchContent();

private:
    /// @brief Извлекает семантическую версию из строки Git-тега.
    /// 
    /// Удаляет ведущий символ 'v' или 'V', отбрасывает суффикс после первого дефиса (для пре-релизов),
    /// и проверяет, является ли оставшаяся строка корректной семантической версией
    /// (major[.minor[.patch]] – только цифры, без ведущих нулей).
    /// 
    /// @param tag Исходный Git-тег (например, "v1.2.3" или "1.2.3-beta").
    /// @return std::string Корректная семантическая версия или пустая строка, если версия невалидна.
    std::string extractVersionFromTag(const std::string& tag);

    std::filesystem::path cmakePath_; ///< Полный путь к файлу CMakeLists.txt.
};

} // namespace audit