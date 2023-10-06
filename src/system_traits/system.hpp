#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "task_processor/task_processor.hpp"
#include "messages/messages.hpp"
#include "configuration/configuration.hpp"

namespace sys {
/**
 * @brief The radar_system_ class - класс, описывающий работу радара (системы)
 */
class system_ :
        public message::registrator::messages_registrator_,
        public config::configuration_,
        public process::task_processor {
public:
    using interface_t = boost::shared_ptr<interface::ISequence_>;

    /**
     * @brief The work_type_ enum - тип работы системы
     */
    enum class work_type {
        Tp_CRASH = 0,       /// <--- система не инициализирвана
        Tp_START = 1,       /// <--- система инициализирована и начала работу
        Tp_STOP  = 2        /// <--- система остановлена
    };

    /**
     * @brief radar_system_ - конструктор
     * @param cfg_file_ - путь к файлу конфигурации системы
     */
    explicit system_(const std::string system_name, const std::string cfg_file_ = "");

    /** Деструктор */
    ~system_() = default;

    /**
     * инициализация буфера для внешнего взаимодействия
     */
    template <class T_data>
    interface_t init_free_buffer(const std::string &message_name, uint16_t size_buffer) {
        return msg_container_->init_free_buffer<T_data>(message_name,size_buffer);
    }

    /**
     * @brief run - запуск системы радара
     */
    void run();

    /**
     * @brief stop - остановка системы радара
     */
    void stop();

    /**
     * @brief system_stopped - получение признака того, что система остановлена
     * @return признак остановки системы
     */
    bool system_stopped();

private:
    /**
     * @brief init_system - инициализация системы
     * @return результат инициализации системы
     */
    bool init_system();

private:
    bool system_init_ = false;                      /// <--- признак инициализации системы
    std::string system_name_ = "";                  /// <--- наименование процесса
    bool rada_properties_init_ = false;             /// <--- признак получения свойств радара
    work_type work_type_ = work_type::Tp_CRASH;     /// <--- текущее состояние системы
    boost::mutex mtx_;                              /// <--- мьютекс доступа к общим ресурсам системы
};
}   /// <--- sys

#endif
