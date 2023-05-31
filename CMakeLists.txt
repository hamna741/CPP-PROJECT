# cmake_minimum_required(VERSION 2.8.9)
# PROJECT(EXCHANGE_INFO)
# option(BUILD_BENCHMARKS "Build benchmarks" OFF)


# include_directories(include)

# find_package( Boost REQUIRED system )
# find_package( cpprestsdk REQUIRED)
# find_package( Threads  REQUIRED)
# find_package( OpenSSL REQUIRED)
# find_package(spdlog REQUIRED)
# add_library(exchangeInfo SHARED src/exchangeInfo.cpp)
# target_link_libraries(exchangeInfo )
# add_executable(output src/main.cpp)
# target_link_libraries(output exchangeInfo cpprestsdk::cpprest Boost::system Threads::Threads OpenSSL::Crypto)
# if(BUILD_BENCHMARKS)
#     message("Building benchmarks")
#     add_subdirectory(/home/hamna/Desktop/myproject/CPP-PROJECT/benchmark)
#     add_subdirectory(/home/hamna/Desktop/myproject/CPP-PROJECT/BENCHMARKING)
# endif()
############################################################################################################################
cmake_minimum_required(VERSION 2.8.9)
project(EXCHANGE_INFO)
cmake_policy(SET CMP0028 NEW)
include_directories(include)


include(ExternalProject)

set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)

# Boost
ExternalProject_Add(boost_external
    GIT_REPOSITORY https://github.com/boostorg/boost.git
   
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}/boost
)
ExternalProject_Get_Property(boost_external source_dir binary_dir)
set(Boost_INCLUDE_DIRS ${source_dir})
set(Boost_LIBRARY_DIRS ${binary_dir}/stage/lib)
set(Boost_LIBRARIES Boost::system)

# cpprestsdk
ExternalProject_Add(cpprestsdk_external
    GIT_REPOSITORY https://github.com/microsoft/cpprestsdk.git
   
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}/cpprestsdk
)
ExternalProject_Get_Property(cpprestsdk_external source_dir binary_dir)
set(cpprestsdk_INCLUDE_DIRS ${source_dir}/Release/include)
set(cpprestsdk_LIBRARY_DIRS ${binary_dir}/Release/lib)
set(cpprestsdk_LIBRARIES cpprestsdk::cpprest)

# OpenSSL
ExternalProject_Add(openssl_external
    GIT_REPOSITORY https://github.com/openssl/openssl.git
    GIT_TAG OpenSSL_1_1_1f
    GIT_SHALLOW    TRUE
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}/openssl
)
ExternalProject_Get_Property(openssl_external source_dir binary_dir)
set(OpenSSL_INCLUDE_DIRS ${source_dir}/include)
set(OpenSSL_LIBRARY_DIRS ${binary_dir})
set(OpenSSL_LIBRARIES OpenSSL::Crypto)

# spdlog
ExternalProject_Add(spdlog_external
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
  
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}/spdlog
)
ExternalProject_Get_Property(spdlog_external source_dir binary_dir)
set(spdlog_INCLUDE_DIRS ${source_dir}/include)

find_package( Boost REQUIRED system )
find_package( cpprestsdk REQUIRED)
find_package( Threads  REQUIRED)
find_package( OpenSSL REQUIRED)
find_package(spdlog REQUIRED)
add_library(exchangeInfo SHARED /home/hamna/Desktop/myproject/CPP-PROJECT/src/exchangeInfo.cpp)
target_link_libraries(exchangeInfo)

add_executable(output /home/hamna/Desktop/myproject/CPP-PROJECT/src/main.cpp)
target_link_libraries(output exchangeInfo ${cpprestsdk_LIBRARIES} ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${OpenSSL_LIBRARIES})


add_dependencies(exchangeInfo boost_external cpprestsdk_external openssl_external spdlog_external)
add_dependencies(output exchangeInfo)
##################################################################
# cmake_minimum_required(VERSION 2.8.9)
# project(EXCHANGE_INFO)

# include_directories(include)

# include(ExternalProject)

# set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)

# # Boost
# ExternalProject_Add(boost_external
#     GIT_REPOSITORY https://github.com/boostorg/boost.git
#     CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}/boost
# )
# ExternalProject_Get_Property(boost_external source_dir binary_dir)
# set(Boost_INCLUDE_DIRS ${source_dir})
# set(Boost_LIBRARY_DIRS ${binary_dir}/stage/lib)

# # cpprestsdk
# ExternalProject_Add(cpprestsdk_external
#     GIT_REPOSITORY https://github.com/microsoft/cpprestsdk.git
#     CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}/cpprestsdk
# )
# ExternalProject_Get_Property(cpprestsdk_external source_dir binary_dir)
# set(cpprestsdk_INCLUDE_DIRS ${source_dir}/Release/include)
# set(cpprestsdk_LIBRARY_DIRS ${binary_dir}/Release/lib)

# # spdlog
# ExternalProject_Add(spdlog_external
#     GIT_REPOSITORY https://github.com/gabime/spdlog.git
#     CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}/spdlog
# )
# ExternalProject_Get_Property(spdlog_external source_dir binary_dir)
# set(spdlog_INCLUDE_DIRS ${source_dir}/include)

# # zlib
# ExternalProject_Add(zlib_external
#     GIT_REPOSITORY https://github.com/madler/zlib.git
#     CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}/zlib
# )
# ExternalProject_Get_Property(zlib_external source_dir binary_dir)
# set(ZLIB_INCLUDE_DIRS ${source_dir})
# set(ZLIB_LIBRARY_DIRS ${binary_dir})

# # OpenSSL
# ExternalProject_Add(openssl_external
#     DEPENDS zlib_external
#     DOWNLOAD_DIR ${PROJECT_SOURCE_DIR}/downloads
#     URL http://openssl.org/source/openssl-1.0.0c.tar.gz
#  #  PATCH_COMMAND ${PROJECT_SOURCE_DIR}/bin/patch.exe -p1 < ${PROJECT_SOURCE_DIR}/downloads/openssl-1.0.0c.patch
#     CONFIGURE_COMMAND ""
#     BUILD_COMMAND ${CMAKE_COMMAND} -DWINST_DIR=${PROJECT_SOURCE_DIR} -DOPENSSL_SOURCE_DIR=${PROJECT_BINARY_DIR}/openssl-prefix/src/openssl/ -DOPENSSL_GENASM_COMMAND=${OPENSSL_GENASM_COMMAND} -DOPENSSL_CONFIGURE_COMMAND=${OPENSSL_CONFIGURE_COMMAND} -DOPENSSL_BUILD_COMMAND=${OPENSSL_BUILD_COMMAND} -DOPENSSL_INSTALL_COMMAND="${OPENSSL_BUILD_COMMAND} install" -P ${PROJECT_SOURCE_DIR}/configure-openssl.cmake
#     BUILD_IN_SOURCE 1
# )
# ExternalProject_Get_Property(openssl_external source_dir binary_dir)
# set(OpenSSL_INCLUDE_DIRS ${source_dir}/include)
# set(OpenSSL_LIBRARY_DIRS ${binary_dir})

# # Define the exchangeInfo target
# add_library(exchangeInfo SHARED src/exchangeInfo.cpp)
# add_dependencies(exchangeInfo boost_external cpprestsdk_external openssl_external spdlog_external)

# # Specify the include directories for the target
# target_include_directories(exchangeInfo PRIVATE
#     ${Boost_INCLUDE_DIRS}
#     ${cpprestsdk_INCLUDE_DIRS}
#     ${OpenSSL_INCLUDE_DIRS}
#     ${spdlog_INCLUDE_DIRS}
# )

# # Specify the library directories for the target
# link_directories(
#     ${Boost_LIBRARY_DIRS}
#     ${cpprestsdk_LIBRARY_DIRS}
#     ${OpenSSL_LIBRARY_DIRS}
# )

# # Specify the libraries to link against for the target
# target_link_libraries(exchangeInfo
#     boost_system  # Update with the Boost library name
#     cpprest  # Update with the cpprestsdk library name
#     ${OpenSSL_LIBRARY_DIRS}/libssl.a
#     ${OpenSSL_LIBRARY_DIRS}/libcrypto.a
#     spdlog  # Update with the spdlog library name
# )


# # Define the output target
# add_executable(output src/main.cpp)
# add_dependencies(output exchangeInfo)

# # Specify the include directories for the target
# target_include_directories(output PRIVATE
#     ${Boost_INCLUDE_DIRS}
#     ${cpprestsdk_INCLUDE_DIRS}
#     ${OpenSSL_INCLUDE_DIRS}
#     ${spdlog_INCLUDE_DIRS}
# )

# # Specify the library directories for the target
# link_directories(
#     ${Boost_LIBRARY_DIRS}
#     ${cpprestsdk_LIBRARY_DIRS}
#     ${OpenSSL_LIBRARY_DIRS}
# )

# # Specify the libraries to link against for the target
# target_link_libraries(output
#     exchangeInfo
#     boost_system  # Update with the Boost library name
#     cpprest  # Update with the cpprestsdk library name
#     ${OpenSSL_LIBRARY_DIRS}/libssl.a
#     ${OpenSSL_LIBRARY_DIRS}/libcrypto.a
#     spdlog  # Update with the spdlog library name
# )
