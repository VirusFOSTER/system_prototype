#ifndef RADAR_SYSTEM_CONFIGURATION_READER_HPP
#define RADAR_SYSTEM_CONFIGURATION_READER_HPP

#include <boost/any.hpp>
#include <iostream>
#include <cstdint>
#include <cstring>
#include <map>
#include <vector>

namespace sys {
namespace config {
/**
 * @brief The configuration_reader_ class - класс для чтения конфигурации системы
 */
class configuration_reader_ {
public:
    /**
     * @brief configuration_reader_ - конструктор
     * @param cfg_file_ - читаемый файл конфигурации
     */
    explicit configuration_reader_(const std::string cfg_file_ = "");

    /** деструктор */
    ~configuration_reader_() = default;

    /**
     * @brief config_is_read - получение результата чтения конфигурации
     * @return результат чтения конфигурации
     */
    bool config_is_read() const { return this->config_is_read_; }

    /**
     * @brief get_readers - получение словаря читателей сообщений
     * @return словарь читателей сообщений
     */
    std::map<uint16_t,std::vector<std::string>> get_readers() const { return this->map_readers_; }

    /**
     * @brief get_writers - получение словаря писателей сообщений
     * @return словарь писателей сообщений
     */
    std::map<uint16_t,std::vector<std::string>> get_writers() const { return this->map_writers_; }

    /**
     * @brief get_active_components - получение массива активных компонент
     * @return массив активных компонент
     */
    std::vector<std::string> get_active_components() const { return this->active_components_; }

private:
    /**
     * @brief read_configuration - чтение конфигурации системы
     * @param cfg_file_ - файл конфигурации системы
     * @return результат чтения конфигурации системы
     */
    bool read_configuration(const std::string cfg_file_ = "");

    /**
     * @brief read_configuration - чтение конфигурации системы по объекту
     * @param obj - объект конфигурации
     * @return результат чтения
     */
    bool read_configuration(boost::any obj);

private:
    std::map<uint16_t,std::vector<std::string>> map_readers_ = {};     /// <--- словарь читателей сообщений
    std::map<uint16_t,std::vector<std::string>> map_writers_ = {};     /// <--- словарь писателей сообщений
    std::vector<std::string> active_components_ = {};               /// <--- массив активных компонент
    bool config_is_read_ = false;       /// <--- признак того, что конфигурация прочитана
};
}
}

#endif
