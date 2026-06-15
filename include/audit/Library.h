#pragma once
#include <string>
#include <vector>

namespace audit {

/// @brief Статус безопасности библиотеки по результатам аудита.
enum class AuditStatus {
    Ok,       ///< Уязвимостей не найдено, версия актуальна.
    Outdated, ///< Библиотека уязвима, требуется обновление.
    NotFound, ///< Библиотека не найдена в базе NVD.
    Error     ///< Ошибка при проверке (сеть, парсинг и т.п.).
};

/// @brief Информация об одной уязвимости (CVE).
struct Vulnerability {
    std::string id;          ///< Идентификатор CVE (например, "CVE-2024-1234").
    std::string description; ///< Краткое описание уязвимости (на английском).
    std::string fixedVersion;///< Версия, в которой уязвимость исправлена.
    double cvssScore = 0.0;  ///< Оценка CVSS (от 0 до 10, где 10 – максимальная опасность).
};

/// @brief Представляет стороннюю библиотеку с её версией и найденными уязвимостями.
class Library {
public:
    /// @brief Конструктор по умолчанию. Создаёт пустую библиотеку со статусом Ok.
    Library() = default;

    /// @brief Конструктор с именем и версией.
    /// @param name Имя библиотеки (например, "fmt").
    /// @param version Версия библиотеки (например, "10.1.0").
    Library(const std::string& name, const std::string& version);

    /// @return Имя библиотеки.
    const std::string& getName() const noexcept;

    /// @return Версия библиотеки.
    const std::string& getVersion() const noexcept;

    /// @return Список найденных уязвимостей (может быть пустым).
    const std::vector<Vulnerability>& getVulnerabilities() const noexcept;

    /// @return Текущий статус аудита.
    AuditStatus getStatus() const noexcept;

    /// @return Рекомендуемая версия для исправления уязвимостей
    ///         (актуально, если статус Outdated).
    const std::string& getSuggestedFixVersion() const noexcept;

    /// @brief Добавляет информацию об одной уязвимости.
    /// @param vuln Объект уязвимости.
    void addVulnerability(const Vulnerability& vuln);

    /// @brief Устанавливает статус аудита.
    /// @param status Новый статус.
    void setStatus(AuditStatus status);

    /// @brief Устанавливает рекомендуемую версию для обновления.
    /// @param version Версия, которая исправляет все известные уязвимости.
    void setSuggestedFixVersion(const std::string& version);

    /// @brief Проверяет, является ли библиотека уязвимой.
    /// @return true если статус == Outdated, иначе false.
    bool isVulnerable() const noexcept;

private:
    std::string name_;                  ///< Имя библиотеки.
    std::string version_;               ///< Текущая версия.
    std::string suggestedFixVersion_;   ///< Рекомендуемая версия для обновления.
    std::vector<Vulnerability> vulnerabilities_; ///< Обнаруженные CVE.
    AuditStatus status_ = AuditStatus::Ok;       ///< Статус аудита.
};

} // namespace audit