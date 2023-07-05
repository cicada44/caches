# Caches

This repository contains the implementation of two different types of caches: the 2Q cache and the ideal cache. The project uses the CMake build system to compile the code into three executable files: `app`, `idealCache`, and `tests`.

## Overview

The 'caches' repository provides two cache implementations that can be used for different purposes:

1. **2Q Cache**: The 2Q cache is a type of cache replacement algorithm that maintains two separate queues to manage frequently and recently used items efficiently.

2. **Ideal Cache**: The ideal cache is a theoretical cache model that assumes infinite cache capacity and perfect knowledge of the future item requests. This cache is used as a benchmark to compare the performance of other cache algorithms.

## Building the Project

To build the project, follow these steps:

1. Clone the repository to your local machine:

        git clone https://github.com/your-username/caches.git



2. Navigate to the project directory:

        cd caches

3. Create a build directory:

        mkdir build
        
4. Generate and compile files

        cmake -S . -B build && cmake --build build

After successful compilation, you will find three executable files (`app`, `idealCache`, and `tests`) in the build directory.

## Usage

### **app**

The `app` executable provides a simple demonstration of the 2Q cache implementation. It simulates cache operations and prints the results. You can run it from the command line:

    ./build/bin/app

### **idealCache**

The `idealCache` executable showcases the ideal cache behavior. It is a reference implementation for comparison purposes. Execute it with:

    ./build/bin/idealCache

### **tests**

The `tests` executable contains unit tests for the cache implementations. Running the tests ensures the correctness of the code. To execute the tests, use:

    ./build/bin/tests

## I/O data format

The I/O data format is:

    Input:
    <size of cache elements> <amount of elements>
    <elements>

    Output:
    <number of hits>


Example

    5 5
    1 2 3 4 5
    0

## Contributing

Contributions to this repository are welcome! If you find any issues or have suggestions for improvements, please submit them as GitHub issues or create a pull request.

## License

This repository is licensed under the UNLICENSE. Feel free to use the code examples for anything purposes, modify them, and distribute them as needed.
