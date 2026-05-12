#pragma once

#include <string>
#include <unordered_map>

namespace audit {

/**
 * @brief Маппер имён библиотек в CPE-идентификаторы
 * 
 * Содержит предустановленные маппинги для популярных C++ библиотек
 * и фоллбэк на авто-генерацию CPE из имени библиотеки.
 */
class CpeMapper {
public:
    CpeMapper();
    
    /**
     * @brief Получить CPE для имени библиотеки
     * @param libName Имя библиотеки (как в CMake FetchContent)
     * @return CPE-строка или пустая строка если не удалось сопоставить
     * 
     * Примеры:
     * - "fmt" → "cpe:2.3:a:fmt:fmt"
     * - "spdlog" → "cpe:2.3:a:gabime:spdlog"
     * - "unknown_lib" → "cpe:2.3:a:unknownlib:unknownlib" (авто-генерация)
     */
    std::string mapToCpe(const std::string& libName) const;
    
    /**
     * @brief Добавить пользовательский маппинг
     * @param libName Имя библиотеки
     * @param cpe Полный или короткий CPE-идентификатор
     */
    void addMapping(const std::string& libName, const std::string& cpe);

private:
    std::unordered_map<std::string, std::string> cpeMap_;
};

} // namespace audit