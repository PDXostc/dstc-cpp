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

unsigned int expected_num_structs = -1;
struct SimpleStruct* expected_values;

DSTC_SERVER(dynamic_type, DSTC_DECL_DYNAMIC_ARG)

void dynamic_type(dstc_dynamic_data_t arg) {
    unsigned int num_structs = arg.length / sizeof(struct SimpleStruct);
    // should be no extra bytes...
    if (arg.length % sizeof(struct SimpleStruct)) {
        printf("arg.length is not a multiple size of SimpleStruct (arg.length=%i)\n", arg.length);
        exit (2);
    }

    if (num_structs != expected_num_structs) {
        printf("Did not get the number of structs expected, got %i expected %i\n", num_structs, expected_num_structs);
        exit(2);
    }

    struct SimpleStruct* received_values = (struct SimpleStruct*) arg.data;
    for (int i = 0; i < expected_num_structs; ++i) {
        if (received_values[i].a != expected_values[i].a ||
            received_values[i].b != expected_values[i].b)
        {
            printf("Data mismatch at index %i expect: %c/%i got: %c/%i", i, expected_values[i].a, expected_values[i].b, received_values[i].a, received_values[i].b);
            exit(1);
        }
    }
    exit(0);
}

int main (int argc, char* argv[]) {
    if ((argc % 2) != 1) {
        printf("Need an even number of arguments to binary");
        exit(3);
    }
    else {
        unsigned int num_entries = ((argc - 1) / 2);
        if (num_entries > 0) {
            expected_num_structs = num_entries;
            expected_values = malloc(sizeof(struct SimpleStruct) * 2);
            for (unsigned int i = 0; i < num_entries; ++i) {
                expected_values[i].a = argv[1 + i * 2][0];
                expected_values[i].b = atoi(argv[2 + i * 2]);
            }
        }
        else {
            exit(3);
        }
    }

    while(1) {
        dstc_process_events(-1);
    }
}