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

namespace remote {
    namespace names {
        const char print_name_and_age[] = "print_name_and_age";
        const char basic_type_one_arg[] = "basic_type_one_arg";
        const char basic_type_many_args[] = "basic_type_many_args";
        const char array_type_one_arg[] = "array_type_one_arg";
    }
    dstc::RemoteFunction<names::print_name_and_age, char[32], int> printNameAndAge;
    dstc::RemoteFunction<names::basic_type_one_arg, int> basicTypeOneArg;
    dstc::RemoteFunction<names::basic_type_many_args, int, char, uint16_t, int> basicTypeManyArgs;
    dstc::RemoteFunction<names::array_type_one_arg, int[17]> arrayTypeOneArg;
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

TEST(RemoteFunction, array_type_many_arg) {
    FAIL() << "Implement me";
}

TEST(RemoteFunction, dynamic_type) {
    FAIL() << "Implement me";
}

TEST(RemoteFunction, multiple_dynamic_type) {
    FAIL() << "Implement me";
}

TEST(RemoteFunction, struct_type) {
    FAIL() << "Implement me";
}

TEST(RemoteFunction, multiple_struct_type) {
    FAIL() << "Implement me";
}


TEST(RemoteFunction, array_of_struct) {
    FAIL() << "Implement me";
}


TEST(RemoteFunction, mixed_types) {
    FAIL() << "Implement me";
}

// Not yet supported by DSTC
TEST(RemoteFunction, DISABLED_callback_no_arg) {
    FAIL() << "This is not supported by DSTC yet";
}

TEST(RemoteFunction, callback_basic_type) {
    FAIL() << "Implement me";
}

TEST(RemoteFunction, callback_multiple_basic_type) {
    FAIL() << "Implement me";
}

TEST(RemoteFunction, callback_struct_type) {
    FAIL() << "Implement me";
}

TEST(RemoteFunction, callback_multiple_struct_type) {
    FAIL() << "Implement me";
}

TEST(RemoteFunction, callback_dynamic_type) {
    FAIL() << "Implement me";
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