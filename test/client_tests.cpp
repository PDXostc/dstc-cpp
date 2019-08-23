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
    }
    dstc::RemoteFunction<names::print_name_and_age, char[32], int> printNameAndAge;
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