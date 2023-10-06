#include "configuration.hpp"
#include "config_reader.hpp"

#include "../components/components_container.hpp"
#include "../components/base_fc.hpp"

using namespace sys;
using namespace config;

configuration_::configuration_(const std::string cfg_file_) {
    this->config_read_ = process_configuration(cfg_file_);
}

bool configuration_::process_configuration(const std::string cfg_file) {
    configuration_reader_ cfg_reader_(cfg_file);
    if (!cfg_reader_.config_is_read()) {
        return false;
    }

    auto readers_ = cfg_reader_.get_readers();
    auto writers_ = cfg_reader_.get_writers();

    if (!this->init_interfaces(readers_,writers_)) {
        return false;
    }

    auto active_components_ = cfg_reader_.get_active_components();
    return this->fix_components(active_components_);
}

bool configuration_::init_interfaces(std::map<uint16_t, std::vector<std::string>> &readers_,
                                     std::map<uint16_t, std::vector<std::string>> &writers_) {
    for (auto wr_itr = writers_.begin(); wr_itr != writers_.end(); wr_itr++) {
        uint16_t message_id = wr_itr->first;
        auto r_itr = readers_.find(message_id);
        if (r_itr != readers_.end()) {
            auto sources_ = wr_itr->second;
            auto targets_ = r_itr->second;
            std::vector<uint16_t> id_sources_ = {};
            std::vector<uint16_t> id_targets_ = {};
            for (auto s_itr = sources_.begin(); s_itr != sources_.end(); s_itr++) {
                auto ptr_sfc_ = fc_container_->get_component(*s_itr);
                if (!ptr_sfc_) {
                    continue;
                }
                bool has_readers_ = false;
                for (auto t_itr = targets_.begin(); t_itr != targets_.end(); t_itr++) {
                    auto ptr_tfc_ = fc_container_->get_component(*t_itr);
                    if (!ptr_tfc_) {
                        continue;
                    }
                    auto new_interface_ = msg_container_->init_interface(ptr_tfc_->component_id(),
                                                                         message_id);
                    if (!new_interface_) {
                        return false;
                    }
                    this->interfaces_.push_back(new_interface_);
                    ptr_tfc_->set_reader_interface(new_interface_);
                    id_targets_.push_back(ptr_tfc_->component_id());
                    has_readers_ = true;
                }
                if (has_readers_) {
                    auto new_interface_ = msg_container_->init_interface(ptr_sfc_->component_id(),
                                                                         message_id);
                    if (!new_interface_) {
                        return false;
                    }
                    this->interfaces_.push_back(new_interface_);
                    ptr_sfc_->set_writer_interface(new_interface_);
                    id_sources_.push_back(ptr_sfc_->component_id());
                }
            }
            msg_container_->set_map_readers(message_id,id_targets_);
            msg_container_->set_map_writers(message_id,id_sources_);
        }
    }
    return true;
}

bool configuration_::fix_components(std::vector<std::string> &fcs_) {
    for (auto &fc_name_ : fcs_) {
        auto ptr_fc_ = fc_container_->get_component(fc_name_);
        if (!ptr_fc_) {
            return false;
        }
        this->components_.push_back(ptr_fc_);
    }
    return true;
}
