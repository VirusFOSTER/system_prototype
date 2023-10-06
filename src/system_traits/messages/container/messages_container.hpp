#ifndef SYSTEM_MESSAGES_CONTAINER_HPP
#define SYSTEM_MESSAGES_CONTAINER_HPP

#include "../../../common/common.hpp"
#include "message_model.hpp"

namespace sys {
namespace message {
namespace container {
namespace concept { class message_concept_; }
/**
 * @brief The message_container_ class - контейнер для хранения типов сообщений
 */
class message_container_ {
    using ptr_conc_message_ = boost::shared_ptr<concept::message_concept_>;

public:
    using interface_t = boost::shared_ptr<interface::ISequence_>;

    /** конструктор */
    explicit message_container_() = default;

    /** деструктор */
    ~message_container_() = default;

    /**
     * фиксация нового типа сообщения
     */
    template <class T_message> bool set_new_message(const std::string &message_name) {
        auto itr = std::find_if(this->messages_.begin(),this->messages_.end(),
                                           [ & ](auto &msg){ return msg->message_name() == message_name; });
        if (itr == this->messages_.end()) {
            this->messages_.push_back(ptr_conc_message_(new concept::message_model_<T_message>(message_name)));
            return true;
        }
        return false;
    }

    /**
     * инициализация буфера для внешнего взаимодействия
     */
    template <class T_message>
    interface_t init_free_buffer(const std::string &message_name,
                                 uint16_t size_buffer) {
        auto itr = std::find_if(this->free_messages_.begin(),this->free_messages_.end(),
                                [ & ](auto &msg){ return msg->message_name() == message_name; });
        if (itr == this->free_messages_.end()) {
            this->free_messages_.push_back(ptr_conc_message_(new concept::message_model_<T_message>(message_name)));
            this->free_messages_.back()->set_message_id(this->free_messages_.size());
            if (this->free_messages_.back()->init_buffer(size_buffer)) {
                auto buffer_ = this->free_messages_.back()->get_buffer();
                if (buffer_) {
                    return interface_t(new interface::ISequence_(0,buffer_));
                }
            }
        }
        return nullptr;
    }

    /**
     * @brief get_free_buffer - получение указателя на буфер внешнего взаимодействия
     * @param message_name - наименование сообщения
     * @return указатель на буфер внешнего взаимодействия
     */
    interface_t get_free_interface(const std::string &message_name, uint16_t component_id);

    /**
     * @brief init_interface - инициализация интерфейса взаимодействия двух компонент
     * @param reader_id - идентификатор читателя сообщения
     * @param writer_id - идентификатор писателя сообщения
     * @return указатель на интерфейс взаимодействия двух компонент
     */
    interface_t init_interface(uint16_t component_id, const std::string &msg_name);

    /**
     * @brief init_interface - инициализация интерфейса взаимодействия двух компонент
     * @param reader_id - идентификатор читателя сообщения
     * @param writer_id - идентификатор писателя сообщения
     * @return указатель на интерфейс взаимодействия двух компонент
     */
    interface_t init_interface(uint16_t component_id, uint16_t &msg_id);

    /**
     * @brief init_message_buffer - инициализация буфера для хранения сообщения
     * @param message_id - идентификатор сообщения
     * @param size_buffer - размер буфера сообщения
     * @return результат инициализации буфера сообщения
     */
    bool init_message_buffer(uint16_t message_id, uint16_t size_buffer, const std::string &msg_name);

    /**
     * @brief set_map_readers - установление списка читателей сообщения с идентификатором message_id
     * @param message_id - идентификатор сообщения
     * @param readers список читателей сообщения
     */
    void set_map_readers(uint16_t message_id, std::vector<uint16_t> readers);

    /**
     * @brief set_map_writers - установление списка писателей сообщения с идентификатором message_id
     * @param message_id - идентификатор сообщения
     * @param writers список писателей сообщения
     */
    void set_map_writers(uint16_t message_id, std::vector<uint16_t> writers);

private:
    std::vector<ptr_conc_message_> messages_ = {};      /// <--- массив зарегистрированных сообщений
    std::vector<ptr_conc_message_> free_messages_ = {}; /// <--- массив внешних зарегистрированных сообщений
};  /// <--- message_container_
}           /// <--- container
}       /// <--- message
}   /// <--- sys

extern boost::shared_ptr<sys::message::container::message_container_> msg_container_;

#endif
