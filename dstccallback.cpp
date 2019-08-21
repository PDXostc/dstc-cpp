#include "dstccallback.hpp"
#include <iostream>

namespace dstc {

    void temp_handler (dstc_callback_t callback_ref,
                                         rmc_node_id_t node_id,
                                         uint8_t *name,
                                         uint8_t* payload,
                                         uint16_t payload_len) {
                                             std::cout << "In temp_handler" << std::endl;
                                         }

    CallbackFunctionBase::CallbackFunctionBase() {
        dstc_activate_callback(nullptr, 1, temp_handler);
    }

    CallbackHandler::CallbackHandler()
    : _next_id(1)
    {}

    CallbackHandler _callback_handler;

    dstc_callback_t CallbackHandler::_getUniqueCallbackID() {
        if (_next_id == 0) ++_next_id;
        while (_callbacks.count(_next_id) > 0) {
            ++_next_id;
        }
        return _next_id++;
    }
}