#include "messages_container.hpp"

using namespace sys;
using namespace message;
using namespace container;

boost::shared_ptr<message_container_> msg_container_(new message_container_);

static bool init_msg_container() {
    if (!msg_container_) {
        msg_container_ = boost::shared_ptr<message_container_>(new message_container_);
    }
    return msg_container_ != nullptr;
}

typename message_container_::interface_t
message_container_::get_free_interface(const std::string &message_name, uint16_t component_id) {
    auto itr = std::find_if(this->free_messages_.begin(),this->free_messages_.end(),
                            [ & ](auto &msg){ return msg->message_name() == message_name; });
    if (itr != this->free_messages_.end()) {
        auto buffer_ = (*itr)->get_buffer();
        if (buffer_) {
            buffer_->push_back_reader(component_id);
            return interface_t(new interface::ISequence_(component_id,buffer_));
        }
    }
    return nullptr;
}

typename message_container_::interface_t
message_container_::init_interface(uint16_t component_id, const std::string &msg_name) {
    auto itr = std::find_if(this->messages_.begin(),this->messages_.end(),
                            [ & ](auto &msg){ return msg->message_name() == msg_name; });
    if (itr != this->messages_.end()) {
        auto buffer_ = (*itr)->get_buffer();
        if (buffer_) {
            return interface_t(new interface::ISequence_(component_id,buffer_));
        }
    }
    return nullptr;
}

typename message_container_::interface_t
message_container_::init_interface(uint16_t component_id, uint16_t &msg_id) {
    auto itr = std::find_if(this->messages_.begin(),this->messages_.end(),
                            [ & ](auto &msg){ return msg->message_id() == msg_id; });
    if (itr != this->messages_.end()) {
        auto buffer_ = (*itr)->get_buffer();
        if (buffer_) {
            return interface_t(new interface::ISequence_(component_id,buffer_));
        }
    }
    return nullptr;
}

bool message_container_::init_message_buffer(uint16_t message_id, uint16_t size_buffer, const std::string &msg_name) {
    auto itr = std::find_if(this->messages_.begin(),this->messages_.end(),
                            [ & ](auto &msg){ return msg->message_name() == msg_name; });
    if (itr != this->messages_.end()) {
        (*itr)->set_message_id(message_id);
        return (*itr)->init_buffer(size_buffer);
    }
    return false;
}

void message_container_::set_map_readers(uint16_t message_id, std::vector<uint16_t> readers) {
    auto itr = std::find_if(this->messages_.begin(),this->messages_.end(),
                            [ & ](auto &msg){ return msg->message_id() == message_id; });
    if (itr != this->messages_.end()) {
        auto buffer_ = (*itr)->get_buffer();
        if (buffer_) {
            buffer_->set_map_readers(readers);
        }
    }
}

void message_container_::set_map_writers(uint16_t message_id, std::vector<uint16_t> writers) {
    auto itr = std::find_if(this->messages_.begin(),this->messages_.end(),
                            [ & ](auto &msg){ return msg->message_id() == message_id; });
    if (itr != this->messages_.end()) {
        auto buffer_ = (*itr)->get_buffer();
        if (buffer_) {
            buffer_->set_map_writers(writers);
        }
    }
}

static bool msg_container_init_ = init_msg_container();
