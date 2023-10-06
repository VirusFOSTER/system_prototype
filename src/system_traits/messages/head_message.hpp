#ifndef RADAR_SYSTEM_MESSAGE_HEAD_HPP
#define RADAR_SYSTEM_MESSAGE_HEAD_HPP

#include <cstdint>
#include <iostream>
#include <cstring>
#include <ctime>
#include "../../common/json/json.hpp"

namespace sys {
namespace message {
using hrtime_t = uint64_t;

/**
 * @brief The message_head_ struct - заголовок сообщения
 */
struct message_head_ {
    uint16_t message_uid_ = 0;          /// <--- уникальный идентификатор сообщения
    uint16_t message_source_ = 0;       /// <--- уникальный идентификатор источника сообщений
    uint32_t message_id_ = 0;           /// <--- порядковый номер сообщения
    hrtime_t message_time_ = 0;         /// <--- время создания сообщения (нс)

    std::string pack(json::out::JsonStringOutput &out_header_) {

        out_header_.startObject("head");
        out_header_.integerProperty("uid", this->message_uid_);
        out_header_.integerProperty("source", this->message_source_);
        out_header_.integerProperty("id", this->message_id_);
        out_header_.integerProperty("time", this->message_time_);
        out_header_.end();

        return out_header_.result();
    }
};  /// <--- message_head_
}       /// <--- message
}   /// <--- sys

#endif
