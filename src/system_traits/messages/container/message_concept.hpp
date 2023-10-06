#ifndef SYSTEM_MESSAGE_CONCEPT_HPP
#define SYSTEM_MESSAGE_CONCEPT_HPP

#include "../../../common/common.hpp"

namespace sys {
namespace message {
namespace container {
namespace concept {
/**
 * @brief The message_concept_ class - концепция хранения типов сообщений
 */
class message_concept_ {
public:
    using buffer_t = boost::shared_ptr<interface::buffer::ISequenceBufferConcept_>;

    /** конструктор */
    explicit message_concept_() = default;

    /** деструктор */
    virtual ~message_concept_() = default;

    /** получение наименования сообщения */
    virtual std::string message_name() const = 0;

    /** получение уникального идентификатора сообщения */
    virtual uint16_t message_id() const = 0;

    /** установление уникального идентификатора сообщения */
    virtual void set_message_id(uint16_t) = 0;

    /** инициализация буфера для хранения сообщений */
    virtual bool init_buffer(uint16_t) = 0;

    /** получение указателя на буфер хранения сообщения */
    virtual buffer_t get_buffer() const = 0;
};  /// <--- message_concept_
}               /// <--- concept
}           /// <--- container
}       /// <--- message
}   /// <--- sys

#endif
