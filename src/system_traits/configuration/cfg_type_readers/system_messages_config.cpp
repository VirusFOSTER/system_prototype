#include "system_messages_config.hpp"
#include "../../messages/container/messages_container.hpp"
#include "../../components/components_container.hpp"

using namespace sys;
using namespace config;

messages_reader_::messages_reader_(const std::string cfg_file_, const std::string cfg_type) {
    if (cfg_file_.length() && cfg_type.length() && fc_container_ && msg_container_) {
        json::loader::JsonLoader jsl(cfg_file_);
        json::object::JsonObject *root = jsl.rootObject();
        this->messages_read_ = this->read_messages_configuration(root,cfg_type);
    }
}

bool messages_reader_::read_messages_configuration(json::object::JsonObject *main_cfg_, const std::string cfg_type) {
    if (main_cfg_ && main_cfg_->hasProperty("type") &&
        main_cfg_->asString("type") == cfg_type &&
        main_cfg_->hasProperty("messages")) {
        json::array::JsonArray *ar_messages = main_cfg_->asArray("messages");
        for (int32_t i = 0; i < ar_messages->size(); i++) {
            json::object::JsonObject *msg_object_ = ar_messages->asObject(i);
            if (this->read_message_configuration(msg_object_)) {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool messages_reader_::read_message_configuration(json::object::JsonObject *msg_obj_) {
    if (this->verification_message_configuration(msg_obj_)) {
        uint16_t message_id_ = msg_obj_->asInteger("message_id");
        std::string message_name_ = msg_obj_->asString("message_name");
        uint16_t buffer_size_ = msg_obj_->asInteger("size_buffer");
        if (msg_container_->init_message_buffer(message_id_,buffer_size_,message_name_)){
            json::array::JsonArray *fcs = msg_obj_->asArray("target");
            this->fill_messages_readers(message_id_,fcs);
            fcs = msg_obj_->asArray("source");
            this->fill_messages_writers(message_id_,fcs);
        }
    }
    return false;
}

bool messages_reader_::verification_message_configuration(json::object::JsonObject *msg_obj_) {
    return msg_obj_ && msg_obj_->hasProperty("message_id") &&
            msg_obj_->hasProperty("message_name") &&
            msg_obj_->hasProperty("alt_message_name") &&
            msg_obj_->hasProperty("size_buffer") &&
            msg_obj_->hasProperty("source") &&
            msg_obj_->hasProperty("target");
}

void messages_reader_::fill_messages_readers(uint16_t msg_id,json::array::JsonArray *ar_sources_) {
    if (ar_sources_) {
        std::vector<std::string> readers_ = {};
        for (int32_t i = 0; i < ar_sources_->size(); i++) {
            auto ptr_fc_ = fc_container_->get_component(ar_sources_->asString(i));
            if (ptr_fc_) {
                readers_.push_back(ar_sources_->asString(i));
            }
        }
        this->map_readers_.insert(std::make_pair(msg_id,readers_));
    }
}

void messages_reader_::fill_messages_writers(uint16_t msg_id, json::array::JsonArray *ar_writers_) {
    if (ar_writers_) {
        std::vector<std::string> writers_ = {};
        for (int32_t i = 0; i < ar_writers_->size(); i++) {
            auto ptr_fc_ = fc_container_->get_component(ar_writers_->asString(i));
            if (ptr_fc_) {
                writers_.push_back(ar_writers_->asString(i));
            }
        }
        this->map_writers_.insert(std::make_pair(msg_id,writers_));
    }
}
