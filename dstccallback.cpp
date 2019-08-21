#include "dstccallback.hpp"

namespace dstc {

    CallbackHandler::CallbackHandler()
    : _next_id(0)
    {}

    CallbackHandler _callback_handler;

    dstc_callback_t CallbackHandler::_getUniqueCallbackID() {
        while (_callbacks.count(_next_id) > 0) {
            ++_next_id;
        }
        return _next_id++;
    }
}