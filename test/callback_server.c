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

DSTC_SERVER(double_value, int,, DSTC_DECL_CALLBACK_ARG)
DSTC_SERVER(add_and_multiply, int,, int,, DSTC_DECL_CALLBACK_ARG)

void double_value(int value, dstc_callback_t callback_ref)
{
    DSTC_SERVER_CALLBACK(callback_ref, int,);
    dstc_callback_ref(value * 2);
    dstc_process_events(-1);
    exit(0);
}

void add_and_multiply(int a, int b, dstc_callback_t callback_ref)
{
    DSTC_SERVER_CALLBACK(callback_ref, int,,int,)
    dstc_callback_ref(a + b, a * b);
    dstc_process_events(-1);
    exit(0);
}

int main (int argc, char* argv[]) {
    while(1) {
        dstc_process_events(-1);
    }
}