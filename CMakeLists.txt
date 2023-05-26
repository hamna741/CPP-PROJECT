cmake_minimum_required(VERSION 2.8.9)
PROJECT(EXCHANGE_INFO)

include_directories(include)

find_package( Boost REQUIRED system )
find_package( cpprestsdk REQUIRED)
find_package( Threads  REQUIRED)
find_package( OpenSSL REQUIRED)

add_library(exchangeInfo SHARED src/exchangeInfo.cpp)
target_link_libraries(exchangeInfo )
add_executable(output src/main.cpp)
target_link_libraries(output exchangeInfo cpprestsdk::cpprest Boost::system Threads::Threads OpenSSL::Crypto)
