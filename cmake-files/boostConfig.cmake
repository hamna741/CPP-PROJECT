
#boost
ExternalProject_Add(boost_external
    GIT_REPOSITORY https://github.com/boostorg/boost.git
    # CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}/boost
    
    INSTALL_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
)
ExternalProject_Get_Property(boost_external source_dir binary_dir)

set(Boost_INCLUDE_DIRS ${source_dir})
set(Boost_LIBRARY_DIRS ${binary_dir}/stage/lib)

set(Boost_LIBRARIES Boost)
