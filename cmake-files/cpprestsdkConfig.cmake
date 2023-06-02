
# cpprestsdk
ExternalProject_Add(cpprestsdk_external
    GIT_REPOSITORY https://github.com/microsoft/cpprestsdk.git
    GIT_TAG master
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""

)

ExternalProject_Get_Property(cpprestsdk_external source_dir binary_dir)
set(cpprestsdk_INCLUDE_DIRS ${source_dir}/Release/include)
set(cpprestsdk_LIBRARY_DIRS ${binary_dir}/Release/lib)
set(cpprestsdk_LIBRARIES cpprestsdk)
