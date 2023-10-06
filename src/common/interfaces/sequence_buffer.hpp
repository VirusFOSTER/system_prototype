#ifndef SYSTEM_SEQUENCE_BUFFER_HPP
#define SYSTEM_SEQUENCE_BUFFER_HPP

#include <deque>
#include <boost/type_traits.hpp>
#include <boost/core/enable_if.hpp>

#include "../type_traits/types/properties/member_type_detector.hpp"
#include "../type_traits/types/properties/member_detector.hpp"
#include "../../system_traits/messages/head_message.hpp"

GENERATE_HAS_MEMBER_TYPE(message_head_)
GENERATE_REQUIRED_MEMBER_CLASS(head_)

namespace sys {
namespace interface {
namespace buffer {
/**
 * @brief The ISequenceBuffer_ class - класс-буфер последовательности сообщений
 */
template <class T_message>
class ISequenceBuffer_  {
public:
    using message_t = T_message;
    using message_iter_t = typename std::deque<message_t>::iterator;
    /**
     * @brief ISequenceBuffer_ - конструктор
     * @param message_id - уникальный идентификатор сообщения
     * @param size_buffer - размер буфера сообщений
     */
    explicit ISequenceBuffer_(const uint16_t message_id, const std::string message_name, const uint16_t size_buffer) :
        message_id_(message_id), message_name_(message_name), size_buffer_(size_buffer) { }

    /** Деструктор */
    ~ISequenceBuffer_() = default;

    /**
     * @brief add_new_element - добавление нового элемента в буфер
     * @return результат добавления
     */
    bool add_new_element(message_t &msg) {
        if (this->control_reader_ && this->messages_.size() == this->size_buffer_) {
            return false;
        }

        this->messages_.push_back(msg);
        while (this->messages_.size() > this->size_buffer_) {
            this->messages_.pop_front();
        }

        this->count_messages_++;

        return true;
    }

    /**
     * @brief get_element - получение очередного сообщения из буфера
     * @param numb_msg - порядковый глобальный номер из буфера
     * @return указатель на элемент буфера
     */
    message_t *get_element(uint64_t numb_msg) {
        if (numb_msg < this->count_messages_) {
            uint64_t d_msg_ = this->count_messages_ - numb_msg;
            uint64_t off_msg_ = (d_msg_ >= this->size_buffer_) ? 0 : this->messages_.size() - d_msg_;
            auto itr = this->messages_.begin();
            std::advance(itr,off_msg_);
            return &(*itr);
        }

        return nullptr;
    }

    /**
     * @brief delete_element - удаление сообщения из очереди
     * @param numb_msg - номер удаляемого сообщения
     * @return результат удаления
     */
    bool delete_element(uint16_t numb_msg) {
        if (numb_msg < this->messages_.size()) {
            auto itr = this->messages_.begin();
            std::advance(itr,numb_msg);
            this->messages_.erase(itr);

            return true;
        }

        return false;
    }

    /**
     * @brief message_id - получение уникального идентификатора сообщения
     * @return уникальный идентификатор сообщения
     */
    uint16_t message_id() const { return this->message_id_; }

    /**
     * @brief message_name - получение наименования сообщения
     * @return наименование сообщения
     */
    std::string message_name() const { return this->message_name_; }

    /**
     * @brief messages_count - получение количества записанных в буфер сообщений
     * @return количество записанных в буфер сообщений
     */
    uint64_t messages_count() const { return this->count_messages_; }

private:
    uint16_t message_id_ = 0;               /// <--- уникальный идентификатор сообщения
    std::string message_name_ = "";         /// <--- наименование сообщения
    uint16_t size_buffer_ = 0;              /// <--- размер буфера сообщений
    uint64_t count_messages_ = 0;           /// <--- общее количество записанных сообщений в буфер
    std::deque<message_t> messages_ = {};   /// <--- очередь сообщений
    bool control_reader_ = false;           /// <--- признак того, что все сообщения должны быть прочитаны
};
}           /// <--- buffer
}       /// <--- interface
}   /// <--- sys

#endif
