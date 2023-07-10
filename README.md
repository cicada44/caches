# 2Q cache implementation

This project is an implementation of the 2Q cache algorithm in C++. The 2Q cache algorithm is a hybrid caching algorithm that combines the advantages of both LRU (Least Recently Used) and LFU (Least Frequently Used) cache algorithms.

## Files

- [Cache classes](./src/cache/cache.hpp)
- [Main for 2Q cache](./src/app/main.cpp)
- [Main for ideal cache](./src/app/idealCache.cpp)

## Usage

To compile the all files, execute the following command:

     cmake --preset release
     cmake --build --preset release

To run the [tests](./test/test/test.cpp), execute the following command:

     ./build/release/bin/test

To run the [main for 2Q](./src/app/main.cpp), execute the following command:

     ./build/release/bin/app


To run the [main for ideal cache](./src/app/idealCache.cpp), execute the following command:

     ./build/release/bin/idealCache

## I/O data format:

    <nElems> <elems>
    hits

Example:

    Input:
    5
    1 2 3 4 5

    Output:
    0
