# 2Queue cache implementation on C++

## Generate

    cmake -S . -B <build_dir>

## Guild

    cmake --build <build_dir>

<hr>

# Binaries

- ## <build_dir>/bin/app
- ## <build_dir>/bin/test
- ## <build_dir>/bin/idealCache

<hr>

# IO data format

## **Input**:
### <cache_size> <n_elems> <elem1, ...>
<br>

## **Output**:
### <n_hits>
<br>

## **Example**:
### Input
### 5 10 0 1 2 3 4 0 1 2 3 4
### Output
### 5