#include "system_component_config.hpp"
#include "../../components/components_container.hpp"
#include "../../components/base_fc.hpp"

using namespace sys;
using namespace config;

components_reader_::components_reader_(const std::string cfg_file_, const std::string cfg_type) {
    if (cfg_file_.length() && cfg_type.length()) {
        json::loader::JsonLoader jsl(cfg_file_);
        json::object::JsonObject *root_ = jsl.rootObject();
        if (root_ && fc_container_) {
            this->verification_components(root_,cfg_type);
        }
    }
}

void components_reader_::verification_components(json::object::JsonObject *obj_components,
                                                 const std::string cfg_type) {

    if (obj_components && obj_components->hasProperty("type") &&
        obj_components->asString("type") == cfg_type &&
        obj_components->hasProperty("components")) {
        json::array::JsonArray *array_components = obj_components->asArray("components");
        for (int32_t  i = 0; i < array_components->size(); i++) {
            json::object::JsonObject *obj_component_ = array_components->asObject(i);
            if (obj_component_->hasProperty("component_id") &&
                obj_component_->hasProperty("component_name") &&
                obj_component_->hasProperty("active")) {
                std::string fc_name_ = obj_component_->asString("component_name");
                uint16_t fc_id_ = (uint16_t)obj_component_->asInteger("component_id");
                auto fc_ = fc_container_->get_component(fc_name_);
                if (fc_) {
                    if (!obj_component_->asBoolean("active")) {
                        fc_container_->remove_component(fc_name_);
                        continue;
                    }
                    if (this->component_name_dublicate(fc_name_) || this->component_id_dublicate(fc_id_)) {
                        continue;
                    }
                    fc_->set_component_id(fc_id_); fc_->set_component_name(fc_name_);
                    this->active_components_names_.push_back(fc_name_);
                    this->active_components_uids_.push_back(fc_id_);
                }
            }
        }
    }
}

bool components_reader_::component_name_dublicate(const std::string &fc_name_) {
    auto itr = std::find_if(this->active_components_names_.begin(),
                            this->active_components_names_.end(),
                            [ & ](auto &name_){ return name_ == fc_name_; });
    return itr != this->active_components_names_.end();
}

bool components_reader_::component_id_dublicate(const uint16_t &fc_id_) {
    auto itr = std::find_if(this->active_components_uids_.begin(),
                            this->active_components_uids_.end(),
                            [ & ](auto &uid_){ return uid_ == fc_id_; });
    return itr != this->active_components_uids_.end();
}
