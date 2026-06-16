#pragma once
#include <string>
#include <vector>

namespace audit {

/// @brief Удаляет начальные и конечные пробельные символы из строки.
/// @param str Входная строка.
/// @return Строка без пробелов по краям.
std::string trim(const std::string& str);

/// @brief Разбивает строку на части по указанному разделителю.
/// @param str Исходная строка.
/// @param delimiter Символ-разделитель.
/// @return Вектор подстрок (если разделителей несколько, между ними могут быть пустые строки).
std::vector<std::string> split(const std::string& str, char delimiter);

/// @brief Заменяет все вхождения подстроки 'from' на подстроку 'to'.
/// @param str Исходная строка.
/// @param from Что заменять.
/// @param to На что заменять.
/// @return Новая строка после замен.
std::string replace(const std::string& str, const std::string& from, const std::string& to);

/// @brief Проверяет, соответствует ли строка формату семантической версии.
///
/// Допустимые форматы: "5", "1.2", "10.20.30".
/// Запрещены: ведущие нули ("01.2"), буквенные суффиксы ("1.2-alpha"), префикс 'v'.
///
/// @param version Строка для проверки.
/// @return true, если формат корректен; false в противном случае.
bool isValidSemanticVersion(const std::string& version);

/// @brief Проверяет существование файла по указанному пути.
/// @param path Путь к файлу.
/// @return true, если файл существует; false, если не существует или это директория.
bool fileExists(const std::string& path);

/// @brief Проверяет, является ли указанный путь директорией.
/// @param path Путь к файловой системе.
/// @return true, если путь ведёт к существующей директории; false в противном случае.
bool isDirectory(const std::string& path);

}  // namespace audit