// Copyright (C) 2019, Jaguar Land Rover
// This program is licensed under the terms and conditions of the
// Mozilla Public License, version 2.0.  The full text of the
// Mozilla Public License is at https://www.mozilla.org/MPL/2.0/
//
// Author:Steven Martin (smarti24@jaguarlandrover.com)

#include "dstc.h"
#include <stdio.h>
#include <stdlib.h>

DSTC_SERVER(basic_type_many_args, int,, char,, uint16_t,, int,);

int expected_arg1 = -124;
char expected_arg2 = 'q';
uint16_t expected_arg3 = 41;
int expected_arg4 = -1412;

void basic_type_many_args(int arg1, char arg2, uint16_t arg3, int arg4) {
    if (arg1 != expected_arg1 || arg2 != expected_arg2 || arg3 != expected_arg3 || arg4 != expected_arg4) {
        exit(1);
    }
    else {
        exit(0);
    }
}

int main (int argc, char* argv[]) {
    if (argc != 5) {
        printf("Did not get expected number of arguments to test server process\n");
        exit(3);
    }
    else {
        expected_arg1 = atoi(argv[1]);
        expected_arg2 = argv[2][0];
        expected_arg3 = atoi(argv[3]);
        expected_arg4 = atoi(argv[4]);
    }

    while(1) {
        dstc_process_events(-1);
    }
}