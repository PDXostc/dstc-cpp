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
DSTC_SERVER(rude_contradiction, DSTC_DECL_DYNAMIC_ARG, DSTC_DECL_CALLBACK_ARG)
DSTC_SERVER(str_concat, DSTC_DECL_DYNAMIC_ARG, DSTC_DECL_DYNAMIC_ARG, DSTC_DECL_CALLBACK_ARG)
DSTC_SERVER(gen_fib, int, [2], DSTC_DECL_CALLBACK_ARG);
DSTC_SERVER(add_and_multiply_arrays, int, [10], int, [10], DSTC_DECL_CALLBACK_ARG);
DSTC_SERVER(echo, struct SimpleStruct, [3], DSTC_DECL_DYNAMIC_ARG, char,, DSTC_DECL_CALLBACK_ARG);

DSTC_SERVER_CALLBACK(callback_double_value, int,);

void double_value(int value, dstc_callback_t callback_ref)
{
    dstc_callback_double_value(callback_ref, value * 2);
    dstc_process_events(-1);
    exit(0);
}

DSTC_SERVER_CALLBACK(callback_add_and_multiply, int,,int,)

void add_and_multiply(int a, int b, dstc_callback_t callback_ref)
{
    dstc_callback_add_and_multiply(callback_ref, a + b, a * b);
    dstc_process_events(-1);
    exit(0);
}

DSTC_SERVER_CALLBACK(callback_add_and_multiply_arrays, int, [10], int, [10]);

void add_and_multiply_arrays(int a[10], int b[10], dstc_callback_t callback_ref)
{

    int sums[10];
    int products[10];

    for (unsigned int i = 0; i < 10; ++i) {
        sums[i] = a[i] + b[i];
        products[i] = a[i] * b[i];
    }

    dstc_callback_add_and_multiply_arrays(callback_ref, sums, products);
    dstc_process_events(-1);
    exit(0);
}

DSTC_SERVER_CALLBACK(callback_do_lots_of_things, struct ForManipulation,)

void do_lots_of_things(struct ForManipulation arg, dstc_callback_t callback_ref)
{

    struct ForManipulation ret;

    ret.ch = arg.ch + 1;
    ret.u16 = arg.u16 + 64;
    ret.i = arg.i * 2;
    ret.d = arg.d / 2.0;

    dstc_callback_do_lots_of_things(callback_ref, ret);

    dstc_process_events(-1);
    exit(0);
}

DSTC_SERVER_CALLBACK(callback_separate_types, struct Struct16,, struct Struct8,)

void separate_types(struct StructA a, struct StructB b, dstc_callback_t callback_ref)
{

    struct Struct8  s8;
    struct Struct16 s16;

    s8.b = a.b;
    s8.c = a.c;
    s8.e = b.e;

    s16.a = a.a;
    s16.d = b.d;
    s16.f = b.f;

    dstc_callback_separate_types(callback_ref, s16, s8);

    dstc_process_events(-1);
    exit(0);
}


DSTC_SERVER_CALLBACK(callback_rude_contradiction, DSTC_DECL_DYNAMIC_ARG)

void rude_contradiction(dstc_dynamic_data_t input, dstc_callback_t callback_ref)
{

    char start_expression[] = "No, ";
    char stop_expression[] = " is the stupidest thing I've ever heard.";

    char* final_expression = (char*) malloc(
        input.length + strlen(start_expression) + strlen(stop_expression)
    );

    strcpy(final_expression, start_expression);
    strcat(final_expression, (char*)input.data);
    strcat(final_expression, stop_expression);

    dstc_dynamic_data_t output;
    output.length = strlen(final_expression) + 1;
    output.data = (void*) final_expression;

    dstc_callback_rude_contradiction(callback_ref, output);

    dstc_process_events(-1);

    free(final_expression);

    exit(0);
}

DSTC_SERVER_CALLBACK(callback_str_concat, DSTC_DECL_DYNAMIC_ARG, DSTC_DECL_DYNAMIC_ARG, DSTC_DECL_DYNAMIC_ARG)

void str_concat(dstc_dynamic_data_t input1, dstc_dynamic_data_t input2, dstc_callback_t callback_ref) {

    char* combined_str = (char*) malloc(input1.length + input2.length - 1);
    strcpy(combined_str, (char*) input1.data);
    strcat(combined_str, (char*) input2.data);

    dstc_dynamic_data_t ret_data;
    ret_data.length = strlen(combined_str) + 1;
    ret_data.data = combined_str;

    // Just to pass more data, we actually pass the original strings back w/ the concat screen
    dstc_callback_str_concat(callback_ref, input1, input2, ret_data);
    dstc_process_events(-1);
    free(combined_str);
    exit(0);
}

DSTC_SERVER_CALLBACK(callback_gen_fib, int, [10]);

void gen_fib(int seeds[2], dstc_callback_t callback_ref) {

    int fib_values[10];
    fib_values[0] = seeds[0];
    fib_values[1] = seeds[1];

    for (unsigned int i = 2; i < 10; ++i) {
        fib_values[i] = fib_values[i-1] + fib_values[i-2];
    }

    dstc_callback_gen_fib(callback_ref, fib_values);
    dstc_process_events(-1);
    exit(0);
}


DSTC_SERVER_CALLBACK(callback_echo, struct SimpleStruct, [3], DSTC_DECL_DYNAMIC_ARG, char,);
void echo(struct SimpleStruct struct_array[3], dstc_dynamic_data_t char_array, char ch, dstc_callback_t callback_ref)
{
    dstc_callback_echo(callback_ref, struct_array, char_array, ch);
    dstc_process_events(-1);
    exit(0);
}

int main (int argc, char* argv[]) {
    while(1) {
        dstc_process_events(-1);
    }
}