#include "dstc.hpp"

// Namespace is optional, but its convenient to put all of the DSTC functions in the same place
namespace Remote {
    // This may change slightly, but I think it is easiest for the DSTC name to live in global memory because
    // it is the "ID" that DSTC knows about a specific remote call
    const char print_name_and_age_dstc_name[] = "print_name_and_age";

    // We create a callable object that can be invoked to execute the remote function
    dstc::RemoteFunction <print_name_and_age_dstc_name, char[32], int> printNameAndAge;
}

int main() {
    // Wait for server to come alive
    Remote::printNameAndAge.blockUntilServerAvailable();

    // Queue DSTC command
    char name[] = "Jack O'Lope";
    Remote::printNameAndAge(name, 823);

    // I'll soon put the dstc_process_events in a separate object, but it lives in main for now...
    dstc_process_events(-1);
}