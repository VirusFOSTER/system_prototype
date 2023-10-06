#ifndef RADAR_SYSTEM_CONFIGURATION_HPP
#define RADAR_SYSTEM_CONFIGURATION_HPP

#include <iostream>
#include <cstring>

#include "../messages/container/messages_container.hpp"
#include "../components/components_container.hpp"

namespace sys {
namespace config {
/**
 * @brief The configuration_ class - класс, содержащий конфигурацию системы
 */
class configuration_ {
protected:
    using interface_t = typename message::container::message_container_::interface_t;
    using ptr_component_t = typename component::components_container_::ptr_component_t;

public:
    /**
     * @brief configuration_ - конструктор
     * @param cfg_file_ - основной файл конфигурации системы
     */
    explicit configuration_(const std::string cfg_file_ = "");

    /** деструктор */
    ~configuration_() = default;

    /**
     * @brief config_read - получение результата чтения конфигурации системы
     * @return результат чтения конфигурации
     */
    bool config_read() const { return this->config_read_; }

private:
    /**
     * @brief process_configuration - обработка конфигурации системы
     * @param cfg_file - файл конфигурации системы
     * @return результат обработки
     */
    bool process_configuration(const std::string cfg_file);

    /**
     * @brief init_interfaces - инициализация интерфейсов системы
     * @param readers_ - словарь читателей сообщений
     * @param writers_ - словарь писателей сообщений
     * @return результат инициализации
     */
    bool init_interfaces(std::map<uint16_t,std::vector<std::string>> &readers_,
                         std::map<uint16_t,std::vector<std::string>> &writers_);

    /**
     * @brief fix_components - фиксация компонент системы
     * @param fcs_ - массив наименований компонент
     * @return результат фиксации
     */
    bool fix_components(std::vector<std::string> &fcs_);

protected:
    bool config_read_ = false;          /// <--- результат чтения конфигурации системы
    std::vector<interface_t> interfaces_ = {};      /// <--- массив сформированных интерфейсов системы
    std::vector<ptr_component_t> components_ = {};  /// <--- массив активных компонент системы
};
}       /// <--- config
}   /// <--- sys

#endif
