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

DSTC_SERVER(multiple_struct_type, struct SimpleStruct,, struct DifferentSimpleStruct,)

struct SimpleStruct expected_arg1;
struct DifferentSimpleStruct expected_arg2;

void multiple_struct_type(struct SimpleStruct arg1, struct DifferentSimpleStruct arg2) {
    if (arg1.a != expected_arg1.a ||
        arg1.b != expected_arg1.b )
    {
        exit(1);
    }
    if (arg2.a != expected_arg2.a ||
        arg2.b != expected_arg2.b ||
        arg2.c != expected_arg2.c )
    {
        exit(1);
    }

    exit(0);
}

int main (int argc, char* argv[]) {

    if (argc != 6) {
        exit(3);
    }

    expected_arg1.a = argv[1][0];
    expected_arg1.b = atoi(argv[2]);
    expected_arg2.a = atoi(argv[3]);
    expected_arg2.b = argv[4][0];
    expected_arg2.c = atoi(argv[5]);

    while(1) {
        dstc_process_events(-1);
    }
}
