cmake_minimum_required(VERSION 3.20)
project(mlp-cli)

# System Requirements
if(NOT LINUX)
    message(FATAL_ERROR "Invalid Operating System '${CMAKE_SYSTEM_NAME}'. You must use a Linux based Operating System.")
endif()

# Required Sets
set(CMAKE_C_STANDARD 23)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_STATIC_LIBRARY_PREFIX "")

# Optional Sets
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_COMPILER "gcc")

# Files needed to create libraries
file(GLOB LIB_SRC_FILES CMAKE_CONFIGURE_DEPENDS "lib/*.c")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# build .so and .a files
add_library(objlib OBJECT ${LIB_SRC_FILES})
set_property(TARGET objlib PROPERTY POSITION_INDEPENDENT_CODE 1)

add_library(mlp_shared SHARED $<TARGET_OBJECTS:objlib>)
set_property(TARGET mlp_shared PROPERTY OUTPUT_NAME mlp)

add_library(mlp_static STATIC $<TARGET_OBJECTS:objlib>)
set_property(TARGET mlp_static PROPERTY OUTPUT_NAME mlp)

# build mlp-cli
file(GLOB SRC_FILES CMAKE_CONFIGURE_DEPENDS "src/*.c")

add_executable(mlp-cli ${SRC_FILES})
target_link_libraries(mlp-cli ${CMAKE_CURRENT_SOURCE_DIR}/bin/mlp.a m)