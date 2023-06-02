##################################################
cmake_minimum_required(VERSION 2.8.9)
project(EXCHANGE_INFO)
cmake_policy(SET CMP0028 NEW)
cmake_policy(SET CMP0046 NEW)
include_directories(include ${cpprestsdk_INCLUDE_DIRS})
include(ExternalProject)
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/cmake-files)

find_package(boost REQUIRED)
find_package(cpprestsdk REQUIRED)
find_package(Threads  REQUIRED)
#find_package(OpenSSL REQUIRED)
find_package(spdlog REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(exchangeInfo SHARED  ${CMAKE_SOURCE_DIR}/src/exchangeInfo.cpp)
add_executable(output  ${CMAKE_SOURCE_DIR}/src/main.cpp)
message("BOOST LIB " ${Boost_LIBRARIES})
target_link_libraries(output exchangeInfo ${cpprestsdk_LIBRARIES}  ${Boost_LIBRARIES}  )
add_dependencies(output exchangeInfo  )
# add_dependencies(output exchangeInfo)
