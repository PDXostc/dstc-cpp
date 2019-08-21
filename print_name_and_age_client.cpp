#include "dstc.hpp"
#include <iostream>

// Namespace is optional, but its convenient to put all of the DSTC functions in the same place
namespace Remote {
    // This may change slightly, but I think it is easiest for the DSTC name to live in global memory because
    // it is the "ID" that DSTC knows about a specific remote call
    //const char print_name_and_age_dstc_name[] = "print_name_and_age";

    // We create a callable object that can be invoked to execute the remote function
    //dstc::RemoteFunction <print_name_and_age_dstc_name, char[32], int> printNameAndAge;

    //const char test_dynamic_function_name[] = "test_dynamic_function";
    //dstc::RemoteFunction<test_dynamic_function_name, dstc_dynamic_data_t, int[4]> testDynamicFunctionName;

    const char double_value_name[] = "double_value";
    dstc::RemoteFunction<double_value_name, int, dstc::CallbackFunction< int > > doubleValue;
}

int main() {

    // Wait for server to come alive
    //Remote::printNameAndAge.blockUntilServerAvailable();
    //std::cout << "Waiting for server function..." << std::endl;
    //Remote::testDynamicFunctionName.blockUntilServerAvailable();
    //std::cout << "Ready." << std::endl;

    //char payload[] = "blah blah data blammo this thing";
    //dstc_dynamic_data_t data;
    //data.data = (void*)payload;
    //data.length = strlen(payload) + 1;

    //int array[4] = {2, 4, 6, 8};

    //Remote::testDynamicFunctionName(data, array);

    // Queue DSTC command
    //char name[] = "Jack O'Lope";
    ///Remote::printNameAndAge(name, 823);

    std::cout << "Waiting for server function..." << std::endl;
    Remote::doubleValue.blockUntilServerAvailable();
    std::cout << "Server function available." << std::endl;

    Remote::doubleValue(1234,
        dstc::CallbackFunction<int>(
            [](int result) { std::cout << "Got value: " << result; }
        )
    );

    /*
    Remote::doubleValue(

    )
    */

    // I'll soon put the dstc_process_events in a separate object, but it lives in main for now...
    dstc_process_events(-1);
}