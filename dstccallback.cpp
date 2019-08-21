#include "dstccallback.hpp"
#include <iostream>

namespace dstc {

    CallbackHandler _callback_handler;

    void dstcCallbackHandlerRoutine(dstc_callback_t callback_ref,
                                         rmc_node_id_t node_id,
                                         uint8_t *name,
                                         uint8_t* payload,
                                         uint16_t payload_len) {
                                             _callback_handler.execute(callback_ref, payload, payload_len);
                                         }

    CallbackHandler::CallbackHandler()
    : _next_id(1)
    {}

    void CallbackHandler::execute(dstc_callback_t callback_ref, uint8_t* payload, uint16_t payload_len) {
        std::shared_ptr<CallbackFunctionBase> function_ptr;

        { // lock scope
            std::lock_guard<std::mutex> lock(_callback_mtx);

            auto function_iter = _callbacks.find(callback_ref);
            if (function_iter == _callbacks.end()) {
                std::runtime_error("CallbackHandler received a callback_ref that has not been registered");
            }
            else {
                function_ptr = function_iter->second;
                _callbacks.erase(function_iter);
            }
        } // end scope lock

        if (function_ptr) {
            return function_ptr->execute(payload, payload_len);
        }
    }


    dstc_callback_t CallbackHandler::_getUniqueCallbackID() {
        if (_next_id == 0) ++_next_id;
        while (_callbacks.count(_next_id) > 0) {
            ++_next_id;
        }
        return _next_id++;
    }
}