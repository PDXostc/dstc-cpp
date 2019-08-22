# dstc-cpp
Implementation of interface layer on top of DSTC with C++ bindings to eliminate need of macros/code generation that, while compatible with `gcc`, is not compatible with `g++`.  This also provides tools and helper routines geared toward C++ developers who want to use DSTC.

# Project Status #

Currently, only client-side interfaces are available, although there are plans to provide similar server functionality.

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
sudo apt install -y googletest gcovr
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