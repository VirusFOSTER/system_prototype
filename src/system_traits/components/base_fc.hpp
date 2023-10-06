#ifndef SYSTEM_BASE_FUNCTIONAL_COMPONENT_HPP
#define SYSTEM_BASE_FUNCTIONAL_COMPONENT_HPP

#include "../../common/common.hpp"
#include <thread>
#include <chrono>

namespace sys {
namespace component {
namespace base_fc {
/**
 * @brief The base_functional_component_ class - базовая функциональная компонента
 * По сути от этого класса должны наследоваться все основные компоненты системы
 * Здесь имеется возможность получения указателя на интерфейсы взаимодействия
 */
class base_functional_component_ {
    using reader_interface_t = boost::shared_ptr<interface::IReaderSequence_>;
    using writer_interface_t = boost::shared_ptr<interface::IWriterSequence_>;

public:
    /** Конструктор */
    explicit base_functional_component_(uint16_t kmpt_id = 0, const std::string kmpt_name = "");

    /** Деструктор */
    ~base_functional_component_() = default;

    /**
     * @brief init - инициализация компоненты
     */
    virtual bool init() = 0;

    /**
     * @brief run - головная процедура нити
     */
    virtual void run() = 0;

    /**
     * @brief set_component_id - установление уникального идентификатора компоненты
     * @param id_ - уникальный идентификатор компоненты
     */
    void set_component_id(uint16_t id_) { this->component_id_ = id_; }

    /**
     * @brief set_component_name - установление наименования компоненты
     * @param name_ - наименование компоненты
     */
    void set_component_name(const std::string name_) { this->component_name_ = name_; }

    /**
     * @brief set_reader_interface - добавление интерфейса для чтения сообщений
     * @param i_reader интерфейс для чтения сообщений
     */
    void set_reader_interface(reader_interface_t i_reader);

    /**
     * @brief set_writer_interface - добавление интерфейса для добавления сообщений
     * @param i_writer интерфейс писателя сообщений
     */
    void set_writer_interface(writer_interface_t i_writer);

    /**
     * @brief component_name - получение наименования компоненты
     */
    std::string component_name() const { return this->component_name_; }

    /**
     * @brief component_id - уникальный идентификатор компоненты
     */
    uint16_t component_id() const { return this->component_id_; }

protected:
    /**
     * @brief get_interface_reader - получение указателя на интерфейс чтения сообщений message_name
     * @param message_name - наименование сообщения
     * @return указатель на интерфейс для чтения сообщений
     */
    interface::IReaderSequence_ *interface_reader(const std::string message_name);

    /**
     * @brief interface_writer - получение указателя на интерфейс писателя сообщений message_name
     * @param message_name - наименование сообщения
     * @return указатель на интерфейс писателя
     */
    interface::IWriterSequence_ *interface_writer(const std::string message_name);

protected:
    uint16_t component_id_ = 0;             /// <--- уникальный идентификатор компоненты
    std::string component_name_ = "";       /// <--- уникальное наименование компоненты
    std::list<reader_interface_t> i_readers_ = {};  /// <--- список интерфейсов читателя для компоненты
    std::list<writer_interface_t> i_writers_ = {};  /// <--- список интерфейсов писателя для компоненты
};
}           /// <--- base_fc
}       /// <--- component
}   /// <--- sys

#endif
