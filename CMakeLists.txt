cmake_minimum_required(VERSION 3.24)
project(SoftwareRenderer CXX)

# Stop in source builds
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

set(3RDPARTY_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/3rdparty)

set(APP_NAME SoftwareRendererApp)

add_subdirectory(src)
