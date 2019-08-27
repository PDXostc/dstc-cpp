// Copyright (C) 2019, Jaguar Land Rover
// This program is licensed under the terms and conditions of the
// Mozilla Public License, version 2.0.  The full text of the
// Mozilla Public License is at https://www.mozilla.org/MPL/2.0/
//
// Author:Steven Martin (smarti24@jaguarlandrover.com)

#include <gtest/gtest.h>
#include "dstc.hpp"
#include <future>
#include <cstdlib>

extern "C" {
    #include "test/teststructs.h"
}

namespace remote {
    namespace names {
        const char print_name_and_age[] = "print_name_and_age";
        const char basic_type_one_arg[] = "basic_type_one_arg";
        const char basic_type_many_args[] = "basic_type_many_args";
        const char array_type_one_arg[] = "array_type_one_arg";
        const char array_type_many_args[] = "array_type_many_args";
        const char dynamic_type[] = "dynamic_type";
        const char multiple_dynamic_type[] = "multiple_dynamic_type";
        const char struct_type[] = "struct_type";
        const char multiple_struct_type[] = "multiple_struct_type";
        const char array_of_struct[] = "array_of_struct";
        const char mixed_types[] = "mixed_types";
        const char double_value[] = "double_value";
        const char add_and_multiply[] = "add_and_multiply";
        const char do_lots_of_things[] = "do_lots_of_things";
        const char separate_types[] = "separate_types";
        const char rude_contradiction[] = "rude_contradiction";
    }
    dstc::RemoteFunction<names::print_name_and_age, char[32], int> printNameAndAge;
    dstc::RemoteFunction<names::basic_type_one_arg, int> basicTypeOneArg;
    dstc::RemoteFunction<names::basic_type_many_args, int, char, uint16_t, int> basicTypeManyArgs;
    dstc::RemoteFunction<names::array_type_one_arg, int[17]> arrayTypeOneArg;
    dstc::RemoteFunction<names::array_type_many_args, char[17], int[5]> arrayTypeManyArgs;
    dstc::RemoteFunction<names::dynamic_type, dstc_dynamic_data_t> dynamicType;
    dstc::RemoteFunction<names::multiple_dynamic_type, dstc_dynamic_data_t, dstc_dynamic_data_t> multipleDynamicType;
    dstc::RemoteFunction<names::struct_type, SimpleStruct> structType;
    dstc::RemoteFunction<names::multiple_struct_type, SimpleStruct, DifferentSimpleStruct> multipleStructType;
    dstc::RemoteFunction<names::array_of_struct, SimpleStruct[5]> arrayOfStruct;
    dstc::RemoteFunction<names::mixed_types, uint8_t, uint16_t[7], SimpleStruct, dstc_dynamic_data_t> mixedTypes;
    dstc::RemoteFunction<names::double_value, int, dstc::CallbackFunction<int>> doubleValue;
    dstc::RemoteFunction<names::add_and_multiply, int, int, dstc::CallbackFunction<int, int>> addAndMultiply;
    dstc::RemoteFunction<names::do_lots_of_things, ForManipulation, dstc::CallbackFunction<ForManipulation>> doLotsOfThings;
    dstc::RemoteFunction<names::separate_types, StructA, StructB, dstc::CallbackFunction<Struct16, Struct8>> separateTypes;
    dstc::RemoteFunction<names::rude_contradiction, dstc_dynamic_data_t, dstc::CallbackFunction<dstc_dynamic_data_t>> rudeContradiction;
}

std::future<int> spawnProcess(std::string&& server_binary_name) {
    return std::async(std::launch::async,
        [server_binary_name] {
            return WEXITSTATUS( std::system(server_binary_name.c_str()) );
        }
    );
}

TEST(ServerAvailable, no_server) {
    dstc::EventLoopRunner runner;

    auto test_start = std::chrono::system_clock::now();
    EXPECT_FALSE(remote::printNameAndAge.blockUntilServerAvailable(runner, 1000));
    EXPECT_FALSE(remote::printNameAndAge.serverAvailable());
    EXPECT_GE(std::chrono::system_clock::now(), test_start + std::chrono::milliseconds(1000));
}

TEST(ServerAvailable, server_already_started) {

    dstc::EventLoopRunner runner;

    auto server_retval_fut = spawnProcess("./print_name_and_age_server \"Boaty McBoatface\" 4");

    // Give server what should be enough time to actually start up
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    EXPECT_TRUE(remote::printNameAndAge.serverAvailable());

    auto start_time = std::chrono::system_clock::now();

    EXPECT_TRUE(remote::printNameAndAge.blockUntilServerAvailable(runner));

    EXPECT_LE(std::chrono::system_clock::now(), start_time + std::chrono::milliseconds(1000));

    char name[] = "Boaty McBoatface";
    remote::printNameAndAge(name, 4);

    EXPECT_EQ(0, server_retval_fut.get());
}

TEST(ServerAvailable, delayed_availability) {

    dstc::EventLoopRunner runner;

    EXPECT_FALSE(remote::printNameAndAge.serverAvailable());

    // launch server in 1000 ms
    auto server_launch_fut = std::async(
        std::launch::async,
        [] {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            return spawnProcess("./print_name_and_age_server \"Neil Peterson III\" 78 ");
        }
    );

    EXPECT_FALSE(remote::printNameAndAge.serverAvailable());

    auto start_time = std::chrono::system_clock::now();

    EXPECT_TRUE(remote::printNameAndAge.blockUntilServerAvailable(runner, 2000));

    auto stop_time = std::chrono::system_clock::now();

    auto elapsed_time = stop_time - start_time;

    EXPECT_GE(elapsed_time, std::chrono::milliseconds(900));
    EXPECT_LT(elapsed_time, std::chrono::milliseconds(2000));

    char name[] = "Neil Peterson III";
    remote::printNameAndAge(name, 78);

    EXPECT_EQ(0, server_launch_fut.get().get());
}

// DSTC does not appear to support this, but leaving this as a placeholder for a test gap
// in the future, as there is code in DSTC++ that can't be tested if this is not enabled
TEST(RemoteFunction, DISABLED_no_args) {
    FAIL() << "This is not supported by DSTC yet";
}

TEST(RemoteFunction, basic_type_one_arg) {
    dstc::EventLoopRunner runner;
    auto fut = spawnProcess("./basic_type_one_arg_server -1927");
    EXPECT_TRUE(remote::basicTypeOneArg.blockUntilServerAvailable(runner));
    remote::basicTypeOneArg(-1927);
    EXPECT_EQ(0, fut.get());
}

TEST(RemoteFunction, basic_type_neg_test) {
    dstc::EventLoopRunner runner;
    auto fut = spawnProcess("./basic_type_one_arg_server -1927");
    EXPECT_TRUE(remote::basicTypeOneArg.blockUntilServerAvailable(runner));
    remote::basicTypeOneArg(1927);
    EXPECT_EQ(1, fut.get());
}

TEST(RemoteFunction, basic_type_many_args) {

    dstc::EventLoopRunner runner;
    auto fut = spawnProcess("./basic_type_many_args_server -512 k 14 51982");
    EXPECT_TRUE(remote::basicTypeManyArgs.blockUntilServerAvailable(runner));
    remote::basicTypeManyArgs(-512, 'k', 14, 51982);
    EXPECT_EQ(0, fut.get());
}

TEST(RemoteFunction, basic_type_many_args_neg_test_1) {

    dstc::EventLoopRunner runner;
    auto fut = spawnProcess("./basic_type_many_args_server -512 k 14 51982");
    EXPECT_TRUE(remote::basicTypeManyArgs.blockUntilServerAvailable(runner));
    remote::basicTypeManyArgs(-511, 'k', 14, 51982);
    EXPECT_EQ(1, fut.get());
}

TEST(RemoteFunction, basic_type_many_args_neg_test_2) {
    dstc::EventLoopRunner runner;
    auto fut = spawnProcess("./basic_type_many_args_server -512 k 14 51982");
    EXPECT_TRUE(remote::basicTypeManyArgs.blockUntilServerAvailable(runner));
    remote::basicTypeManyArgs(-512, 'i', 14, 51982);
    EXPECT_EQ(1, fut.get());
}

TEST(RemoteFunction, basic_type_many_args_neg_test_3) {
    dstc::EventLoopRunner runner;
    auto fut = spawnProcess("./basic_type_many_args_server -512 k 14 51982");
    EXPECT_TRUE(remote::basicTypeManyArgs.blockUntilServerAvailable(runner));
    remote::basicTypeManyArgs(-512, 'k', 15, 51982);
    EXPECT_EQ(1, fut.get());
}

TEST(RemoteFunction, basic_type_many_args_neg_test_4) {
    dstc::EventLoopRunner runner;
    auto fut = spawnProcess("./basic_type_many_args_server -512 k 14 51982");
    EXPECT_TRUE(remote::basicTypeManyArgs.blockUntilServerAvailable(runner));
    remote::basicTypeManyArgs(-512, 'k', 14, 51983);
    EXPECT_EQ(1, fut.get());
}

TEST(RemoteFunction, array_type_one_arg) {
    dstc::EventLoopRunner runner;
    auto fut = spawnProcess("./array_type_one_arg_server 51");
    EXPECT_TRUE(remote::arrayTypeOneArg.blockUntilServerAvailable(runner));

    int array[17];
    for (int i = 0; i < 17; ++i) {
        array[i] = i + 51;
    }

    remote::arrayTypeOneArg(array);

    EXPECT_EQ(0, fut.get());
}

TEST(RemoteFunction, array_type_one_arg_neg) {
    dstc::EventLoopRunner runner;
    auto fut = spawnProcess("./array_type_one_arg_server 51");
    EXPECT_TRUE(remote::arrayTypeOneArg.blockUntilServerAvailable(runner));

    int array[17];
    for (int i = 0; i < 17; ++i) {
        array[i] = i + 51;
    }

    // spoil one element
    array[15] = 99999;

    remote::arrayTypeOneArg(array);

    EXPECT_EQ(1, fut.get());
}

TEST(RemoteFunction, array_type_many_args) {
    dstc::EventLoopRunner runner;
    auto fut = spawnProcess("./array_type_many_args_server bananas 817");
    EXPECT_TRUE(remote::arrayTypeManyArgs.blockUntilServerAvailable(runner));

    int array[5];
    for (auto i = 0; i < 5; ++i) {
        array[i] = 817 + i;
    }

    char ch_array[] = "bananas";

    remote::arrayTypeManyArgs(ch_array, array);

    EXPECT_EQ(0, fut.get());;
}

TEST(RemoteFunction, array_type_many_args_neg1) {
    dstc::EventLoopRunner runner;
    auto fut = spawnProcess("./array_type_many_args_server bananas 817");
    EXPECT_TRUE(remote::arrayTypeManyArgs.blockUntilServerAvailable(runner));

    int array[5];
    for (auto i = 0; i < 5; ++i) {
        array[i] = 817 + i;
    }

    // too many letters
    char ch_array[] = "bananasas";

    remote::arrayTypeManyArgs(ch_array, array);

    EXPECT_EQ(1, fut.get());;
}

TEST(RemoteFunction, array_type_many_args_neg2) {
    dstc::EventLoopRunner runner;
    auto fut = spawnProcess("./array_type_many_args_server bananas 817");
    EXPECT_TRUE(remote::arrayTypeManyArgs.blockUntilServerAvailable(runner));

    int array[5];
    for (auto i = 0; i < 5; ++i) {
        array[i] = 817 + i;
    }

    // bad data
    array[2] = 135352;

    char ch_array[] = "bananas";

    remote::arrayTypeManyArgs(ch_array, array);

    EXPECT_EQ(1, fut.get());;
}

TEST(RemoteFunction, dynamic_type) {
    dstc::EventLoopRunner runner;
    auto fut = spawnProcess("./dynamic_type_server a 1341 e 325 g 128976");
    EXPECT_TRUE(remote::dynamicType.blockUntilServerAvailable(runner));

    SimpleStruct data[3];
    data[0].a = 'a';
    data[0].b = 1341;
    data[1].a = 'e';
    data[1].b = 325;
    data[2].a = 'g';
    data[2].b = 128976;

    dstc_dynamic_data_t dyn_data;
    dyn_data.length = sizeof(SimpleStruct) * 3;
    dyn_data.data = (void*) data;

    remote::dynamicType(dyn_data);

    EXPECT_EQ(0, fut.get());
}

TEST(RemoteFunction, dynamic_type_neg) {
    dstc::EventLoopRunner runner;
    auto fut = spawnProcess("./dynamic_type_server a 1341 e 325 g 128976");
    EXPECT_TRUE(remote::dynamicType.blockUntilServerAvailable(runner));

    SimpleStruct data[3];
    data[0].a = 'a';
    data[0].b = 1341;
    data[1].a = 'e';
    data[1].b = 326; // incorrect data
    data[2].a = 'g';
    data[2].b = 128976;

    dstc_dynamic_data_t dyn_data;
    dyn_data.length = sizeof(SimpleStruct) * 3;
    dyn_data.data = (void*) data;

    remote::dynamicType(dyn_data);

    EXPECT_EQ(1, fut.get());
}

TEST(RemoteFunction, multiple_dynamic_type) {
    dstc::EventLoopRunner runner;
    auto fut = spawnProcess("./multiple_dynamic_type_server");
    EXPECT_TRUE(remote::multipleDynamicType.blockUntilServerAvailable(runner));

    std::cout << "Function manifested itself..." << std::endl;

    SimpleStruct expected_arg1[3];
    expected_arg1[0].a = 'a';
    expected_arg1[1].a = 'b';
    expected_arg1[2].a = 'c';

    expected_arg1[0].b = 123;
    expected_arg1[1].b = 456;
    expected_arg1[2].b = 789;

    DifferentSimpleStruct expected_arg2[2];
    expected_arg2[0].a = 234;
    expected_arg2[1].a = 567;

    expected_arg2[0].b = 'x';
    expected_arg2[1].b = 'y';

    expected_arg2[0].c = 1234;
    expected_arg2[1].c = 9876;

    dstc_dynamic_data_t dyn_data1, dyn_data2;

    dyn_data1.length = sizeof(SimpleStruct) * 3;
    dyn_data2.length = sizeof(DifferentSimpleStruct) * 2;

    dyn_data1.data = (void*)expected_arg1;
    dyn_data2.data = (void*)expected_arg2;

    remote::multipleDynamicType(dyn_data1, dyn_data2);

    EXPECT_EQ(0, fut.get());
}


TEST(RemoteFunction, multiple_dynamic_type_neg1) {
    dstc::EventLoopRunner runner;
    auto fut = spawnProcess("./multiple_dynamic_type_server");
    EXPECT_TRUE(remote::multipleDynamicType.blockUntilServerAvailable(runner));

    SimpleStruct expected_arg1[3];
    expected_arg1[0].a = 'a';
    expected_arg1[1].a = 'b';
    expected_arg1[2].a = 'c';

    expected_arg1[0].b = 1234; // wrong data
    expected_arg1[1].b = 456;
    expected_arg1[2].b = 789;

    DifferentSimpleStruct expected_arg2[2];
    expected_arg2[0].a = 234;
    expected_arg2[1].a = 567;

    expected_arg2[0].b = 'x';
    expected_arg2[1].b = 'y';

    expected_arg2[0].c = 1234;
    expected_arg2[1].c = 9876;

    dstc_dynamic_data_t dyn_data1, dyn_data2;

    dyn_data1.length = sizeof(SimpleStruct) * 3;
    dyn_data2.length = sizeof(DifferentSimpleStruct) * 2;

    dyn_data1.data = (void*)expected_arg1;
    dyn_data2.data = (void*)expected_arg2;

    remote::multipleDynamicType(dyn_data1, dyn_data2);

    EXPECT_EQ(1, fut.get());
}


TEST(RemoteFunction, multiple_dynamic_type_neg2) {
    dstc::EventLoopRunner runner;
    auto fut = spawnProcess("./multiple_dynamic_type_server");
    EXPECT_TRUE(remote::multipleDynamicType.blockUntilServerAvailable(runner));

    SimpleStruct expected_arg1[3];
    expected_arg1[0].a = 'a';
    expected_arg1[1].a = 'b';
    expected_arg1[2].a = 'c';

    expected_arg1[0].b = 123;
    expected_arg1[1].b = 456;
    expected_arg1[2].b = 789;

    DifferentSimpleStruct expected_arg2[2];
    expected_arg2[0].a = 234;
    expected_arg2[1].a = 567;

    expected_arg2[0].b = 'z'; // wrong data
    expected_arg2[1].b = 'y';

    expected_arg2[0].c = 1234;
    expected_arg2[1].c = 9876;

    dstc_dynamic_data_t dyn_data1, dyn_data2;

    dyn_data1.length = sizeof(SimpleStruct) * 3;
    dyn_data2.length = sizeof(DifferentSimpleStruct) * 2;

    dyn_data1.data = (void*)expected_arg1;
    dyn_data2.data = (void*)expected_arg2;

    remote::multipleDynamicType(dyn_data1, dyn_data2);

    EXPECT_EQ(1, fut.get());
}


TEST(RemoteFunction, struct_type) {
    dstc::EventLoopRunner runner;
    auto fut = spawnProcess("./struct_type_server H 7228");
    EXPECT_TRUE(remote::structType.blockUntilServerAvailable(runner));

    SimpleStruct data;
    data.a = 'H';
    data.b = 7228;

    remote::structType(data);

    EXPECT_EQ(0, fut.get());
}

TEST(RemoteFunction, struct_type_neg) {
    dstc::EventLoopRunner runner;
    auto fut = spawnProcess("./struct_type_server H 7228");
    EXPECT_TRUE(remote::structType.blockUntilServerAvailable(runner));

    SimpleStruct data;
    data.a = 'h'; // wrong
    data.b = 7228;

    remote::structType(data);

    EXPECT_EQ(1, fut.get());
}

TEST(RemoteFunction, multiple_struct_type) {
    dstc::EventLoopRunner runner;
    auto fut = spawnProcess("./multiple_struct_type_server m 192 1245 R 102");
    EXPECT_TRUE(remote::multipleStructType.blockUntilServerAvailable(runner));

    SimpleStruct arg1;
    arg1.a = 'm';
    arg1.b = 192;

    DifferentSimpleStruct arg2;
    arg2.a = 1245;
    arg2.b = 'R';
    arg2.c = 102;

    remote::multipleStructType(arg1, arg2);

    EXPECT_EQ(0, fut.get());
}

TEST(RemoteFunction, multiple_struct_type_neg1) {
    dstc::EventLoopRunner runner;
    auto fut = spawnProcess("./multiple_struct_type_server m 192 1245 R 102");
    EXPECT_TRUE(remote::multipleStructType.blockUntilServerAvailable(runner));

    SimpleStruct arg1;
    arg1.a = 'm';
    arg1.b = 19512; // UNACCEPTABLE!

    DifferentSimpleStruct arg2;
    arg2.a = 1245;
    arg2.b = 'R';
    arg2.c = 102;

    remote::multipleStructType(arg1, arg2);

    EXPECT_EQ(1, fut.get());
}

TEST(RemoteFunction, multiple_struct_type_neg2) {
    dstc::EventLoopRunner runner;
    auto fut = spawnProcess("./multiple_struct_type_server m 192 1245 R 102");
    EXPECT_TRUE(remote::multipleStructType.blockUntilServerAvailable(runner));

    SimpleStruct arg1;
    arg1.a = 'm';
    arg1.b = 192;

    DifferentSimpleStruct arg2;
    arg2.a = 1245;
    arg2.b = 'r'; // WRONG!
    arg2.c = 102;

    remote::multipleStructType(arg1, arg2);

    EXPECT_EQ(1, fut.get());
}


TEST(RemoteFunction, array_of_struct) {
    dstc::EventLoopRunner runner;
    auto fut = spawnProcess("./array_of_struct_server a 1 b 2 c 3 d 4 e 5");
    EXPECT_TRUE(remote::arrayOfStruct.blockUntilServerAvailable(runner));

    SimpleStruct array[5];
    for (auto i = 0; i < 5; ++i) {
        array[i].a = 'a' + i;
        array[i].b = i + 1;
    }

    remote::arrayOfStruct(array);

    EXPECT_EQ(0, fut.get());
}

TEST(RemoteFunction, array_of_struct_neg) {
    dstc::EventLoopRunner runner;
    auto fut = spawnProcess("./array_of_struct_server a 1 b 2 c 3 d 4 e 5");
    EXPECT_TRUE(remote::arrayOfStruct.blockUntilServerAvailable(runner));

    SimpleStruct array[5];
    for (auto i = 0; i < 5; ++i) {
        array[i].a = 'a' + i;
        array[i].b = i + 1;
    }

    array[3].b = 'x'; // wrong!!!!!

    remote::arrayOfStruct(array);

    EXPECT_EQ(1, fut.get());
}

TEST(RemoteFunction, mixed_types) {
    dstc::EventLoopRunner runner;
    auto fut = spawnProcess("./mixed_types_server");
    EXPECT_TRUE(remote::mixedTypes.blockUntilServerAvailable(runner));

    uint16_t array[7];
    for (uint16_t i = 0; i < 7; ++i) {
        array[i] = 1000 + i;
    }

    SimpleStruct strct;
    strct.a = 'c';
    strct.b = 541;

    char send_string[] = "this is a string";
    dstc_dynamic_data_t dyn_data;
    dyn_data.data = (void*)send_string;
    dyn_data.length = strlen(send_string) + 1;

    remote::mixedTypes(42, array, strct, dyn_data);

    EXPECT_EQ(0, fut.get());
}

TEST(RemoteFunction, mixed_types_neg1) {
    dstc::EventLoopRunner runner;
    auto fut = spawnProcess("./mixed_types_server");
    EXPECT_TRUE(remote::mixedTypes.blockUntilServerAvailable(runner));

    uint16_t array[7];
    for (uint16_t i = 0; i < 7; ++i) {
        array[i] = 1000 + i;
    }

    SimpleStruct strct;
    strct.a = 'c';
    strct.b = 541;

    char send_string[] = "this is a string";
    dstc_dynamic_data_t dyn_data;
    dyn_data.data = (void*)send_string;
    dyn_data.length = strlen(send_string) + 1;

    // incorrect int
    remote::mixedTypes(23, array, strct, dyn_data);

    EXPECT_EQ(1, fut.get());
}

TEST(RemoteFunction, mixed_types_neg2) {
    dstc::EventLoopRunner runner;
    auto fut = spawnProcess("./mixed_types_server");
    EXPECT_TRUE(remote::mixedTypes.blockUntilServerAvailable(runner));

    uint16_t array[7];
    for (uint16_t i = 0; i < 7; ++i) {
        array[i] = 1100 + i; // wrong!!!!
    }

    SimpleStruct strct;
    strct.a = 'c';
    strct.b = 541;

    char send_string[] = "this is a string";
    dstc_dynamic_data_t dyn_data;
    dyn_data.data = (void*)send_string;
    dyn_data.length = strlen(send_string) + 1;

    remote::mixedTypes(42, array, strct, dyn_data);

    EXPECT_EQ(1, fut.get());
}

TEST(RemoteFunction, mixed_types_neg3) {
    dstc::EventLoopRunner runner;
    auto fut = spawnProcess("./mixed_types_server");
    EXPECT_TRUE(remote::mixedTypes.blockUntilServerAvailable(runner));

    uint16_t array[7];
    for (uint16_t i = 0; i < 7; ++i) {
        array[i] = 1000 + i;
    }

    SimpleStruct strct;
    strct.a = 'a';  // WRONG!!!!!!
    strct.b = 541;

    char send_string[] = "this is a string";
    dstc_dynamic_data_t dyn_data;
    dyn_data.data = (void*)send_string;
    dyn_data.length = strlen(send_string) + 1;

    remote::mixedTypes(42, array, strct, dyn_data);

    EXPECT_EQ(1, fut.get());
}

TEST(RemoteFunction, mixed_types_neg4) {
    dstc::EventLoopRunner runner;
    auto fut = spawnProcess("./mixed_types_server");
    EXPECT_TRUE(remote::mixedTypes.blockUntilServerAvailable(runner));

    uint16_t array[7];
    for (uint16_t i = 0; i < 7; ++i) {
        array[i] = 1000 + i;
    }

    SimpleStruct strct;
    strct.a = 'c';
    strct.b = 541;

    char send_string[] = "this is NOT a string"; // oh so wrong!!!
    dstc_dynamic_data_t dyn_data;
    dyn_data.data = (void*)send_string;
    dyn_data.length = strlen(send_string) + 1;

    remote::mixedTypes(42, array, strct, dyn_data);

    EXPECT_EQ(1, fut.get());
}

// Not yet supported by DSTC
TEST(RemoteFunction, DISABLED_callback_no_arg) {
    FAIL() << "This is not supported by DSTC yet";
}

TEST(RemoteFunction, callback_basic_type) {
    dstc::EventLoopRunner runner;
    auto fut = spawnProcess("./callback_server");
    EXPECT_TRUE(remote::doubleValue.blockUntilServerAvailable(runner));

    std::atomic<int> value = 0;

    remote::doubleValue(1234, dstc::CallbackFunction<int>(
        [&value] (int arg){
            value = arg;
        })
    );

    while (value == 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    EXPECT_EQ(2468, value);

    EXPECT_EQ(0, fut.get());
}

TEST(RemoteFunction, callback_multiple_basic_type) {
    dstc::EventLoopRunner runner;
    auto fut = spawnProcess("./callback_server");
    EXPECT_TRUE(remote::addAndMultiply.blockUntilServerAvailable(runner));

    std::atomic<bool> done(false);

    remote::addAndMultiply(4, 6,
        dstc::CallbackFunction<int, int>(
            [&done] (int sum, int product) {
                EXPECT_EQ(4 + 6, sum);
                EXPECT_EQ(4 * 6, product);
                done = true;
            }
        )
    );

    while (!done) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    EXPECT_EQ(0, fut.get());
}

TEST(RemoteFunction, callback_struct_type) {
    dstc::EventLoopRunner runner;
    auto fut = spawnProcess("./callback_server");
    EXPECT_TRUE(remote::doLotsOfThings.blockUntilServerAvailable(runner));

    std::atomic<bool> done(false);

    ForManipulation data;
    data.ch = 'd';
    data.d = 142.0;
    data.i = 1512;
    data.u16 = 23;

    remote::doLotsOfThings(
        data, dstc::CallbackFunction<ForManipulation>(
            [&done](ForManipulation ret) {
                EXPECT_EQ('d' + 1, ret.ch);
                EXPECT_EQ(142.0 / 2.0, ret.d);
                EXPECT_EQ(1512 * 2, ret.i);
                EXPECT_EQ(23 + 64, ret.u16);
                done = true;
            }
        )
    );

    while (!done) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    EXPECT_EQ(0, fut.get());
}

TEST(RemoteFunction, callback_multiple_struct_type) {
    dstc::EventLoopRunner runner;
    auto fut = spawnProcess("./callback_server");
    EXPECT_TRUE(remote::separateTypes.blockUntilServerAvailable(runner));

    std::atomic<bool> done(false);

    StructA args_a;
    StructB args_b;

    args_a.a = 1;
    args_a.b = 2;
    args_a.c = 3;
    args_b.d = 4;
    args_b.e = 5;
    args_b.f = 6;

    remote::separateTypes(
        args_a, args_b,
        dstc::CallbackFunction<Struct16, Struct8> (
            [&done, &args_a, &args_b] (Struct16 struct16, Struct8 struct8) {
                EXPECT_EQ(args_a.a, struct16.a);
                EXPECT_EQ(args_a.b, struct8.b);
                EXPECT_EQ(args_a.c, struct8.c);
                EXPECT_EQ(args_b.d, struct16.d);
                EXPECT_EQ(args_b.e, struct8.e);
                EXPECT_EQ(args_b.f, struct16.f);
                done = true;
            }
        )
    );

    while (!done) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    EXPECT_EQ(0, fut.get());
}

TEST(RemoteFunction, callback_dynamic_type) {
    dstc::EventLoopRunner runner;
    auto fut = spawnProcess("./callback_server");
    EXPECT_TRUE(remote::rudeContradiction.blockUntilServerAvailable(runner));

    std::atomic<bool> done = false;

    char expression[] = "torqouise is a fun color";
    dstc_dynamic_data_t data;
    data.length = strlen(expression) + 1;
    data.data = (void*) expression;

    std::string final_expression = std::string("No, ") + expression + " is the stupidest thing I've ever heard.";

    remote::rudeContradiction(
        data,
        dstc::CallbackFunction<dstc_dynamic_data_t>(
            [&done, &final_expression] (dstc_dynamic_data_t reply) {
                EXPECT_EQ(final_expression.size() + 1, reply.length);
                EXPECT_STREQ(final_expression.c_str(), (char*) reply.data);
                done = true;
            }
        )
    );

    while (!done) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    EXPECT_EQ(0, fut.get());
}

TEST(RemoteFunction, callback_multiple_dynamic_type) {
    FAIL() << "Implement me";
}

TEST(RemoteFunction, callback_array_type) {
    FAIL() << "Implement me";
}

TEST(RemoteFunction, callback_multiple_array_type) {
    FAIL() << "Implement me";
}

TEST(RemoteFunction, callback_mixed_types) {
    FAIL() << "Implement me";
}

TEST(RemoteFunction, multiple_functions) {
    FAIL() << "Implement me";
}