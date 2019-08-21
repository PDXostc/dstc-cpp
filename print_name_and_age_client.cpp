#include "dstc.hpp"
#include <iostream>
#include <atomic>

// Namespace is optional, but its convenient to put all of the DSTC functions in the same place
namespace Remote {

    namespace names {
        const char double_value_name[] = "double_value";
    }

    dstc::RemoteFunction<names::double_value_name, int, dstc::CallbackFunction< int > > doubleValue;
}

int main() {
    std::cout << "Waiting for server function..." << std::endl;
    Remote::doubleValue.blockUntilServerAvailable();
    std::cout << "Server function available." << std::endl;

    std::atomic<bool> flag(false);

    Remote::doubleValue(12345, dstc::CallbackFunction<int>(
        [&flag](int result) {
            std::cout << "Got value: " << result << std::endl;
            flag = true;
        }
    ));

    while(!flag) {
        dstc_process_events(-1);
    }
}