# Binance Exchange Information Handler: C++

## GETTING STARTED

The application interacts with binance exchange and collects relavent inforamtion for each symbol and

## PROJECT HIRARCHY

    .
    ├──build
    ├── unittest
    │   ├── unittest.cpp
    |   ├── CMakELists.txt
    ├── BENCHMARK
    │   ├── benchmark.cpp
    |   ├── CMakELists.txt
    ├── include
    │   ├── exchangeInfo.h  
    ├── src
    │   ├── exchnageInfo.cpp
    |   ├── main.cpp
    |   
    ├── files
    │   ├── answer.json
    |   ├── queryfile.json
        ├── configFile.json
    ├──CMakeList.txt
    ├──.gitignore
    ├──Dockerfile
    ├──CMakELists.txt

First, create a directory for your project:

```
mkdir CPP-PROJECT
```

```
 mkdir build & cd build
 ```

### FILES

```answer.json``` To store output of each query.

```query.json``` Contains queries to be processed

```configFile.json``` contains configuration details(logging information, API url and refresh interval)

```logFile.txt``` For maintaining logs.

## RUNING UNIT TESTS

execute the command ```cmake -DBUILD_UNITTESTS=ON ..``` to build test and execute  ```ctest```  to run binary. Alternatively, ```./unittest/runUnitTests``` to do so.

### OUTPUT

## Building Benchmarks

To build benchmark run the command  

```
cmake -DBUILD_BENCHMARKS=ON -DBENCHMARK_DOWNLOAD_DEPENDENCIES=ON ..
```

on terminal. where .. represent location of CMakeLists.txt file in root directory. ```-D``` is used to set ```BUILD_BENCHMARKS```  value to ON.

## Running Benchmarks

Benchmarks are executed by running the produced binaries. Benchmarks binaries,
by default, accept options that may be specified either through their command

## Output Formats
  