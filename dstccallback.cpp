#include "dstccallback.hpp"
#include <iostream>

namespace dstc {

    CallbackHandler _callback_handler;

    void dstcCallbackHandlerRoutine(dstc_callback_t callback_ref,
                                         rmc_node_id_t node_id,
                                         uint8_t *name,
                                         uint8_t* payload,
                                         uint16_t payload_len) {
                                             std::cout << "In temp_handler: " << callback_ref << std::endl;
                                         }

    CallbackHandler::CallbackHandler()
    : _next_id(1)
    {}



    dstc_callback_t CallbackHandler::_getUniqueCallbackID() {
        if (_next_id == 0) ++_next_id;
        while (_callbacks.count(_next_id) > 0) {
            ++_next_id;
        }
        return _next_id++;
    }
}