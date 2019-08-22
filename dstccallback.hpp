// Copyright (C) 2019, Jaguar Land Rover
// This program is licensed under the terms and conditions of the
// Mozilla Public License, version 2.0.  The full text of the
// Mozilla Public License is at https://www.mozilla.org/MPL/2.0/
//
// Author:Steven Martin (smarti24@jaguarlandrover.com)

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

    class CallbackFunctionBase {
    public:
        virtual void execute(uint8_t* payload, uint16_t payload_len) = 0;
        //virtual ~CallbackFunctionBase() = 0;
    };

    namespace utils {

        template <typename TYPE>
        TYPE parseArg(uint8_t* ptr) {
            TYPE ret_val;

            if constexpr (std::is_same<dstc_dynamic_data_t, TYPE>::value) {
                memcpy(&ret_val.length, ptr, sizeof(uint16_t));
                ptr += sizeof(uint16_t);
                ret_val.data = ptr;
                ptr += ret_val.length;
            }
            else if constexpr (std::is_array<TYPE>::value) {
                memcpy((void*)ret_val, ptr, sizeof(TYPE));
                ptr += sizeof(TYPE);
            }
            else if constexpr (std::is_base_of<CallbackFunctionBase, TYPE>::value) {
                memcpy((void*)&ret_val, ptr, sizeof(dstc_callback_t));
                ptr += sizeof(dstc_callback_t);
            }
            else {
                memcpy((void*)&ret_val, ptr, sizeof(TYPE));
                ptr += sizeof(TYPE);
            }

            return ret_val;
        }

        // Unless there ends up being a cleaner way, we use these macro-like
        // templates to extract data as the correct type so we can use the callback
        // function as appropriate.  These definitions are generated in gen_getargtypefunctions.py
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

        template <typename ARG0_T,
                typename ARG1_T,
                typename ARG2_T,
                typename ARG3_T,
                typename ... OTHERS>
        ARG3_T getArgType3(uint8_t* ptr) {
            return parseArg<ARG3_T>(ptr);
        }

        template <typename ARG0_T,
                typename ARG1_T,
                typename ARG2_T,
                typename ARG3_T,
                typename ARG4_T,
                typename ... OTHERS>
        ARG4_T getArgType4(uint8_t* ptr) {
            return parseArg<ARG4_T>(ptr);
        }

        template <typename ARG0_T,
                typename ARG1_T,
                typename ARG2_T,
                typename ARG3_T,
                typename ARG4_T,
                typename ARG5_T,
                typename ... OTHERS>
        ARG5_T getArgType5(uint8_t* ptr) {
            return parseArg<ARG5_T>(ptr);
        }

        template <typename ARG0_T,
                typename ARG1_T,
                typename ARG2_T,
                typename ARG3_T,
                typename ARG4_T,
                typename ARG5_T,
                typename ARG6_T,
                typename ... OTHERS>
        ARG6_T getArgType6(uint8_t* ptr) {
            return parseArg<ARG6_T>(ptr);
        }

        template <typename ARG0_T,
                typename ARG1_T,
                typename ARG2_T,
                typename ARG3_T,
                typename ARG4_T,
                typename ARG5_T,
                typename ARG6_T,
                typename ARG7_T,
                typename ... OTHERS>
        ARG7_T getArgType7(uint8_t* ptr) {
            return parseArg<ARG7_T>(ptr);
        }

        template <typename ARG0_T,
                typename ARG1_T,
                typename ARG2_T,
                typename ARG3_T,
                typename ARG4_T,
                typename ARG5_T,
                typename ARG6_T,
                typename ARG7_T,
                typename ARG8_T,
                typename ... OTHERS>
        ARG8_T getArgType8(uint8_t* ptr) {
            return parseArg<ARG8_T>(ptr);
        }

        template <typename ARG0_T,
                typename ARG1_T,
                typename ARG2_T,
                typename ARG3_T,
                typename ARG4_T,
                typename ARG5_T,
                typename ARG6_T,
                typename ARG7_T,
                typename ARG8_T,
                typename ARG9_T,
                typename ... OTHERS>
        ARG9_T getArgType9(uint8_t* ptr) {
            return parseArg<ARG9_T>(ptr);
        }

        template <typename ARG0_T,
                typename ARG1_T,
                typename ARG2_T,
                typename ARG3_T,
                typename ARG4_T,
                typename ARG5_T,
                typename ARG6_T,
                typename ARG7_T,
                typename ARG8_T,
                typename ARG9_T,
                typename ARG10_T,
                typename ... OTHERS>
        ARG10_T getArgType10(uint8_t* ptr) {
            return parseArg<ARG10_T>(ptr);
        }

        template <typename ARG0_T,
                typename ARG1_T,
                typename ARG2_T,
                typename ARG3_T,
                typename ARG4_T,
                typename ARG5_T,
                typename ARG6_T,
                typename ARG7_T,
                typename ARG8_T,
                typename ARG9_T,
                typename ARG10_T,
                typename ARG11_T,
                typename ... OTHERS>
        ARG11_T getArgType11(uint8_t* ptr) {
            return parseArg<ARG11_T>(ptr);
        }

        template <typename ARG0_T,
                typename ARG1_T,
                typename ARG2_T,
                typename ARG3_T,
                typename ARG4_T,
                typename ARG5_T,
                typename ARG6_T,
                typename ARG7_T,
                typename ARG8_T,
                typename ARG9_T,
                typename ARG10_T,
                typename ARG11_T,
                typename ARG12_T,
                typename ... OTHERS>
        ARG12_T getArgType12(uint8_t* ptr) {
            return parseArg<ARG12_T>(ptr);
        }

        template <typename ARG0_T,
                typename ARG1_T,
                typename ARG2_T,
                typename ARG3_T,
                typename ARG4_T,
                typename ARG5_T,
                typename ARG6_T,
                typename ARG7_T,
                typename ARG8_T,
                typename ARG9_T,
                typename ARG10_T,
                typename ARG11_T,
                typename ARG12_T,
                typename ARG13_T,
                typename ... OTHERS>
        ARG13_T getArgType13(uint8_t* ptr) {
            return parseArg<ARG13_T>(ptr);
        }

        template <typename ARG0_T,
                typename ARG1_T,
                typename ARG2_T,
                typename ARG3_T,
                typename ARG4_T,
                typename ARG5_T,
                typename ARG6_T,
                typename ARG7_T,
                typename ARG8_T,
                typename ARG9_T,
                typename ARG10_T,
                typename ARG11_T,
                typename ARG12_T,
                typename ARG13_T,
                typename ARG14_T,
                typename ... OTHERS>
        ARG14_T getArgType14(uint8_t* ptr) {
            return parseArg<ARG14_T>(ptr);
        }

        template <typename ARG0_T,
                typename ARG1_T,
                typename ARG2_T,
                typename ARG3_T,
                typename ARG4_T,
                typename ARG5_T,
                typename ARG6_T,
                typename ARG7_T,
                typename ARG8_T,
                typename ARG9_T,
                typename ARG10_T,
                typename ARG11_T,
                typename ARG12_T,
                typename ARG13_T,
                typename ARG14_T,
                typename ARG15_T,
                typename ... OTHERS>
        ARG15_T getArgType15(uint8_t* ptr) {
            return parseArg<ARG15_T>(ptr);
        }
    } // utils

    void dstcCallbackHandlerRoutine(dstc_callback_t callback_ref,
                                    rmc_node_id_t node_id,
                                    uint8_t *name,
                                    uint8_t* payload,
                                    uint16_t payload_len);

    template<class ... Types>
    class CallbackFunction : public CallbackFunctionBase{
    public:

        // This function can be automatically generated using the gen_callbackfunction_execute.py
        // script if number of parameters needs to be expanded
        void execute(uint8_t* payload, uint16_t payload_len) override {
            if constexpr (sizeof...(Types) == 0) {
                return _function();
            }
            else if constexpr (sizeof...(Types) == 1) {
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
            else if constexpr (sizeof...(Types) == 4) {
                return _function(utils::getArgType0<Types...>(payload),
                                 utils::getArgType1<Types...>(payload),
                                 utils::getArgType2<Types...>(payload),
                                 utils::getArgType3<Types...>(payload));
            }
            else if constexpr (sizeof...(Types) == 5) {
                return _function(utils::getArgType0<Types...>(payload),
                                 utils::getArgType1<Types...>(payload),
                                 utils::getArgType2<Types...>(payload),
                                 utils::getArgType3<Types...>(payload),
                                 utils::getArgType4<Types...>(payload));
            }
            else if constexpr (sizeof...(Types) == 6) {
                return _function(utils::getArgType0<Types...>(payload),
                                 utils::getArgType1<Types...>(payload),
                                 utils::getArgType2<Types...>(payload),
                                 utils::getArgType3<Types...>(payload),
                                 utils::getArgType4<Types...>(payload),
                                 utils::getArgType5<Types...>(payload));
            }
            else if constexpr (sizeof...(Types) == 7) {
                return _function(utils::getArgType0<Types...>(payload),
                                 utils::getArgType1<Types...>(payload),
                                 utils::getArgType2<Types...>(payload),
                                 utils::getArgType3<Types...>(payload),
                                 utils::getArgType4<Types...>(payload),
                                 utils::getArgType5<Types...>(payload),
                                 utils::getArgType6<Types...>(payload));
            }
            else if constexpr (sizeof...(Types) == 8) {
                return _function(utils::getArgType0<Types...>(payload),
                                 utils::getArgType1<Types...>(payload),
                                 utils::getArgType2<Types...>(payload),
                                 utils::getArgType3<Types...>(payload),
                                 utils::getArgType4<Types...>(payload),
                                 utils::getArgType5<Types...>(payload),
                                 utils::getArgType6<Types...>(payload),
                                 utils::getArgType7<Types...>(payload));
            }
            else if constexpr (sizeof...(Types) == 9) {
                return _function(utils::getArgType0<Types...>(payload),
                                 utils::getArgType1<Types...>(payload),
                                 utils::getArgType2<Types...>(payload),
                                 utils::getArgType3<Types...>(payload),
                                 utils::getArgType4<Types...>(payload),
                                 utils::getArgType5<Types...>(payload),
                                 utils::getArgType6<Types...>(payload),
                                 utils::getArgType7<Types...>(payload),
                                 utils::getArgType8<Types...>(payload));
            }
            else if constexpr (sizeof...(Types) == 10) {
                return _function(utils::getArgType0<Types...>(payload),
                                 utils::getArgType1<Types...>(payload),
                                 utils::getArgType2<Types...>(payload),
                                 utils::getArgType3<Types...>(payload),
                                 utils::getArgType4<Types...>(payload),
                                 utils::getArgType5<Types...>(payload),
                                 utils::getArgType6<Types...>(payload),
                                 utils::getArgType7<Types...>(payload),
                                 utils::getArgType8<Types...>(payload),
                                 utils::getArgType9<Types...>(payload));
            }
            else if constexpr (sizeof...(Types) == 11) {
                return _function(utils::getArgType0<Types...>(payload),
                                 utils::getArgType1<Types...>(payload),
                                 utils::getArgType2<Types...>(payload),
                                 utils::getArgType3<Types...>(payload),
                                 utils::getArgType4<Types...>(payload),
                                 utils::getArgType5<Types...>(payload),
                                 utils::getArgType6<Types...>(payload),
                                 utils::getArgType7<Types...>(payload),
                                 utils::getArgType8<Types...>(payload),
                                 utils::getArgType9<Types...>(payload),
                                 utils::getArgType10<Types...>(payload));
            }
            else if constexpr (sizeof...(Types) == 12) {
                return _function(utils::getArgType0<Types...>(payload),
                                 utils::getArgType1<Types...>(payload),
                                 utils::getArgType2<Types...>(payload),
                                 utils::getArgType3<Types...>(payload),
                                 utils::getArgType4<Types...>(payload),
                                 utils::getArgType5<Types...>(payload),
                                 utils::getArgType6<Types...>(payload),
                                 utils::getArgType7<Types...>(payload),
                                 utils::getArgType8<Types...>(payload),
                                 utils::getArgType9<Types...>(payload),
                                 utils::getArgType10<Types...>(payload),
                                 utils::getArgType11<Types...>(payload));
            }
            else if constexpr (sizeof...(Types) == 13) {
                return _function(utils::getArgType0<Types...>(payload),
                                 utils::getArgType1<Types...>(payload),
                                 utils::getArgType2<Types...>(payload),
                                 utils::getArgType3<Types...>(payload),
                                 utils::getArgType4<Types...>(payload),
                                 utils::getArgType5<Types...>(payload),
                                 utils::getArgType6<Types...>(payload),
                                 utils::getArgType7<Types...>(payload),
                                 utils::getArgType8<Types...>(payload),
                                 utils::getArgType9<Types...>(payload),
                                 utils::getArgType10<Types...>(payload),
                                 utils::getArgType11<Types...>(payload),
                                 utils::getArgType12<Types...>(payload));
            }
            else if constexpr (sizeof...(Types) == 14) {
                return _function(utils::getArgType0<Types...>(payload),
                                 utils::getArgType1<Types...>(payload),
                                 utils::getArgType2<Types...>(payload),
                                 utils::getArgType3<Types...>(payload),
                                 utils::getArgType4<Types...>(payload),
                                 utils::getArgType5<Types...>(payload),
                                 utils::getArgType6<Types...>(payload),
                                 utils::getArgType7<Types...>(payload),
                                 utils::getArgType8<Types...>(payload),
                                 utils::getArgType9<Types...>(payload),
                                 utils::getArgType10<Types...>(payload),
                                 utils::getArgType11<Types...>(payload),
                                 utils::getArgType12<Types...>(payload),
                                 utils::getArgType13<Types...>(payload));
            }
            else if constexpr (sizeof...(Types) == 15) {
                return _function(utils::getArgType0<Types...>(payload),
                                 utils::getArgType1<Types...>(payload),
                                 utils::getArgType2<Types...>(payload),
                                 utils::getArgType3<Types...>(payload),
                                 utils::getArgType4<Types...>(payload),
                                 utils::getArgType5<Types...>(payload),
                                 utils::getArgType6<Types...>(payload),
                                 utils::getArgType7<Types...>(payload),
                                 utils::getArgType8<Types...>(payload),
                                 utils::getArgType9<Types...>(payload),
                                 utils::getArgType10<Types...>(payload),
                                 utils::getArgType11<Types...>(payload),
                                 utils::getArgType12<Types...>(payload),
                                 utils::getArgType13<Types...>(payload),
                                 utils::getArgType14<Types...>(payload));
            }
            else if constexpr (sizeof...(Types) == 16) {
                return _function(utils::getArgType0<Types...>(payload),
                                 utils::getArgType1<Types...>(payload),
                                 utils::getArgType2<Types...>(payload),
                                 utils::getArgType3<Types...>(payload),
                                 utils::getArgType4<Types...>(payload),
                                 utils::getArgType5<Types...>(payload),
                                 utils::getArgType6<Types...>(payload),
                                 utils::getArgType7<Types...>(payload),
                                 utils::getArgType8<Types...>(payload),
                                 utils::getArgType9<Types...>(payload),
                                 utils::getArgType10<Types...>(payload),
                                 utils::getArgType11<Types...>(payload),
                                 utils::getArgType12<Types...>(payload),
                                 utils::getArgType13<Types...>(payload),
                                 utils::getArgType14<Types...>(payload),
                                 utils::getArgType15<Types...>(payload));
            }

            static_assert (sizeof...(Types) <= 16, "Currently only up to 16 paramater for callbacks in supported, although this can be expanded");
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