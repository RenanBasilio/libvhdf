# CMake build script for building an executable application.
#
# Building from source:
# > cmake . -Bbuild -DLIBDIR:STRING=<library directory>
# > cmake --build .

cmake_minimum_required(VERSION 3.9)
project(vhdf
        DESCRIPTION "Virtual hard disk file library."
        VERSION 0.0.0)

include(GNUInstallDirs)
enable_language(C)

# Add header directory to project
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Add a submodule to the project
# include(libs/*/)

# Add an external library to the project
# find_package(LIB REQUIRED)
# include_directories(${LIB_INCLUDE_DIRS})

# Declare source files for build
set(
    SOURCE_FILES
    src/vhdf.cpp
)

set(
    HEADER_FILES 
    include/vhdf.hpp
)

set( 
    PRIVATE_HEADER_FILES
)

# Set the name of the built executable
set(LIB_NAME "${PROJECT_NAME}")

add_library(${LIB_NAME} SHARED ${SOURCE_FILES})
set_target_properties(
    ${LIB_NAME} PROPERTIES
    VERSION 0
    SOVERSION 0
    PUBLIC_HEADER "${HEADER_FILES}"
    PRIVATE_HEADER "${PRIVATE_HEADER_FILES}"
)
add_definitions(-DLIBVHDF_EXPORTS)

target_include_directories(${LIB_NAME} PUBLIC ${INCLUDE_DIRECTORIES})
target_include_directories(${LIB_NAME} PUBLIC include)

option(BUILD_TESTS "Build Tests." OFF)
if(BUILD_TESTS)
    include(test/CMakeTests.cmake)
endif(BUILD_TESTS)

# Link with an external package
# target_link_libraries(${EXE_NAME} ${LIB_LIBRARIES})

# Link with a submodule
# target_link_libraries(${EXE_NAME} ${SUBPROJECT_NAME})

# Copy dynamic libraries to executable directory if necessary
# add_custom_command(
#     TARGET ${PROJECT_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#         ${LIB_SHARED_LIB}
#         $<TARGET_FILE_DIR:${PROJECT_NAME}>
# )