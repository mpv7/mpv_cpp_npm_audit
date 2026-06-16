# 🔍 cpp-audit

**cpp-audit** — инструмент для аудита безопасности C++-зависимостей, подключаемых через `FetchContent` в CMake-проектах.

Программа анализирует файл `CMakeLists.txt`, извлекает сведения о внешних библиотеках и их версиях, выполняет поиск известных уязвимостей через базу данных NVD (National Vulnerability Database) и формирует HTML-отчёт с результатами проверки.

---

## ✨ Возможности

* Анализ зависимостей, подключённых через `FetchContent_Declare`
* Извлечение версии библиотеки из `GIT_TAG`
* Поиск известных уязвимостей через NVD API
* Поддержка API-ключей NVD
* Сопоставление CVE с конкретными версиями библиотек
* Фильтрация результатов по CPE
* Генерация подробного HTML-отчёта
* Цветовая индикация уровня опасности уязвимостей
* Автоматическое открытие отчёта в браузере
* Модульные тесты на базе Doctest
* Интеграция с CTest
* Генерация документации через Doxygen

---

## 📦 Используемые технологии

* C++17
* CMake 3.15+
* libcurl
* nlohmann/json
* Doctest
* Doxygen
* vcpkg

---

## 📋 Требования

| Компонент  | Версия                                |
| ---------- | ------------------------------------- |
| CMake      | 3.15+                                 |
| Компилятор | GCC / Clang / MSVC с поддержкой C++17 |
| vcpkg      | актуальная версия                     |
| Git        | рекомендуется                         |

---

## 🚀 Установка

### 1. Клонирование репозитория

```bash
git clone https://github.com/mpv7/mpv_cpp_npm_audit.git
cd mpv_cpp_npm_audit
```

---

### 2. Сборка проекта

Создайте каталог сборки:

```bash
mkdir build
cd build
```

Сконфигурируйте проект:

```bash
cmake .. 
```

Соберите проект:

```bash
cmake --build .
```
После успешной сборки исполняемый файл будет находиться в:

```text
build/bin/cpp-audit.exe
```

---

## ▶️ Использование

### Запуск без API-ключа

```bash
cpp-audit.exe "Путь к проекту"
```

### Запуск с API-ключом NVD

Получите API-ключ на сайте:

https://nvd.nist.gov/developers/request-an-api-key

После этого выполните:

```bash
cpp-audit.exe "Путь к проекту" --api-key=YOUR_API_KEY
```

Использование API-ключа значительно уменьшает задержки между запросами к NVD.

---

## 📄 Пример анализируемого проекта

```cmake
include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)

FetchContent_MakeAvailable(nlohmann_json)
```

В результате анализа будет обнаружена зависимость:

```text
nlohmann_json -> 3.11.3
```

---

## 📊 HTML-отчёт

После завершения проверки автоматически формируется HTML-отчёт, содержащий:

* количество проверенных библиотек;
* список найденных уязвимостей;
* идентификаторы CVE;
* уровень критичности;
* описание уязвимости;
* рекомендации по обновлению.

Пример:

```text
Library        Version     CVE             Severity
-----------------------------------------------------
fmt            8.1.1       CVE-XXXX-XXXX   HIGH
spdlog         1.9.2       CVE-XXXX-XXXX   MEDIUM
```

Отчёт автоматически открывается в браузере после завершения анализа.

<img width="1885" height="877" alt="image" src="https://github.com/user-attachments/assets/b16ffe7e-f1c8-4c4a-90c7-f79308a8a566" />

---

## 🧪 Тестирование

Запуск всех тестов:

```bash
ctest --verbose
```

---

## 📚 Документация

Для генерации документации выполните:

```bash
doxygen Doxyfile
```

После генерации документация будет доступна по пути:

```text
docs/html/index.html
```

---

## 📁 Структура проекта

```text
cpp-audit/
│
├── src/                # Исходный код
├── include/            # Заголовочные файлы
├── tests/              # Модульные тесты
├── docs/               # Документация Doxygen
├── reports/            # HTML-отчёты
├── CMakeLists.txt
└── README.md
```

---

## 🔒 Источник данных об уязвимостях

Проверка выполняется через официальную базу данных уязвимостей NVD (National Vulnerability Database), поддерживаемую NIST (National Institute of Standards and Technology).

Документация API:

https://nvd.nist.gov/developers
