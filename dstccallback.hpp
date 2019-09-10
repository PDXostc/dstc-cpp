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
    };

    namespace utils {

        template <typename TYPE>
        TYPE parseArg(uint8_t** ptr) {
            TYPE ret_val;

            if constexpr (std::is_same<dstc_dynamic_data_t, TYPE>::value) {
                memcpy(&ret_val.length, (*ptr), sizeof(uint16_t));
                (*ptr) += sizeof(uint16_t);
                ret_val.data = (*ptr);
                (*ptr) += ret_val.length;
            }
            else if constexpr (std::is_array<TYPE>::value) {
                memcpy((void*)ret_val, (*ptr), sizeof(TYPE));
                (*ptr) += sizeof(TYPE);
            }
            else if constexpr (std::is_base_of<CallbackFunctionBase, TYPE>::value) {
                memcpy((void*)&ret_val, (*ptr), sizeof(dstc_callback_t));
                (*ptr) += sizeof(dstc_callback_t);
            }
            else {
                memcpy((void*)&ret_val, (*ptr), sizeof(TYPE));
                (*ptr) += sizeof(TYPE);
            }

            return ret_val;
        }

        template<typename TYPE>
        typename std::remove_extent<TYPE>::type* parseArgArray(uint8_t** ptr) {
            auto p = new typename std::remove_extent<TYPE>::type [std::extent<TYPE>::value];
            memcpy((void*) p, (*ptr), sizeof(TYPE));
            (*ptr) += sizeof(TYPE);
            return p;
        }


        // Unless there ends up being a cleaner way, we use these macro-like
        // templates to extract data as the correct type so we can use the callback
        // function as appropriate.  These definitions are generated in gen_getargtypefunctions.py
        template <typename ARG0_T,
                  typename ... OTHERS>
        auto getArgType0(uint8_t** ptr) {
            if constexpr (std::is_array<ARG0_T>::value)
                return parseArgArray<ARG0_T>(ptr);
            else
                return parseArg<ARG0_T>(ptr);
        }

        template <typename ARG0_T,
                  typename ARG1_T,
                  typename ... OTHERS>
        auto getArgType1(uint8_t** ptr) {
            if constexpr (std::is_array<ARG1_T>::value)
                return parseArgArray<ARG1_T>(ptr);
            else
                return parseArg<ARG1_T>(ptr);
        }

        template <typename ARG0_T,
                  typename ARG1_T,
                  typename ARG2_T,
                  typename ... OTHERS>
        auto getArgType2(uint8_t** ptr) {
            if constexpr (std::is_array<ARG2_T>::value)
                return parseArgArray<ARG2_T>(ptr);
            else
                return parseArg<ARG2_T>(ptr);
        }

        template <typename ARG0_T,
                  typename ARG1_T,
                  typename ARG2_T,
                  typename ARG3_T,
                  typename ... OTHERS>
        auto getArgType3(uint8_t** ptr) {
            if constexpr (std::is_array<ARG3_T>::value)
                return parseArgArray<ARG3_T>(ptr);
            else
                return parseArg<ARG3_T>(ptr);
        }

        template <typename ARG0_T,
                  typename ARG1_T,
                  typename ARG2_T,
                  typename ARG3_T,
                  typename ARG4_T,
                  typename ... OTHERS>
        auto getArgType4(uint8_t** ptr) {
            if constexpr (std::is_array<ARG4_T>::value)
                return parseArgArray<ARG4_T>(ptr);
            else
                return parseArg<ARG4_T>(ptr);
        }

        template <typename ARG0_T,
                  typename ARG1_T,
                  typename ARG2_T,
                  typename ARG3_T,
                  typename ARG4_T,
                  typename ARG5_T,
                  typename ... OTHERS>
        auto getArgType5(uint8_t** ptr) {
            if constexpr (std::is_array<ARG5_T>::value)
                return parseArgArray<ARG5_T>(ptr);
            else
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
        auto getArgType6(uint8_t** ptr) {
            if constexpr (std::is_array<ARG6_T>::value)
                return parseArgArray<ARG6_T>(ptr);
            else
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
        auto getArgType7(uint8_t** ptr) {
            if constexpr (std::is_array<ARG7_T>::value)
                return parseArgArray<ARG7_T>(ptr);
            else
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
        auto getArgType8(uint8_t** ptr) {
            if constexpr (std::is_array<ARG8_T>::value)
                return parseArgArray<ARG8_T>(ptr);
            else
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
        auto getArgType9(uint8_t** ptr) {
            if constexpr (std::is_array<ARG9_T>::value)
                return parseArgArray<ARG9_T>(ptr);
            else
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
        auto getArgType10(uint8_t** ptr) {
            if constexpr (std::is_array<ARG10_T>::value)
                return parseArgArray<ARG10_T>(ptr);
            else
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
        auto getArgType11(uint8_t** ptr) {
            if constexpr (std::is_array<ARG11_T>::value)
                return parseArgArray<ARG11_T>(ptr);
            else
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
        auto getArgType12(uint8_t** ptr) {
            if constexpr (std::is_array<ARG12_T>::value)
                return parseArgArray<ARG12_T>(ptr);
            else
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
        auto getArgType13(uint8_t** ptr) {
            if constexpr (std::is_array<ARG13_T>::value)
                return parseArgArray<ARG13_T>(ptr);
            else
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
        auto getArgType14(uint8_t** ptr) {
            if constexpr (std::is_array<ARG14_T>::value)
                return parseArgArray<ARG14_T>(ptr);
            else
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
        auto getArgType15(uint8_t** ptr) {
            if constexpr (std::is_array<ARG15_T>::value)
                return parseArgArray<ARG15_T>(ptr);
            else
                return parseArg<ARG15_T>(ptr);
        }

    } // utils

    template <class T>
    constexpr bool isVarPointer(T&& val) {
        return std::is_pointer<T>::value;
    }

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
                auto arg0 = utils::getArgType0<Types...>(&payload);
                _function(arg0);
                if constexpr (isVarPointer(arg0)) delete[] arg0;
            }
            else if constexpr (sizeof...(Types) == 2) {
                auto arg0 = utils::getArgType0<Types...>(&payload);
                auto arg1 = utils::getArgType1<Types...>(&payload);
                _function(arg0, arg1);
                if constexpr (isVarPointer(arg0)) delete[] arg0;
                if constexpr (isVarPointer(arg1)) delete[] arg1;
            }
            else if constexpr (sizeof...(Types) == 3) {
                auto arg0 = utils::getArgType0<Types...>(&payload);
                auto arg1 = utils::getArgType1<Types...>(&payload);
                auto arg2 = utils::getArgType2<Types...>(&payload);
                _function(arg0, arg1, arg2);
                if constexpr (isVarPointer(arg0)) delete[] arg0;
                if constexpr (isVarPointer(arg1)) delete[] arg1;
                if constexpr (isVarPointer(arg2)) delete[] arg2;
            }
            else if constexpr (sizeof...(Types) == 4) {
                auto arg0 = utils::getArgType0<Types...>(&payload);
                auto arg1 = utils::getArgType1<Types...>(&payload);
                auto arg2 = utils::getArgType2<Types...>(&payload);
                auto arg3 = utils::getArgType3<Types...>(&payload);
                _function(arg0, arg1, arg2, arg3);
                if constexpr (isVarPointer(arg0)) delete[] arg0;
                if constexpr (isVarPointer(arg1)) delete[] arg1;
                if constexpr (isVarPointer(arg2)) delete[] arg2;
                if constexpr (isVarPointer(arg3)) delete[] arg3;
            }
            else if constexpr (sizeof...(Types) == 5) {
                auto arg0 = utils::getArgType0<Types...>(&payload);
                auto arg1 = utils::getArgType1<Types...>(&payload);
                auto arg2 = utils::getArgType2<Types...>(&payload);
                auto arg3 = utils::getArgType3<Types...>(&payload);
                auto arg4 = utils::getArgType4<Types...>(&payload);
                _function(arg0, arg1, arg2, arg3, arg4);
                if constexpr (isVarPointer(arg0)) delete[] arg0;
                if constexpr (isVarPointer(arg1)) delete[] arg1;
                if constexpr (isVarPointer(arg2)) delete[] arg2;
                if constexpr (isVarPointer(arg3)) delete[] arg3;
                if constexpr (isVarPointer(arg4)) delete[] arg4;
            }
            else if constexpr (sizeof...(Types) == 6) {
                auto arg0 = utils::getArgType0<Types...>(&payload);
                auto arg1 = utils::getArgType1<Types...>(&payload);
                auto arg2 = utils::getArgType2<Types...>(&payload);
                auto arg3 = utils::getArgType3<Types...>(&payload);
                auto arg4 = utils::getArgType4<Types...>(&payload);
                auto arg5 = utils::getArgType5<Types...>(&payload);
                _function(arg0, arg1, arg2, arg3, arg4, arg5);
                if constexpr (isVarPointer(arg0)) delete[] arg0;
                if constexpr (isVarPointer(arg1)) delete[] arg1;
                if constexpr (isVarPointer(arg2)) delete[] arg2;
                if constexpr (isVarPointer(arg3)) delete[] arg3;
                if constexpr (isVarPointer(arg4)) delete[] arg4;
                if constexpr (isVarPointer(arg5)) delete[] arg5;
            }
            else if constexpr (sizeof...(Types) == 7) {
                auto arg0 = utils::getArgType0<Types...>(&payload);
                auto arg1 = utils::getArgType1<Types...>(&payload);
                auto arg2 = utils::getArgType2<Types...>(&payload);
                auto arg3 = utils::getArgType3<Types...>(&payload);
                auto arg4 = utils::getArgType4<Types...>(&payload);
                auto arg5 = utils::getArgType5<Types...>(&payload);
                auto arg6 = utils::getArgType6<Types...>(&payload);
                _function(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
                if constexpr (isVarPointer(arg0)) delete[] arg0;
                if constexpr (isVarPointer(arg1)) delete[] arg1;
                if constexpr (isVarPointer(arg2)) delete[] arg2;
                if constexpr (isVarPointer(arg3)) delete[] arg3;
                if constexpr (isVarPointer(arg4)) delete[] arg4;
                if constexpr (isVarPointer(arg5)) delete[] arg5;
                if constexpr (isVarPointer(arg6)) delete[] arg6;
            }
            else if constexpr (sizeof...(Types) == 8) {
                auto arg0 = utils::getArgType0<Types...>(&payload);
                auto arg1 = utils::getArgType1<Types...>(&payload);
                auto arg2 = utils::getArgType2<Types...>(&payload);
                auto arg3 = utils::getArgType3<Types...>(&payload);
                auto arg4 = utils::getArgType4<Types...>(&payload);
                auto arg5 = utils::getArgType5<Types...>(&payload);
                auto arg6 = utils::getArgType6<Types...>(&payload);
                auto arg7 = utils::getArgType7<Types...>(&payload);
                _function(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
                if constexpr (isVarPointer(arg0)) delete[] arg0;
                if constexpr (isVarPointer(arg1)) delete[] arg1;
                if constexpr (isVarPointer(arg2)) delete[] arg2;
                if constexpr (isVarPointer(arg3)) delete[] arg3;
                if constexpr (isVarPointer(arg4)) delete[] arg4;
                if constexpr (isVarPointer(arg5)) delete[] arg5;
                if constexpr (isVarPointer(arg6)) delete[] arg6;
                if constexpr (isVarPointer(arg7)) delete[] arg7;
            }
            else if constexpr (sizeof...(Types) == 9) {
                auto arg0 = utils::getArgType0<Types...>(&payload);
                auto arg1 = utils::getArgType1<Types...>(&payload);
                auto arg2 = utils::getArgType2<Types...>(&payload);
                auto arg3 = utils::getArgType3<Types...>(&payload);
                auto arg4 = utils::getArgType4<Types...>(&payload);
                auto arg5 = utils::getArgType5<Types...>(&payload);
                auto arg6 = utils::getArgType6<Types...>(&payload);
                auto arg7 = utils::getArgType7<Types...>(&payload);
                auto arg8 = utils::getArgType8<Types...>(&payload);
                _function(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
                if constexpr (isVarPointer(arg0)) delete[] arg0;
                if constexpr (isVarPointer(arg1)) delete[] arg1;
                if constexpr (isVarPointer(arg2)) delete[] arg2;
                if constexpr (isVarPointer(arg3)) delete[] arg3;
                if constexpr (isVarPointer(arg4)) delete[] arg4;
                if constexpr (isVarPointer(arg5)) delete[] arg5;
                if constexpr (isVarPointer(arg6)) delete[] arg6;
                if constexpr (isVarPointer(arg7)) delete[] arg7;
                if constexpr (isVarPointer(arg8)) delete[] arg8;
            }
            else if constexpr (sizeof...(Types) == 10) {
                auto arg0 = utils::getArgType0<Types...>(&payload);
                auto arg1 = utils::getArgType1<Types...>(&payload);
                auto arg2 = utils::getArgType2<Types...>(&payload);
                auto arg3 = utils::getArgType3<Types...>(&payload);
                auto arg4 = utils::getArgType4<Types...>(&payload);
                auto arg5 = utils::getArgType5<Types...>(&payload);
                auto arg6 = utils::getArgType6<Types...>(&payload);
                auto arg7 = utils::getArgType7<Types...>(&payload);
                auto arg8 = utils::getArgType8<Types...>(&payload);
                auto arg9 = utils::getArgType9<Types...>(&payload);
                _function(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
                if constexpr (isVarPointer(arg0)) delete[] arg0;
                if constexpr (isVarPointer(arg1)) delete[] arg1;
                if constexpr (isVarPointer(arg2)) delete[] arg2;
                if constexpr (isVarPointer(arg3)) delete[] arg3;
                if constexpr (isVarPointer(arg4)) delete[] arg4;
                if constexpr (isVarPointer(arg5)) delete[] arg5;
                if constexpr (isVarPointer(arg6)) delete[] arg6;
                if constexpr (isVarPointer(arg7)) delete[] arg7;
                if constexpr (isVarPointer(arg8)) delete[] arg8;
                if constexpr (isVarPointer(arg9)) delete[] arg9;
            }
            else if constexpr (sizeof...(Types) == 11) {
                auto arg0 = utils::getArgType0<Types...>(&payload);
                auto arg1 = utils::getArgType1<Types...>(&payload);
                auto arg2 = utils::getArgType2<Types...>(&payload);
                auto arg3 = utils::getArgType3<Types...>(&payload);
                auto arg4 = utils::getArgType4<Types...>(&payload);
                auto arg5 = utils::getArgType5<Types...>(&payload);
                auto arg6 = utils::getArgType6<Types...>(&payload);
                auto arg7 = utils::getArgType7<Types...>(&payload);
                auto arg8 = utils::getArgType8<Types...>(&payload);
                auto arg9 = utils::getArgType9<Types...>(&payload);
                auto arg10 = utils::getArgType10<Types...>(&payload);
                _function(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);
                if constexpr (isVarPointer(arg0)) delete[] arg0;
                if constexpr (isVarPointer(arg1)) delete[] arg1;
                if constexpr (isVarPointer(arg2)) delete[] arg2;
                if constexpr (isVarPointer(arg3)) delete[] arg3;
                if constexpr (isVarPointer(arg4)) delete[] arg4;
                if constexpr (isVarPointer(arg5)) delete[] arg5;
                if constexpr (isVarPointer(arg6)) delete[] arg6;
                if constexpr (isVarPointer(arg7)) delete[] arg7;
                if constexpr (isVarPointer(arg8)) delete[] arg8;
                if constexpr (isVarPointer(arg9)) delete[] arg9;
                if constexpr (isVarPointer(arg10)) delete[] arg10;
            }
            else if constexpr (sizeof...(Types) == 12) {
                auto arg0 = utils::getArgType0<Types...>(&payload);
                auto arg1 = utils::getArgType1<Types...>(&payload);
                auto arg2 = utils::getArgType2<Types...>(&payload);
                auto arg3 = utils::getArgType3<Types...>(&payload);
                auto arg4 = utils::getArgType4<Types...>(&payload);
                auto arg5 = utils::getArgType5<Types...>(&payload);
                auto arg6 = utils::getArgType6<Types...>(&payload);
                auto arg7 = utils::getArgType7<Types...>(&payload);
                auto arg8 = utils::getArgType8<Types...>(&payload);
                auto arg9 = utils::getArgType9<Types...>(&payload);
                auto arg10 = utils::getArgType10<Types...>(&payload);
                auto arg11 = utils::getArgType11<Types...>(&payload);
                _function(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11);
                if constexpr (isVarPointer(arg0)) delete[] arg0;
                if constexpr (isVarPointer(arg1)) delete[] arg1;
                if constexpr (isVarPointer(arg2)) delete[] arg2;
                if constexpr (isVarPointer(arg3)) delete[] arg3;
                if constexpr (isVarPointer(arg4)) delete[] arg4;
                if constexpr (isVarPointer(arg5)) delete[] arg5;
                if constexpr (isVarPointer(arg6)) delete[] arg6;
                if constexpr (isVarPointer(arg7)) delete[] arg7;
                if constexpr (isVarPointer(arg8)) delete[] arg8;
                if constexpr (isVarPointer(arg9)) delete[] arg9;
                if constexpr (isVarPointer(arg10)) delete[] arg10;
                if constexpr (isVarPointer(arg11)) delete[] arg11;
            }
            else if constexpr (sizeof...(Types) == 13) {
                auto arg0 = utils::getArgType0<Types...>(&payload);
                auto arg1 = utils::getArgType1<Types...>(&payload);
                auto arg2 = utils::getArgType2<Types...>(&payload);
                auto arg3 = utils::getArgType3<Types...>(&payload);
                auto arg4 = utils::getArgType4<Types...>(&payload);
                auto arg5 = utils::getArgType5<Types...>(&payload);
                auto arg6 = utils::getArgType6<Types...>(&payload);
                auto arg7 = utils::getArgType7<Types...>(&payload);
                auto arg8 = utils::getArgType8<Types...>(&payload);
                auto arg9 = utils::getArgType9<Types...>(&payload);
                auto arg10 = utils::getArgType10<Types...>(&payload);
                auto arg11 = utils::getArgType11<Types...>(&payload);
                auto arg12 = utils::getArgType12<Types...>(&payload);
                _function(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12);
                if constexpr (isVarPointer(arg0)) delete[] arg0;
                if constexpr (isVarPointer(arg1)) delete[] arg1;
                if constexpr (isVarPointer(arg2)) delete[] arg2;
                if constexpr (isVarPointer(arg3)) delete[] arg3;
                if constexpr (isVarPointer(arg4)) delete[] arg4;
                if constexpr (isVarPointer(arg5)) delete[] arg5;
                if constexpr (isVarPointer(arg6)) delete[] arg6;
                if constexpr (isVarPointer(arg7)) delete[] arg7;
                if constexpr (isVarPointer(arg8)) delete[] arg8;
                if constexpr (isVarPointer(arg9)) delete[] arg9;
                if constexpr (isVarPointer(arg10)) delete[] arg10;
                if constexpr (isVarPointer(arg11)) delete[] arg11;
                if constexpr (isVarPointer(arg12)) delete[] arg12;
            }
            else if constexpr (sizeof...(Types) == 14) {
                auto arg0 = utils::getArgType0<Types...>(&payload);
                auto arg1 = utils::getArgType1<Types...>(&payload);
                auto arg2 = utils::getArgType2<Types...>(&payload);
                auto arg3 = utils::getArgType3<Types...>(&payload);
                auto arg4 = utils::getArgType4<Types...>(&payload);
                auto arg5 = utils::getArgType5<Types...>(&payload);
                auto arg6 = utils::getArgType6<Types...>(&payload);
                auto arg7 = utils::getArgType7<Types...>(&payload);
                auto arg8 = utils::getArgType8<Types...>(&payload);
                auto arg9 = utils::getArgType9<Types...>(&payload);
                auto arg10 = utils::getArgType10<Types...>(&payload);
                auto arg11 = utils::getArgType11<Types...>(&payload);
                auto arg12 = utils::getArgType12<Types...>(&payload);
                auto arg13 = utils::getArgType13<Types...>(&payload);
                _function(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13);
                if constexpr (isVarPointer(arg0)) delete[] arg0;
                if constexpr (isVarPointer(arg1)) delete[] arg1;
                if constexpr (isVarPointer(arg2)) delete[] arg2;
                if constexpr (isVarPointer(arg3)) delete[] arg3;
                if constexpr (isVarPointer(arg4)) delete[] arg4;
                if constexpr (isVarPointer(arg5)) delete[] arg5;
                if constexpr (isVarPointer(arg6)) delete[] arg6;
                if constexpr (isVarPointer(arg7)) delete[] arg7;
                if constexpr (isVarPointer(arg8)) delete[] arg8;
                if constexpr (isVarPointer(arg9)) delete[] arg9;
                if constexpr (isVarPointer(arg10)) delete[] arg10;
                if constexpr (isVarPointer(arg11)) delete[] arg11;
                if constexpr (isVarPointer(arg12)) delete[] arg12;
                if constexpr (isVarPointer(arg13)) delete[] arg13;
            }
            else if constexpr (sizeof...(Types) == 15) {
                auto arg0 = utils::getArgType0<Types...>(&payload);
                auto arg1 = utils::getArgType1<Types...>(&payload);
                auto arg2 = utils::getArgType2<Types...>(&payload);
                auto arg3 = utils::getArgType3<Types...>(&payload);
                auto arg4 = utils::getArgType4<Types...>(&payload);
                auto arg5 = utils::getArgType5<Types...>(&payload);
                auto arg6 = utils::getArgType6<Types...>(&payload);
                auto arg7 = utils::getArgType7<Types...>(&payload);
                auto arg8 = utils::getArgType8<Types...>(&payload);
                auto arg9 = utils::getArgType9<Types...>(&payload);
                auto arg10 = utils::getArgType10<Types...>(&payload);
                auto arg11 = utils::getArgType11<Types...>(&payload);
                auto arg12 = utils::getArgType12<Types...>(&payload);
                auto arg13 = utils::getArgType13<Types...>(&payload);
                auto arg14 = utils::getArgType14<Types...>(&payload);
                _function(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14);
                if constexpr (isVarPointer(arg0)) delete[] arg0;
                if constexpr (isVarPointer(arg1)) delete[] arg1;
                if constexpr (isVarPointer(arg2)) delete[] arg2;
                if constexpr (isVarPointer(arg3)) delete[] arg3;
                if constexpr (isVarPointer(arg4)) delete[] arg4;
                if constexpr (isVarPointer(arg5)) delete[] arg5;
                if constexpr (isVarPointer(arg6)) delete[] arg6;
                if constexpr (isVarPointer(arg7)) delete[] arg7;
                if constexpr (isVarPointer(arg8)) delete[] arg8;
                if constexpr (isVarPointer(arg9)) delete[] arg9;
                if constexpr (isVarPointer(arg10)) delete[] arg10;
                if constexpr (isVarPointer(arg11)) delete[] arg11;
                if constexpr (isVarPointer(arg12)) delete[] arg12;
                if constexpr (isVarPointer(arg13)) delete[] arg13;
                if constexpr (isVarPointer(arg14)) delete[] arg14;
            }
            else if constexpr (sizeof...(Types) == 16) {
                auto arg0 = utils::getArgType0<Types...>(&payload);
                auto arg1 = utils::getArgType1<Types...>(&payload);
                auto arg2 = utils::getArgType2<Types...>(&payload);
                auto arg3 = utils::getArgType3<Types...>(&payload);
                auto arg4 = utils::getArgType4<Types...>(&payload);
                auto arg5 = utils::getArgType5<Types...>(&payload);
                auto arg6 = utils::getArgType6<Types...>(&payload);
                auto arg7 = utils::getArgType7<Types...>(&payload);
                auto arg8 = utils::getArgType8<Types...>(&payload);
                auto arg9 = utils::getArgType9<Types...>(&payload);
                auto arg10 = utils::getArgType10<Types...>(&payload);
                auto arg11 = utils::getArgType11<Types...>(&payload);
                auto arg12 = utils::getArgType12<Types...>(&payload);
                auto arg13 = utils::getArgType13<Types...>(&payload);
                auto arg14 = utils::getArgType14<Types...>(&payload);
                auto arg15 = utils::getArgType15<Types...>(&payload);
                _function(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15);
                if constexpr (isVarPointer(arg0)) delete[] arg0;
                if constexpr (isVarPointer(arg1)) delete[] arg1;
                if constexpr (isVarPointer(arg2)) delete[] arg2;
                if constexpr (isVarPointer(arg3)) delete[] arg3;
                if constexpr (isVarPointer(arg4)) delete[] arg4;
                if constexpr (isVarPointer(arg5)) delete[] arg5;
                if constexpr (isVarPointer(arg6)) delete[] arg6;
                if constexpr (isVarPointer(arg7)) delete[] arg7;
                if constexpr (isVarPointer(arg8)) delete[] arg8;
                if constexpr (isVarPointer(arg9)) delete[] arg9;
                if constexpr (isVarPointer(arg10)) delete[] arg10;
                if constexpr (isVarPointer(arg11)) delete[] arg11;
                if constexpr (isVarPointer(arg12)) delete[] arg12;
                if constexpr (isVarPointer(arg13)) delete[] arg13;
                if constexpr (isVarPointer(arg14)) delete[] arg14;
                if constexpr (isVarPointer(arg15)) delete[] arg15;
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