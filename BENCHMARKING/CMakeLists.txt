cmake_minimum_required(VERSION 3.16)
project(Benchmark)
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

file(GLOB SRC_FILES ./*.cpp)
include_directories( /CPP-PROJECT/include)

add_executable(my_benchmark /CPP-PROJECT/BENCHMARK/benchmark.cpp)
target_sources(my_benchmark PRIVATE /CPP-PROJECT/src/exchangeInfo.cpp)
#target_sources(my_benchmark PRIVATE /CPP-PROJECT/src/main.cpp)
target_link_libraries(my_benchmark PRIVATE benchmark::benchmark cpprestsdk::cpprest Boost::system Threads::Threads  spdlog::spdlog ssl crypto)