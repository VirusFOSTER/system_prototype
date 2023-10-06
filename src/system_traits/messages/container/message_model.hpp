#ifndef SYSTEM_MESSAGE_MODEL_HPP
#define SYSTEM_MESSAGE_MODEL_HPP

#include "message_concept.hpp"

namespace sys {
namespace message {
namespace container {
namespace concept {
/**
 * модель хранения типа сообщения
 */
template <class T_message>
class message_model_ : public message_concept_ {
public:
    using message_t = T_message;
    using real_buffer_t = interface::buffer::ISequenceBuffer_<message_t>;
    using buffer_t = typename message_concept_::buffer_t;

    /** конструктор */
    explicit message_model_(const std::string &msg_name) : message_name_(msg_name) {}

    ~message_model_() = default;

    /**
     * @brief message_name - получение наименования сообщения
     * @return наименование сообщения
     */
    std::string message_name() const { return this->message_name_; }

    /**
     * @brief message_id - получение уникального идентификатора сообщения
     * @return уникальный идентификатор сообщения
     */
    uint16_t message_id() const { return this->message_id_; }

    /**
     * @brief set_message_id - установление уникального идентификатора сообщения
     * @param msg_id - уникальный идентификатор сообщения
     */
    void set_message_id(uint16_t msg_id) { this->message_id_ = msg_id; }

    /**
     * @brief init_buffer - инициализация буфера хранения сообщений
     * @param size_buffer - размер буфера
     * @return результат инициализации буфера
     */
    bool init_buffer(uint16_t size_buffer) {
        this->buffer_ = buffer_t(new typename buffer_t::element_type(
                                     new real_buffer_t(this->message_id_,this->message_name_,size_buffer)));
        return this->buffer_ != nullptr;
    }

    /**
     * @brief get_buffer - получение указателя на буфер хранения сообщений
     * @return указатель на буфер хранения сообщений
     */
    buffer_t get_buffer() const { return this->buffer_; }

private:
    std::string message_name_ = "";     /// <--- наименование сообщения
    uint16_t message_id_ = 0;           /// <--- уникальный идентификатор сообщения
    buffer_t buffer_ = nullptr;         /// <--- указатель на буфер хранения сообщения
};
}               /// <--- concept
}           /// <--- container
}       /// <--- message
}   /// <--- sys

#endif
