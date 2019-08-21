#pragma once
#include <mutex>
#include <functional>
#include <unordered_map>
#include <memory>

extern "C" {
    #include "dstc.h"
}

namespace dstc {

    class CallbackFunctionBase {
    public:
        //virtual ~CallbackFunctionBase() = 0;
    };

    template<class ... Types>
    class CallbackFunction : public CallbackFunctionBase{
    public:
        CallbackFunction(std::function<void(Types...)> function)
        : _function(function)
        {}
    private:
        std::function<void(Types...)> _function;
    };

    class CallbackHandler {
    public:

        CallbackHandler();

        template<class ... Types>
        dstc_callback_t registerCallback( CallbackFunction<Types...>& callback_function ) {

            auto callback_shared = std::make_shared<CallbackFunction<Types...>>(callback_function);

            std::lock_guard<std::mutex> lock(_callback_mtx);
            auto callback_id = _getUniqueCallbackID();
            _callbacks[callback_id] = callback_shared;
            return callback_id;
        }

    private:

        dstc_callback_t _getUniqueCallbackID();

        std::mutex _callback_mtx;
        dstc_callback_t _next_id;
        std::unordered_map<dstc_callback_t, std::shared_ptr<CallbackFunctionBase>> _callbacks;
    };
}