// Copyright (C) 2019, Jaguar Land Rover
// This program is licensed under the terms and conditions of the
// Mozilla Public License, version 2.0.  The full text of the
// Mozilla Public License is at https://www.mozilla.org/MPL/2.0/
//
// Author:Steven Martin (smarti24@jaguarlandrover.com)

#include <mutex>
#include <thread>

namespace dstc {

    class EventLoopRunner {
    public:

        EventLoopRunner();
        ~EventLoopRunner();
    private:
        void _start();

        void _spawnThread();
        void _stopThread();

        static std::mutex _num_objects_mtx;
        static unsigned int _num_objects;

        static std::thread _thread;

        static bool _thread_running;
    };

}