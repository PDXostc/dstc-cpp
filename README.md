# dstc-cpp
Implementation of interface layer on top of DSTC with C++ bindings to eliminate need of macros/code generation that, while compatible with `gcc`, is not compatible with `g++`.  This also provides tools and helper routines geared toward C++ developers who want to use DSTC.

# Project Status #

Currently, only client-side interfaces are available, although there are plans to provide similar server functionality.

# Differences between DSTC and DSTCPP

The primary difference is that DSTCPP does not use macros for any of the code generation, preferring instead to use templates and objects in their stead.

This means that accessing a server's function can be done by creating a `dstc::RemoteFunction` object.  Macros such as `DSTC_DECL_DYNAMIC_ARG` are replaced by the underlying data structure (`dstc_dynamic_data_t`).  Callbacks have been re-implemented to allow callback routines to be declared on the fly using lambda expressions or be bound to existing global functions or member functions (`std::function`).  This effectively replaces the `DSTC_DECL_CALLBACK_ARG` with a `dstc::CallbackFunction` which wraps a `std::function` object that gets invoked when the callback occurs.  This has the added benefit of declaring the callback arguments when the original `dstc::RemoteFunction` object gets created, as discussed later.  (See more examples in `test/client_tests.cpp`).

# Usage

## Client-side, by example

For this example, imagine that you have a server function that accepts a variable-length string, and then returns an `unsigned int` that is the number of characters in the string.  The DSTC server may have a macro generation that looks like this:

```c
DSTC_SERVER(calc_strlen, DSTC_DECL_DYNAMIC_ARG, DSTC_DECL_CALLBACK_ARG)
```

To execute this function, first we must create a RemoteFunction object, using code like this:

```cpp
const char calc_strlen_name[] = "calc_strlen";
dstc::RemoteFunction<calc_strlen_name, dstc_dynamic_data_t, dstc::CallbackFunction<int>> remoteCalcStrlen;
```

Note that `calc_strlen_name` much match the string in the `DSTC_SERVER` macro (because `DSTC` uses strings for function identification.)  It also must be const because the template generates code using this as a global `const`. Please also note that instead of using the macro `DSTC_DECL_DYNAMIC_ARG`, we simply specify that one of the arguments is a `dstc_dynamic_data_t`, which makes the argument list more clear.  Also, as discussed in a preceding section, the arguments of the callback routine are embedded into the object creation, a `dst::CallbackFunction`.  (In this example, we expect the callback function to have an `int` as an argument).

Now, we can ensure the server has started up by using code like this:

```cpp
dstc::EventLoopRunner runner;
remoteCalcStrlen.blockUntilServerAvailable(runner);
```

Please note that using an `EventLoopRunner` is optional... the only thing this object does is ensure a thread is running that is periodically calling `dstc_process_events` as long as the `EventLoopRunner` is in scope.  You can alternatively create your own loop and use dstc::RemoteFunction::serverAvailable() to check if the server is running.

Now, to execute the function, we simply call the `RemoteFunction` with the appropriate parameters.  (Note that since this function uses standard templates, modern IDEs such as VS Code will remind you of argument lists as you type):

```cpp
char string[] = "string to calculate length of";
dstc_dynamic_data_t arg;
arg.length = strlen(string) + 1; // +1 for `\0`
arg.data = (void*) string;
remoteCalcStrlen(
    arg,
    dstc::CallbackFunction<int> (
        [](int str_len) {
            std::cout << "strlen: " << str_len << std::endl;
        }
    )
);
```

Please note that in this example we used a lambda expression for the callback.  However, you can also construct a dstc::CallbackFunction object using any standard std::function that has the required argument list and has no return (`void`).

Detailed examples of interfacing with a DSTC server with various types of parameters can be seen in `test/client_tests.cpp`.

# Dependencies

Build instructions are for Ubuntu 18.04, although these should be similar to other platforms.

## RMC ##

Install `Reliable Multicast` (`RMC`) using the instructions found on [its github page](https://github.com/PDXostc/reliable_multicast).

## DSTC ###

Install `DSTC` using the instructions found on [its github page](https://github.com/PDXostc/dstc).

## CMake ##

```
sudo apt install -y cmake
```

## gtest (for tests) ##

```
sudo apt install -y googletest
pushd /usr/src/googletest
sudo cmake CMakeLists.txt
sudo make -j$(nproc)
sudo cp googlemock/*.a /usr/lib
sudo cp googlemock/gtest/*.a /usr/lib
popd
```

# Building

## Normal build ##

```
cmake .
make -j ${nproc}
```

## Running tests ##

### Setup ###

If necessary for how you installed `DSTC`/`RMC`, set your `LD_LIBRARY_PATH` so the appropriate `.so` files can be found.

### Executing tests ###

These tests rely on DSTC being shut down between some tests so that remote functions are no longer being advertised at different points of the test.  Therefore, a wrapper function has been ran that executes each test within the `run_tests` binary that gets generated above.

```
./run_tests_separate_processes.py
```