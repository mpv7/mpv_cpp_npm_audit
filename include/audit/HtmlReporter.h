#pragma once
#include <string>
#include <vector>

#include "audit/Library.h"

namespace audit {

/// @brief Генератор HTML-отчёта о результатах аудита зависимостей.
///
/// Создаёт стилизованный HTML-документ с таблицей, содержащей информацию
/// о каждой библиотеке: версия, статус уязвимости, список CVE и рекомендуемая версия.
/// Также предоставляет метод для автоматического открытия отчёта в браузере.
class HtmlReporter {
 public:
  /// @brief Генерирует HTML-отчёт и сохраняет его в файл.
  ///
  /// На основе переданных данных о библиотеках создаёт HTML-страницу,
  /// включающую сводную статистику, таблицу с деталями и цветовую индикацию статусов.
  ///
  /// @param libraries Вектор объектов Library, полученных в результате аудита.
  /// @param outputPath Путь к файлу, в который будет сохранён отчёт (обычно "audit_report.html").
  static void generate(const std::vector<Library>& libraries, const std::string& outputPath);

  /// @brief Открывает HTML-файл в браузере по умолчанию.
  ///
  /// Определяет команду для текущей операционной системы (Windows: start, macOS: open, Linux: xdg-open)
  /// и выполняет её для указанного файла. При ошибке выводит предупреждение в консоль.
  ///
  /// @param filePath Путь к HTML-файлу (абсолютный или относительный).
  static void openInBrowser(const std::string& filePath);
};

}  // namespace audit