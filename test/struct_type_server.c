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

DSTC_SERVER(struct_type, struct SimpleStruct,)

struct SimpleStruct expected_arg;

void struct_type(struct SimpleStruct arg) {
    if (arg.a != expected_arg.a || arg.b != expected_arg.b) {
        exit(1);
    }
    exit(0);
}

int main (int argc, char* argv[]) {

    if (argc != 3) {
        exit(3);
    }

    expected_arg.a = argv[1][0];
    expected_arg.b = atoi(argv[2]);

    while(1) {
        dstc_process_events(-1);
    }
}