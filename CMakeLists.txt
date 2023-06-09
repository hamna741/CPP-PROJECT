##################################################
cmake_minimum_required(VERSION 2.8.9)
project(EXCHANGE_INFO)
cmake_policy(SET CMP0028 NEW)
cmake_policy(SET CMP0046 NEW)
include_directories(include)

#include_directories(include ${cpprestsdk_INCLUDE_DIRS})
#include(ExternalProject)
#list(APPEND CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/cmake-files)

find_package(Boost REQUIRED system)
find_package(cpprestsdk REQUIRED)
find_package(Threads  REQUIRED)
#find_package(OpenSSL REQUIRED)
find_package(spdlog REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include)
option(BUILD_TESTS "Build unit tests" OFF )
option(BUILD_BENCHMARKS "Build benchmarks" OFF)
if(BUILD_TESTS)
    message("Building GTEST")
    add_subdirectory(./googletest)
    add_subdirectory(./unittest)
  
endif()

if(BUILD_BENCHMARKS)
    message("Building benchmarks")
    add_subdirectory(./benchmark)
    add_subdirectory(./BENCHMARK)
endif()

add_library(exchangeInfo SHARED  ${CMAKE_SOURCE_DIR}/src/exchangeInfo.cpp)
add_executable(output  ${CMAKE_SOURCE_DIR}/src/main.cpp)
# target_link_libraries(output exchangeInfo ${Boost_LIBRARIES} ${cpprestsdk_LIBRARIES}    )
target_link_libraries(output exchangeInfo cpprestsdk::cpprest Boost::system Threads::Threads OpenSSL::Crypto spdlog::spdlog )
add_dependencies(output exchangeInfo  )
# add_dependencies(output exchangeInfo)
