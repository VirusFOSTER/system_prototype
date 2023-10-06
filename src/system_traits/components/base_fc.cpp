#include "base_fc.hpp"

using namespace sys;
using namespace component;
using namespace base_fc;

base_functional_component_::base_functional_component_(uint16_t kmpt_id, const std::string kmpt_name) :
    component_id_(kmpt_id),
    component_name_(kmpt_name){ }

void base_functional_component_::set_reader_interface(reader_interface_t i_reader) {
    if (i_reader) {
        auto new_i_sequence_ = boost::dynamic_pointer_cast<interface::ISequence_>(i_reader);
        if (new_i_sequence_) {
            auto message_name_ = new_i_sequence_->message_interface();
            auto itr = std::find_if(this->i_readers_.begin(),this->i_readers_.end(),
                                    [ & ](auto &ic){
                auto i_sequence_ = boost::dynamic_pointer_cast<interface::ISequence_>(ic);
                if (i_sequence_) {
                    return message_name_ == i_sequence_->message_interface();
                }
                return false;
            });
            if (itr == this->i_readers_.end()) {
                this->i_readers_.push_back(i_reader);
            }
        }
    }
}

void base_functional_component_::set_writer_interface(writer_interface_t i_writer) {
    if (i_writer) {
        auto new_i_sequence_ = boost::dynamic_pointer_cast<interface::ISequence_>(i_writer);
        if (new_i_sequence_) {
            auto message_name_ = new_i_sequence_->message_interface();
            auto itr = std::find_if(this->i_writers_.begin(),this->i_writers_.end(),
                                    [ & ](auto &ic){
                auto i_sequence_ = boost::dynamic_pointer_cast<interface::ISequence_>(ic);
                if (i_sequence_) {
                    return message_name_ == i_sequence_->message_interface();
                }
                return false;
            });
            if (itr == this->i_writers_.end()) {
                this->i_writers_.push_back(i_writer);
            }
        }
    }
}

interface::IReaderSequence_ *base_functional_component_::interface_reader(const std::string message_name) {
    if (!message_name.length()) {
        return nullptr;
    }
    auto itr = std::find_if(this->i_readers_.begin(),this->i_readers_.end(),
                            [ & ](auto &ic){
        auto i_sequence_ = boost::dynamic_pointer_cast<interface::ISequence_>(ic);
        return (i_sequence_) ? i_sequence_->message_interface() == message_name : false;
    });
    return (itr != this->i_readers_.end()) ? (*itr).get() : nullptr;
}

interface::IWriterSequence_ *base_functional_component_::interface_writer(const std::string message_name) {
    if (!message_name.length()) {
        return nullptr;
    }
    auto itr = std::find_if(this->i_writers_.begin(),this->i_writers_.end(),
                            [ & ](auto &ic){
        auto i_sequence_ = boost::dynamic_pointer_cast<interface::ISequence_>(ic);
        return (i_sequence_) ? i_sequence_->message_interface() == message_name : false;
    });
    return (itr != this->i_writers_.end()) ? (*itr).get() : nullptr;
}
