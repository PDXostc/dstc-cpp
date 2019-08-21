#pragma once

extern "C" {
    #include "dstc.h"
    //extern int dstc_dyndata_length(dstc_dynamic_data_t*);
}

#include <type_traits>
#include <iostream> // should be removed after library is debugged and stable

#include "dstccallback.hpp"

namespace dstc {

    extern CallbackHandler _callback_handler;;

    namespace utils {

        template<typename HEAD>
        uint32_t getArgSize(HEAD head)
        {
            if constexpr (std::is_same<dstc_dynamic_data_t, HEAD>::value) {
                return sizeof(uint32_t) + head.length;
            }
            else {
                return sizeof(HEAD);
            }
        }

        template<typename HEAD, typename ... TAIL>
        uint32_t _getArgsSize(HEAD head, TAIL ... args) {
            if constexpr (sizeof...(TAIL) > 0) {
                return getArgSize<HEAD>(head) + _getArgsSize<TAIL...>(args...);
            }
            else {
                return getArgSize<HEAD>(head);
            }
        }

        template<typename ... TYPES>
        uint32_t getArgsSize(TYPES ... args) {
            if constexpr (sizeof...(TYPES) > 0) {
                return _getArgsSize<TYPES...>(args...);
            }
            else {
                return 0;
            }
        }

        template<typename HEAD, typename ... TAIL>
        void copyArgs(uint8_t* ptr, HEAD head, TAIL ... args) {

            if constexpr (std::is_same<dstc_dynamic_data_t, HEAD>::value) {
                memcpy(ptr, (void*) &head.length, sizeof(uint16_t));
                ptr += sizeof(uint16_t);
                memcpy(ptr, head.data, head.length);
                ptr += head.length;
            }
            else if constexpr (std::is_array<HEAD>::value) {
                memcpy(ptr, (void*)head, sizeof(HEAD));
                ptr += sizeof(HEAD);
            }
            else if constexpr (std::is_base_of<CallbackFunctionBase, HEAD>::value) {
                auto callback_id = _callback_handler.registerCallback(head);
                memcpy(ptr, (void*)&callback_id, sizeof(callback_id));
                ptr += sizeof(callback_id);
            }
            else {
                memcpy(ptr, (void*)&head, sizeof(HEAD));
                ptr += sizeof(HEAD);
            }
            if constexpr (sizeof...(TAIL) > 0) {
                return copyArgs<TAIL...>(ptr, args...);
            }
        }
    }

    template<const char* DSTC_Name, class ... Types>
    class RemoteFunction {
    public:
        RemoteFunction() {
            dstc_register_client_function(NULL, (char*) DSTC_Name, (void*)RemoteFunction::execute);
        }

        int operator()(Types ... args) {
            return execute(args...);
        }

        static uint32_t getArgSize(Types ... args) {
            return utils::getArgsSize<Types...>(args...);
        }

        static int execute(Types... args) {
            auto arg_size = getArgSize(args...);
            uint8_t arg_buf[arg_size];
            uint8_t* ptr = arg_buf;

            if constexpr (sizeof...(Types) > 0) {
                utils::copyArgs<Types...>(ptr, args...);
            }

            return dstc_queue_func(nullptr, (char*)DSTC_Name, arg_buf, arg_size);
        }

        void blockUntilServerAvailable() {
            while (!dstc_remote_function_available_by_name((char*)DSTC_Name)) {
                 dstc_process_events(-1);
            }
        }
    };
}
