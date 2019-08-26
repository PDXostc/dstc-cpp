// Copyright (C) 2019, Jaguar Land Rover
// This program is licensed under the terms and conditions of the
// Mozilla Public License, version 2.0.  The full text of the
// Mozilla Public License is at https://www.mozilla.org/MPL/2.0/
//
// Author:Steven Martin (smarti24@jaguarlandrover.com)

#include "dstc.h"
#include <stdio.h>
#include <stdlib.h>

DSTC_SERVER(basic_type_one_arg, int,);

int expected_value = -124819;

void basic_type_one_arg(int value) {
    if (value == expected_value) {
        exit(0);
    }
    else {
        printf("Did not get expected value.\n");
        exit(1);
    }
}

int main (int argc, char* argv[]) {
    if (argc != 2) {
        printf("Did not get expected number of arguments to test server process\n");
        exit(3);
    }
    else {
        expected_value = atoi(argv[1]);
    }

    while(1) {
        dstc_process_events(-1);
    }
}