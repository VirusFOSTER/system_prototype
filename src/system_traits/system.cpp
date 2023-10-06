#include "system.hpp"
#include "components/components_container.hpp"
#include "components/base_fc.hpp"

using namespace sys;

system_::system_(const std::string system_name,
                             const std::string cfg_file_) :
    config::configuration_(cfg_file_) {
    this->system_init_ = this->init_system();
}

bool system_::init_system() {
    if (this->config_read_) {
        for (auto &fc : this->components_) {
            if (!this->push_task(fc)) {
                return false;
            }
        }
        return true;
    }
    return false;
}

void system_::run() {
    if (this->system_init_) {
        this->work_type_ = work_type::Tp_START;
        this->start();
    }
}

void system_::stop() {
    boost::lock_guard<boost::mutex> lock_(this->mtx_);
    this->work_type_ == work_type::Tp_STOP;
}

bool system_::system_stopped() {
    boost::lock_guard<boost::mutex> lock_(this->mtx_);
    return this->work_type_ == work_type::Tp_STOP;
}
