#pragma once
#include <nlohmann/json.hpp>
#include <string>

namespace audit {

/// @brief Клиент для взаимодействия с NVD API (National Vulnerability Database).
///
/// Выполняет HTTP-запросы к REST API сервиса NVD NIST для поиска уязвимостей
/// по ключевому слову (имени библиотеки). Поддерживает авторизацию по API-ключу
/// и настройку таймаута. Использует libcurl для сетевых операций.
class NvdClient {
 public:
  /// @brief Конструктор клиента NVD.
  /// @param apiKey API-ключ для доступа к NVD (можно получить на nvd.nist.gov).
  ///               Если не указан, запросы будут без ключа (ограничение 5 запросов за 30 секунд).
  /// @param timeoutSeconds Таймаут HTTP-запроса в секундах. По умолчанию 30.
  /// @throws std::runtime_error Если не удалось инициализировать libcurl.
  NvdClient(const std::string& apiKey = "", int timeoutSeconds = 30);

  /// @brief Деструктор. Очищает ресурсы libcurl.
  ~NvdClient();

  // Запрет копирования (объект содержит ресурс CURL)
  NvdClient(const NvdClient&) = delete;
  NvdClient& operator=(const NvdClient&) = delete;

  /// @brief Выполняет поиск уязвимостей по ключевому слову.
  ///
  /// Формирует URL: https://services.nvd.nist.gov/rest/json/cves/2.0?keywordSearch=<keyword>,
  /// отправляет GET-запрос и возвращает ответ в виде JSON-объекта.
  ///
  /// @param keyword Ключевое слово (обычно имя библиотеки).
  /// @return nlohmann::json Распарсенный ответ от NVD API.
  /// @throws std::runtime_error При ошибках сети, HTTP-статусе не 200 или ошибках парсинга JSON.
  nlohmann::json fetchVulnerabilities(const std::string& keyword);

 private:
  /// @brief Формирует URL запроса к NVD API с ключевым словом.
  /// @param keyword Ключевое слово для поиска.
  /// @return std::string Полный URL для GET-запроса.
  std::string buildQueryUrl(const std::string& keyword) const;

  /// @brief Выполняет HTTP GET-запрос и возвращает тело ответа.
  ///
  /// Устанавливает заголовки (включая apiKey, если передан), таймаут,
  /// User-Agent. При ошибках выбрасывает исключение.
  ///
  /// @param url Адрес запроса.
  /// @return std::string Тело ответа (в формате JSON).
  /// @throws std::runtime_error При сбоях CURL или некорневом HTTP-статусе.
  std::string performHttpGet(const std::string& url);

  std::string apiKey_;  ///< API-ключ для доступа к NVD (может быть пустым).
  int timeoutSeconds_;  ///< Таймаут запроса в секундах.
  void* curlHandle_;    ///< Указатель на объект CURL* (libcurl). Хранится как void* для инкапсуляции.
};

}  // namespace audit