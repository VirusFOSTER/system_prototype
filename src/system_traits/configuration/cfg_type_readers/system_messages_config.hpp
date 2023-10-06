#ifndef RADAR_SYSTEM_MESSAGES_CONFIGURATION_HPP
#define RADAR_SYSTEM_MESSAGES_CONFIGURATION_HPP

#include "../../../common/json/json.hpp"

namespace sys {
namespace config {
/**
 * @brief The messages_reader_ class - класс для получения информации о сообщениях системы
 */
class messages_reader_ {
public:
    /** конструктор */
    explicit messages_reader_(const std::string cfg_file_ = "", const std::string cfg_type = "");

    /** деструктор */
    ~messages_reader_() = default;

    /**
     * @brief get_readers - получение идентификаторов читателей сообщений
     * @return словарь читателей сообщений
     */
    std::map<uint16_t,std::vector<std::string>> get_readers() const { return this->map_readers_; }

    /**
     * @brief get_writers - получение идентификаторов писателей сообщений
     * @return словарь писателей сообщений
     */
    std::map<uint16_t,std::vector<std::string>> get_writers() const { return this->map_writers_; }

    /**
     * @brief message_read - получение признака чтения конфигурации
     * @return результат чтения конфигурации
     */
    bool message_read() const { return this->messages_read_; }

private:
    /**
     * @brief read_messages_configuration - чтение конфигурации сообщений системы
     * @param main_cfg_ - указатель на головную конфигурацию сообщений системы
     * @return результат чтения конфигурации
     */
    bool read_messages_configuration(json::object::JsonObject *main_cfg_, const std::string cfg_type);

    /**
     * @brief read_message_configuration - чтение конфигурации отдельно взятого сообщения
     * @param msg_obj_ - объект конфигурации отдельно взятого сообщения
     * @return результат чтения
     */
    bool read_message_configuration(json::object::JsonObject *msg_obj_);

    /**
     * @brief verification_message_confiuration - верификация конфигурации отдельно взятого сообщения
     * @param msg_obj_ - объект конфигурации отдельно взятого сообщения
     * @return результат верификации
     */
    bool verification_message_configuration(json::object::JsonObject *msg_obj_);

    /**
     * @brief fill_messages_readers - заполнение массива читателей сообщений
     */
    void fill_messages_readers(uint16_t msg_id, json::array::JsonArray *ar_sources_);

    /**
     * @brief fill_messages_writers - заполнение массива писателей сообщений
     */
    void fill_messages_writers(uint16_t msg_id,json::array::JsonArray *ar_writers_);

protected:
    bool messages_read_ = false;    /// <--- признак удачного чтения конфигурации сообщений
    std::map<uint16_t,std::vector<std::string>> map_readers_ = {}; /// <--- карта читателей сообщений
    std::map<uint16_t,std::vector<std::string>> map_writers_ = {}; /// <--- карта писателей сообщений
};  /// <--- messages_reader_
}       /// <--- config
}   /// <--- sys

#endif
