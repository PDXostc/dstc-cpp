// Copyright (C) 2019, Jaguar Land Rover
// This program is licensed under the terms and conditions of the
// Mozilla Public License, version 2.0.  The full text of the
// Mozilla Public License is at https://www.mozilla.org/MPL/2.0/
//
// Author:Steven Martin (smarti24@jaguarlandrover.com)

#include "dstc.h"
#include <stdio.h>
#include <stdlib.h>

#include "teststructs.h"

#define ARRAY_SIZE 5

DSTC_SERVER(array_of_struct, struct SimpleStruct, [ARRAY_SIZE] )

struct SimpleStruct expected_data[ARRAY_SIZE];

void array_of_struct(struct SimpleStruct arg[ARRAY_SIZE]) {
    for (unsigned int i = 0; i < ARRAY_SIZE; ++i) {
        if (arg[i].a != expected_data[i].a ||
            arg[i].b != expected_data[i].b)
        {
            exit(1);
        }
    }
    exit(0);
}

int main (int argc, char* argv[]) {

    if (argc != 1 + ARRAY_SIZE * 2) {
        exit(3);
    }
    for (unsigned int i = 0; i < ARRAY_SIZE; ++i) {
        expected_data[i].a = argv[1+i*2][0];
        expected_data[i].b = atoi(argv[2+i*2]);
    }
    while(1) {
        dstc_process_events(-1);
    }
}