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

DSTC_SERVER(multiple_dynamic_type, DSTC_DECL_DYNAMIC_ARG, DSTC_DECL_DYNAMIC_ARG)

#define EXPECTED_DYN_ENTRIES_1  3
#define EXPECTED_DYN_ENTRIES_2  2

struct SimpleStruct expected_arg1[ EXPECTED_DYN_ENTRIES_1 ];
struct DifferentSimpleStruct expected_arg2[ EXPECTED_DYN_ENTRIES_2 ];

void populateExpectedData() {
    expected_arg1[0].a = 'a';
    expected_arg1[1].a = 'b';
    expected_arg1[2].a = 'c';

    expected_arg1[0].b = 123;
    expected_arg1[1].b = 456;
    expected_arg1[2].b = 789;

    expected_arg2[0].a = 234;
    expected_arg2[1].a = 567;

    expected_arg2[0].b = 'x';
    expected_arg2[1].b = 'y';

    expected_arg2[0].c = 1234;
    expected_arg2[1].c = 9876;
}


void multiple_dynamic_type(dstc_dynamic_data_t arg1, dstc_dynamic_data_t arg2) {

    unsigned int num_structs1 = arg1.length / sizeof(struct SimpleStruct);
    // should be no extra bytes...
    if (arg1.length % sizeof(struct SimpleStruct)) {
        printf("arg1.length is not a multiple size of SimpleStruct (arg.length=%i)\n", arg1.length);
        exit (2);
    }

    if (num_structs1 != EXPECTED_DYN_ENTRIES_1) {
        printf("Did not get the number of structs expected, got %i expected %i\n", num_structs1, EXPECTED_DYN_ENTRIES_1);
        exit(2);
    }

    unsigned int num_structs2 = arg2.length / sizeof(struct DifferentSimpleStruct);

    if (arg2.length %sizeof(struct DifferentSimpleStruct)) {
        printf("arg2.length is not a multiple size of DifferentSimpleStruct (arg2.length=%i)\n", arg2.length);
        exit (2);
    }

    if (num_structs2 != EXPECTED_DYN_ENTRIES_2) {
        printf("Did not get the number of structs expected, got %i expected %i\n", num_structs2, EXPECTED_DYN_ENTRIES_2);
        exit(2);
    }

    struct SimpleStruct* received_values1 = (struct SimpleStruct*) arg1.data;
    struct DifferentSimpleStruct* received_values2 = (struct DifferentSimpleStruct*) arg2.data;

    for (unsigned int i = 0; i < EXPECTED_DYN_ENTRIES_1; ++i) {
        if (received_values1[i].a != expected_arg1[i].a
         || received_values1[i].b != expected_arg1[i].b)
        {
            exit(1);
        }
    }

    for (unsigned int i = 0; i < EXPECTED_DYN_ENTRIES_2; ++i) {
        if (received_values2[i].a != expected_arg2[i].a
         || received_values2[i].b != expected_arg2[i].b
         || received_values2[i].c != expected_arg2[i].c)
        {
            exit(1);
        }
    }

    exit(0);
}

int main (int argc, char* argv[]) {
    populateExpectedData();

    while(1) {
        dstc_process_events(-1);
    }
}