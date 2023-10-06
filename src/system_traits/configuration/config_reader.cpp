#include "config_reader.hpp"
#include "cfg_type_readers/system_component_config.hpp"
#include "cfg_type_readers/system_messages_config.hpp"

#define FUNCTIONAL_COMPONENTS_CONFIG (std::string)"components_config"
#define MESSAGES_CONFIG (std::string)"messages_config"

using namespace sys;
using namespace config;

configuration_reader_::configuration_reader_(const std::string cfg_file_) {
    this->config_is_read_ = this->read_configuration(cfg_file_);
}

bool configuration_reader_::read_configuration(const std::string cfg_file_) {
    if (cfg_file_.length()) {
        json::loader::JsonLoader jsl(cfg_file_);
        json::object::JsonObject *root = jsl.rootObject();
        if (root->hasProperty("type") && root->hasProperty("files") &&
            root->asString("type") == "radar_system_config") {
            json::array::JsonArray *cfg_array = root->asArray("files");
            for (int32_t i = 0; i < cfg_array->size(); i++) {
                json::object::JsonObject *cfg_object_ = cfg_array->asObject(i);
                if (!this->read_configuration(*cfg_object_)) {
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}

bool configuration_reader_::read_configuration(boost::any obj) {
    auto ptr_object_ = boost::any_cast<json::object::JsonObject>(&obj);
    if (ptr_object_) {
        if (ptr_object_->hasProperty("type") && ptr_object_->hasProperty("path_file")) {
            std::string config_type_ = ptr_object_->asString("type");
            std::string path_ = (std::string)PROJECT_PATH + ptr_object_->asString("path_file");
            if (config_type_ == FUNCTIONAL_COMPONENTS_CONFIG) {
                components_reader_ cr_(path_,config_type_);
                this->active_components_ = cr_.active_components_names();
                return true;
            } else if (config_type_ == MESSAGES_CONFIG) {
                messages_reader_ ms_(path_,config_type_);
                if (!ms_.message_read()) {
                    return false;
                }
                this->map_readers_ = ms_.get_readers();
                this->map_writers_ = ms_.get_writers();
                return true;
            }
        }
    }
    return false;
}
