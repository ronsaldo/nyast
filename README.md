# NyaST
A Minimalist Smalltalk Environment implemented in C++ 17

## About NyaST
Nyast is a minimalistic Smalltalk environment completely implemented in C++. For
this Smalltalk environment we are taking inspiration and some core base class
library from Pharo. One of the main objectives of this environment is to have an
embeddable solution C++ suitable for usage in environments where JIT compilation
is forbidden, such as video games. For this reason, the usage of a JIT is optional,
and having a standard ABI and calling conventions for AoT compilation is a desireable capability.
A second objective here is to have direct support for multi-threading, which
is provided by carefully crafting a minimalistic thread safe core runtime
environment.

## Building

NyaST can be built with CMake by running the following commands in bash:

```
mkdir build
cd build
cmake ..
cmake --build .
```

## Status

The following matrix describes the current implementation status of nyast:

| Feature Name | Status |
| ------------ | ------ |
