// Copyright (C) 2019, Jaguar Land Rover
// This program is licensed under the terms and conditions of the
// Mozilla Public License, version 2.0.  The full text of the
// Mozilla Public License is at https://www.mozilla.org/MPL/2.0/
//
// Author:Steven Martin (smarti24@jaguarlandrover.com)

#include "dstc.h"
#include <stdio.h>
#include <stdlib.h>

DSTC_SERVER(print_name_and_age, char, [32], int,)

int ret_val = 63;

int expected_age = -16512;
char* expected_name;


void print_name_and_age(char name[32], int age)
{
    if (expected_age != age) {
        printf("Did not get expected age in server\n");
        exit(1);
    }
    if (strcmp(expected_name, name)) {
        printf("Did not get expected name in server\n");
        exit(2);
    }

    // All's well
    exit(0);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Did not get expected number of arguments to test server process\n");
        exit(3);
    }
    else {
        expected_name = argv[1];
        expected_age = atoi(argv[2]);
    }

    while(1) {
        dstc_process_events(-1);
    }
}