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
DSTC_SERVER(do_lots_of_things, struct ForManipulation,, DSTC_DECL_CALLBACK_ARG)
DSTC_SERVER(separate_types, struct StructA,, struct StructB,, DSTC_DECL_CALLBACK_ARG)

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

void do_lots_of_things(struct ForManipulation arg, dstc_callback_t callback_ref)
{
    DSTC_SERVER_CALLBACK(callback_ref, struct ForManipulation,)

    struct ForManipulation ret;

    ret.ch = arg.ch + 1;
    ret.u16 = arg.u16 + 64;
    ret.i = arg.i * 2;
    ret.d = arg.d / 2.0;

    dstc_callback_ref(ret);

    dstc_process_events(-1);
    exit(0);
}

void separate_types(struct StructA a, struct StructB b, dstc_callback_t callback_ref)
{
    DSTC_SERVER_CALLBACK(callback_ref, struct Struct16,, struct Struct8,)

    struct Struct8  s8;
    struct Struct16 s16;

    s8.b = a.b;
    s8.c = a.c;
    s8.e = b.e;

    s16.a = a.a;
    s16.d = b.d;
    s16.f = b.f;

    dstc_callback_ref(s16, s8);

    dstc_process_events(-1);
    exit(0);
}

int main (int argc, char* argv[]) {
    while(1) {
        dstc_process_events(-1);
    }
}