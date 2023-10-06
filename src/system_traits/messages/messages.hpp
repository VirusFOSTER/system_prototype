#ifndef SYSTEM_REGISTRATOR_MESSAGES_HPP
#define SYSTEM_REGISTRATOR_MESSAGES_HPP

#include "container/messages_container.hpp"

namespace sys {
namespace message {
namespace registrator {
struct messages_registrator_ {
    explicit messages_registrator_() {
        if (msg_container_) {

        }
    }
};
}           /// <--- registrator
}       /// <--- message
}   /// <--- sys

#endif
