// Copyright (C) 2019, Jaguar Land Rover
// This program is licensed under the terms and conditions of the
// Mozilla Public License, version 2.0.  The full text of the
// Mozilla Public License is at https://www.mozilla.org/MPL/2.0/
//
// Author:Steven Martin (smarti24@jaguarlandrover.com)

#include "dstc.h"
#include <stdio.h>
#include <stdlib.h>

#define ARR_SIZE    17

DSTC_SERVER(array_type_one_arg, int, [ ARR_SIZE ]);

int expected_value_start = -2351;

void array_type_one_arg(int arg[ ARR_SIZE ]) {

    for (int i = 0; i < ARR_SIZE; ++i) {
        if (arg[i] != expected_value_start + i) {
            exit(1);
        }
    }

    exit(0);
}

int main (int argc, char* argv[]) {
    if (argc != 2) {
        printf("Did not get expected number of arguments to test server process\n");
        exit(3);
    }
    else {
        expected_value_start = atoi(argv[1]);
    }

    while(1) {
        dstc_process_events(-1);
    }
}