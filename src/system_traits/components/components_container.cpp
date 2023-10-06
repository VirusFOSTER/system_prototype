#include "components_container.hpp"
#include "base_fc.hpp"

using namespace sys;
using namespace component;

boost::shared_ptr<components_container_> fc_container_ = nullptr;

static bool init_fc_container() {
    if (!fc_container_) {
        fc_container_ = boost::shared_ptr<components_container_>(new components_container_);
    }
    return fc_container_ != nullptr;
}

components_container_::components_container_() { }

bool components_container_::component_registration(ptr_component_t kmpt, const std::string kmpt_name){
    boost::lock_guard<boost::mutex> lock_(this->mtx_);
    if (kmpt && kmpt_name.length()) {
        auto itr = this->components_.find(kmpt_name);
        if (itr == this->components_.end()) {
            this->components_.insert(std::make_pair(kmpt_name,kmpt));
            return true;
        }
    }

    return false;
}

components_container_::ptr_component_t components_container_::get_component(const std::string kmpt_name) {
    boost::lock_guard<boost::mutex> lock_(this->mtx_);
    auto itr = this->components_.find(kmpt_name);
    return (itr != this->components_.end()) ? itr->second : nullptr;
}

std::string components_container_::get_component_name(const uint16_t kmpt_id) {
    boost::lock_guard<boost::mutex> lock_(this->mtx_);
    for (auto itr = this->components_.begin(); itr != this->components_.end(); itr++) {
        auto ptr_component_ = itr->second;
        if (kmpt_id == ptr_component_->component_id()) {
            return itr->first;
        }
    }
    return "";
}

void components_container_::remove_component(const std::string kmpt_name) {
    auto itr = this->components_.find(kmpt_name);
    if (itr != this->components_.end()) {
        this->components_.erase(itr);
    }
}

static bool fc_container_init_ = init_fc_container();
