// Copyright (C) 2019, Jaguar Land Rover
// This program is licensed under the terms and conditions of the
// Mozilla Public License, version 2.0.  The full text of the
// Mozilla Public License is at https://www.mozilla.org/MPL/2.0/
//
// Author:Steven Martin (smarti24@jaguarlandrover.com)

#include "dstceventloop.hpp"

#include <utility>

#include <iostream>

extern "C" {
    #include "dstc.h"
}

namespace dstc {

    std::mutex EventLoopRunner::_num_objects_mtx;
    unsigned int EventLoopRunner::_num_objects = 0;
    std::thread EventLoopRunner::_thread;
    bool EventLoopRunner::_thread_running = false;

    EventLoopRunner::EventLoopRunner()
    {
        _start();
    }

    EventLoopRunner::~EventLoopRunner() {
        std::lock_guard<std::mutex> lock(_num_objects_mtx);
        --_num_objects;
        if (_num_objects == 0) {
            _thread_running = false;

            // catch corner case that thread has not quite yet started...
            while(!_thread.joinable()) {}

            _thread.join();
        }
    }

    void EventLoopRunner::_start() {
        bool start_thread = false;
        { // lock scope
            std::lock_guard<std::mutex> lock(_num_objects_mtx);
            if (_num_objects == 0) {
                start_thread = true;
            }
            ++_num_objects;
        } // end lock scope

        if (start_thread) {
            _thread_running = true;
            _thread = std::thread(
                [] {
                    while(_thread_running) {
                        // if `dstc_process_events` reports a timeout, we yield the
                        // thread for 10 ms which should give other threads that may be
                        // blocked a chance to call into the API
                        if (dstc_process_events(10) == ETIME) {
                            std::this_thread::sleep_for(std::chrono::milliseconds(10));
                        }
                    }
                }
            );
        }
    }

}