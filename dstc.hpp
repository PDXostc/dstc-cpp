// Copyright (C) 2019, Jaguar Land Rover
// This program is licensed under the terms and conditions of the
// Mozilla Public License, version 2.0.  The full text of the
// Mozilla Public License is at https://www.mozilla.org/MPL/2.0/
//
// Author:Steven Martin (smarti24@jaguarlandrover.com)

#pragma once

extern "C" {
    #include "dstc.h"
}

#include <type_traits>
#include <chrono>
#include <thread>

#include "dstccallback.hpp"
#include "dstceventloop.hpp"

namespace dstc {

    extern CallbackHandler _callback_handler;;

    namespace utils {

        template<typename HEAD>
        uint32_t getArgSize(HEAD head)
        {
            if constexpr (std::is_same<dstc_dynamic_data_t, HEAD>::value) {
                return sizeof(uint16_t) + head.length;
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

        bool serverAvailable() {
            return dstc_remote_function_available_by_name((char*)DSTC_Name);
        }

        bool blockUntilServerAvailable(const EventLoopRunner& runner, int timeout_ms = -1) {

            (void) runner; // cast to avoid not used warning--it does not get used in this function
                           // but the callee must prove they have created the object so that
                           // this function can assume dstc_process_events is being called in the background

            if (timeout_ms < 0) {
                while(!serverAvailable()) {} // block forever if server never comes up
                return true;
            }
            else {
                auto timeout = std::chrono::system_clock::now() + std::chrono::milliseconds(timeout_ms);
                while (true) {
                    if (serverAvailable()) {
                        return true;
                    }
                    if (std::chrono::system_clock::now() >= timeout) {
                        return false;
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }
            }
        }
    };
}
