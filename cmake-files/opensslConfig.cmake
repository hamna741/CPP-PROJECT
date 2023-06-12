
#OpenSSL
set(OPENSSL_SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/openssl-src) # default path by CMake
set(OPENSSL_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/openssl)
set(OPENSSL_CONFIGURE_COMMAND ${OPENSSL_SOURCE_DIR}/config)
set(OpenSSL_INCLUDE_DIRS ${source_dir}/include)
set(OpenSSL_LIBRARY_DIRS ${binary_dir})
set(OpenSSL_LIBRARIES OpenSSL::Crypto)
set(OPENSSL_ROOT_DIR ${CMAKE_CURRENT_BINARY_DIR}/openssl-src )
ExternalProject_Add(
    openssl_external
  SOURCE_DIR ${OPENSSL_SOURCE_DIR}
  GIT_REPOSITORY https://github.com/openssl/openssl.git
  GIT_TAG OpenSSL_1_1_1f
  USES_TERMINAL_DOWNLOAD TRUE
  CONFIGURE_COMMAND
    ${OPENSSL_CONFIGURE_COMMAND}
    --prefix=${OPENSSL_INSTALL_DIR}
    --openssldir=${OPENSSL_INSTALL_DIR}
  BUILD_COMMAND make
  TEST_COMMAND ""
  INSTALL_COMMAND make installS
  INSTALL_DIR ${OPENSSL_INSTALL_DIR}
)
ExternalProject_Get_Property(openssl_external source_dir binary_dir)