# Binance Exchange Information Handler: C++

## Table of contents

1. [PROJECT HIRARCHY](#project-hirarchy)
2. [FILES](#files)
3. [GETTING STARTED](#getting-started)
    - [Dockerfile](#dockerfile)
    - [Top Level CMake File](#top-level-cmake-file)
    - [Running Unit tests](#running-unit-tests)
    - [Running Benchmarks](#running-benchmarks)

The application interacts with binance exchange and collects relavent information for each symbol and stores it in file.

## PROJECT HIRARCHY

    .
    ├──build
    ├── unittest
    │   ├── unittest.cpp
    |   ├── CMakeLists.txt
    ├── BENCHMARK
    │   ├── benchmark.cpp
    |   ├── CMakeLists.txt
    ├── include
    │   ├── exchangeInfo.h  
    ├── src
    │   ├── exchangeInfo.cpp
    |   ├── main.cpp
    |   
    ├── files
    │   ├── answer.json
    |   ├── queryfile.json
        ├── configFile.json
    ├──CMakeList.txt
    ├──.gitignore
    ├──Dockerfile
    ├──CMakeLists.txt

### FILES

```answer.json``` To store output of each query.

```query.json``` Contains queries to be processed

```configFile.json``` contains configuration details(logging information, API url and refresh interval)

```json
{
    "logging": {
        "level": "trace",
        "file": true,
        "console": true
    },
    "exchange_info_url": "fapi.binance.com/fapi/v1/exchangeInfo",
    "request_interval": 60
 }
```

To enable or disable file or console logs set file  and console value to either true or false respectively.

```logFile.txt``` For maintaining logs.

## GETTING STARTED

### Dockerfile

To build docker image execute following command.

```
docker build -t project .
```

and to run container following command.

```
docker run --rm -v "$(pwd):/CPP-PROJECT" -it project
```

```-rm``` flag for removing conatiner after it stops.

```-v``` flag for volume mapping cuurent working directory to CPP-PROJECT direcotry in docker conatiner.

```-it``` for running container in interactive mode.

Navigate to build folder (``` cd build ```) to configure and build project.

### Top Level CMake File

The top level cmake file creates executable output and links it to lshared library ExchangeInfo and cpprestsdk, Boost, Threads, OpenSSL and spdlog.

### Running Unit tests

Execute the command

```
cmake -DBUILD_UNITTESTS=ON ..
```

 to build test and execute  ```ctest```  to run binary. Alternatively, ```./unittest/runUnitTests``` to do so.

### Running Benchmarks

To build benchmark run the command  

```
cmake -DBUILD_BENCHMARKS=ON -DBENCHMARK_DOWNLOAD_DEPENDENCIES=ON ..
```

on terminal. where .. represent location of CMakeLists.txt file in root directory. ```-D``` is used to set ```BUILD_BENCHMARKS```  value to ON.
