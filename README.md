# C++ Audit Tool



**Аналог npm audit для C++ проектов на CMake**

Проверяет зависимости (FetchContent) через NVD API на наличие уязвимостей.

## Функционал

- 🔍 Парсинг CMakeLists.txt (FetchContent)
- 🌐 Запросы к NVD API
- 📊 Анализ уязвимостей и сравнение версий
- 📄 Генерация HTML отчета
- 🚀 Автоматический запуск в браузере

## Требования

- C++17
- CMake 3.15+
- libcurl
- nlohmann/json

## Сборка

```bash
git clone https://github.com/your/cpp-audit.git
cd cpp-audit
mkdir build && cd build
cmake ..
cmake --build .