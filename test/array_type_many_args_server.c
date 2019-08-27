// Copyright (C) 2019, Jaguar Land Rover
// This program is licensed under the terms and conditions of the
// Mozilla Public License, version 2.0.  The full text of the
// Mozilla Public License is at https://www.mozilla.org/MPL/2.0/
//
// Author:Steven Martin (smarti24@jaguarlandrover.com)

#include "dstc.h"
#include <stdio.h>
#include <stdlib.h>

#define CH_ARR_SIZE     17
#define INT_ARR_SIZE    5

DSTC_SERVER(array_type_many_args, char, [ CH_ARR_SIZE ], int, [ INT_ARR_SIZE ]);

int expected_value_start = -2351;
char expected_ch_array[CH_ARR_SIZE];

void array_type_many_args(char arg1[CH_ARR_SIZE], int arg2[ INT_ARR_SIZE ]) {
    if (strcmp(expected_ch_array, arg1)) {
        printf("Strings did not match.  Expected %s, but got %s\n", expected_ch_array, arg1);
        exit(1);
    }
    for (int i = 0; i < INT_ARR_SIZE; ++i) {
        if (arg2[i] != expected_value_start + i) {
            printf("Int array value did not match\n");
            exit(1);
        }
    }
    exit(0);
}

int main (int argc, char* argv[]) {
    if (argc != 3) {
        printf("Did not get expected number of arguments to test server process\n");
        exit(3);
    }
    else {
        if (strlen(argv[1]) < CH_ARR_SIZE + 1) {
            strcpy(expected_ch_array, argv[1]);
        }
        else {
            printf("String length too long");
            exit(3);
        }

        expected_value_start = atoi(argv[2]);
    }

    while(1) {
        dstc_process_events(-1);
    }
}