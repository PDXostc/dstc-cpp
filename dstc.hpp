extern "C" {
    #include "dstc.h"
    extern int dstc_dyndata_length(dstc_dynamic_data_t*);
}

#include <type_traits>

namespace dstc {

    namespace utils {

        template<typename HEAD>
        constexpr uint32_t getArgSize()
        {
            return sizeof(HEAD);
        }

        template<typename HEAD, typename ... TAIL>
        uint32_t _getArgsSize() {
            if constexpr (sizeof...(TAIL) > 0) {
                return getArgSize<HEAD>() + _getArgsSize<TAIL...>();
            }
            else {
                return getArgSize<HEAD>();
            }
        }

        template<typename ... TYPES>
        constexpr uint32_t getArgsSize() {
            if constexpr (sizeof...(TYPES) > 0) {
                return _getArgsSize<TYPES...>();
            }
            else {
                return 0;
            }
        }

        template<typename HEAD, typename ... TAIL>
        void copyArgs(uint8_t* ptr, HEAD head, TAIL ... args) {
            if constexpr (std::is_array<HEAD>::value) {
                memcpy(ptr, (void*)head, sizeof(HEAD));
            }
            else {
                memcpy(ptr, (void*)&head, sizeof(HEAD));
            }
            if constexpr (sizeof...(TAIL) > 0) {
                ptr += sizeof(HEAD);
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

        static uint32_t getArgSize() {
            return utils::getArgsSize<Types...>();
        }

        static int execute(Types... args) {
            auto arg_size = getArgSize();
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