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

DSTC_SERVER(mixed_types, uint8_t,, uint16_t, [7], struct SimpleStruct,, DSTC_DECL_DYNAMIC_ARG)

void validateArg1(uint8_t arg) {
    if (arg != 42) {
        exit(1);
    }
}

void validateArg2(uint16_t arg[7]) {
    for (int i = 0; i < 7; ++i) {
        if (arg[i] != i + 1000) {
            exit(1);
        }
    }
}

void validateArg3(struct SimpleStruct arg) {
    if (arg.a != 'c' || arg.b != 541) {
        exit(1);
    }
}

void validateArg4(dstc_dynamic_data_t arg) {
    char expected_str[] = "this is a string";
    if (arg.length != (strlen(expected_str) + 1)) {
        exit(1);
    }
    if (strcmp((char*)arg.data, expected_str)) {
        exit(1);
    }
}

void mixed_types(uint8_t arg1, uint16_t arg2[7], struct SimpleStruct arg3, dstc_dynamic_data_t arg4) {
    validateArg1(arg1);
    validateArg2(arg2);
    validateArg3(arg3);
    validateArg4(arg4);
    exit(0);
}

int main (int argc, char* argv[]) {
    while(1) {
        dstc_process_events(-1);
    }
}