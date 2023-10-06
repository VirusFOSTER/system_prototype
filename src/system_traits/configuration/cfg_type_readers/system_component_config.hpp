#ifndef RADAR_SYSTEM_COMPONENTS_CONFIGURATION_HPP
#define RADAR_SYSTEM_COMPONENTS_CONFIGURATION_HPP

#include <iostream>
#include <cstdint>

#include "../../../common/json/json.hpp"

namespace sys {
namespace config {
/**
 * @brief The components_reader_ class - класс для получения информации о компонентах системы
 */
class components_reader_ {
public:
    /**
     * @brief components_reader_ - конструктор
     * @param cfg_file_ - файл конфигурации, содержащий информацию о компонентах
     */
    explicit components_reader_(const std::string cfg_file_ = "", const std::string cfg_type = "");

    /** деструктор */
    ~components_reader_() = default;

    /**
     * @brief active_components_names - получение списка активных компонент системы
     * @return массив активных компонент системы
     */
    std::vector<std::string> active_components_names() const { return this->active_components_names_; }

private:
    /**
     * @brief verification_components - верификация компонент системы
     */
    void verification_components(json::object::JsonObject *obj_components, const std::string cfg_type);

    /**
     * @brief component_name_dublicate - проверка на дублирование компонент по наименованию
     * @param fc_name_ - наименование компоненты
     * @return результат проверки
     */
    bool component_name_dublicate(const std::string &fc_name_);

    /**
     * @brief component_id_dublicate - проверка на дублирование компонент по идентификатору
     * @param fc_id_ - уникальный идентификатор компоненты
     * @return результат проверки
     */
    bool component_id_dublicate(const uint16_t &fc_id_);

private:
    std::vector<std::string> active_components_names_ = {}; /// <--- имена активных компонент
    std::vector<uint16_t> active_components_uids_ = {};     /// <--- идентификаторы активных компонент
};
}       /// <--- config
}   /// <--- sys

#endif
