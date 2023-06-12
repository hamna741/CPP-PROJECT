
#spdlog
ExternalProject_Add(spdlog_external
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG master
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)

ExternalProject_Get_Property(spdlog_external source_dir binary_dir)
set(spdlog_INCLUDE_DIRS ${source_dir}/include)
