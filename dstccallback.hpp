#pragma once
#include <mutex>
#include <functional>
#include <unordered_map>
#include <memory>

#include <iostream> // debug only, should be removed

extern "C" {
    #include "dstc.h"
}

namespace dstc {

    namespace utils {

        template <typename TYPE>
        TYPE parseArg(uint8_t* ptr) {
            TYPE ret_val;
            memcpy(&ret_val, ptr, sizeof(TYPE));
            ptr += sizeof(TYPE);
            return ret_val;
        }

        // Unless there ends up being a cleaner way, we use these macro-like
        // templates to extract data as the correct type so we can use the callback
        // function as appropriate
        template <typename ARG0_T,
                  typename ... OTHERS>
        ARG0_T getArgType0(uint8_t* ptr) {
            return parseArg<ARG0_T>(ptr);
        }

        template <typename ARG0_T,
                  typename ARG1_T,
                  typename ... OTHERS>
        ARG1_T getArgType1(uint8_t* ptr) {
            return parseArg<ARG1_T>(ptr);
        }

        template <typename ARG0_T,
                  typename ARG1_T,
                  typename ARG2_T,
                  typename ... OTHERS>
        ARG2_T getArgType2(uint8_t* ptr) {
            return parseArg<ARG2_T>(ptr);
        }
    } // utils

    void dstcCallbackHandlerRoutine(dstc_callback_t callback_ref,
                                    rmc_node_id_t node_id,
                                    uint8_t *name,
                                    uint8_t* payload,
                                    uint16_t payload_len);

    class CallbackFunctionBase {
    public:
        virtual void execute(uint8_t* payload, uint16_t payload_len) = 0;
        //virtual ~CallbackFunctionBase() = 0;
    };

    template<class ... Types>
    class CallbackFunction : public CallbackFunctionBase{
    public:

        void execute(uint8_t* payload, uint16_t payload_len) override {
            if constexpr (sizeof...(Types) == 1) {
               return _function(utils::getArgType0<Types...>(payload));
            }
            else if constexpr (sizeof...(Types) == 2) {
                return _function(utils::getArgType0<Types...>(payload),
                                 utils::getArgType1<Types...>(payload));
            }
            else if constexpr (sizeof...(Types) == 3) {
                return _function(utils::getArgType0<Types...>(payload),
                                 utils::getArgType1<Types...>(payload),
                                 utils::getArgType2<Types...>(payload));
            }

            static_assert (sizeof...(Types) < 4, "Currently only up to 1 paramater for callbacks in supported, although this can be expanded");

            return;
        }

        CallbackFunction(std::function<void(Types...)> function)
        : _function(function)
        {}
    private:
        std::function<void(Types...)> _function;
    };

    class CallbackHandler {
    public:

        CallbackHandler();

        void execute(dstc_callback_t callback_ref, uint8_t* payload, uint16_t payload_len);

        template<class ... Types>
        dstc_callback_t registerCallback( CallbackFunction<Types...>& callback_function ) {

            auto callback_shared = std::make_shared<CallbackFunction<Types...>>(callback_function);

            std::lock_guard<std::mutex> lock(_callback_mtx);
            auto callback_id = _getUniqueCallbackID();
            _callbacks[callback_id] = callback_shared;
            dstc_activate_callback(nullptr, callback_id, dstcCallbackHandlerRoutine);
            return callback_id;
        }

    private:

        dstc_callback_t _getUniqueCallbackID();

        std::mutex _callback_mtx;
        dstc_callback_t _next_id;
        std::unordered_map<dstc_callback_t, std::shared_ptr<CallbackFunctionBase>> _callbacks;
    };
}