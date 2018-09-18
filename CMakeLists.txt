# CMake build script for building an executable application.
#
# Building from source:
# > cmake . -Bbuild -DLIBDIR:STRING=<library directory>
# > cmake --build .

cmake_minimum_required(VERSION 3.9)
project(vhdf
        DESCRIPTION "Virtual hard disk file library."
        VERSION 1.0.0)

include(GNUInstallDirs)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 11)

# Add header directory to project
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

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
    VERSION 1.0.0
    SOVERSION 1.0.0
    PUBLIC_HEADER "${HEADER_FILES}"
    PRIVATE_HEADER "${PRIVATE_HEADER_FILES}"
)
add_definitions(-DLIBVHDF_EXPORTS)

target_include_directories(${LIB_NAME} PUBLIC ${INCLUDE_DIRECTORIES})
target_include_directories(${LIB_NAME} PUBLIC include)

install(TARGETS ${LIB_NAME} DESTINATION lib)

option(BUILD_TESTS "Build Tests." OFF)
if(BUILD_TESTS)
    include(test/CMakeTests.cmake)
endif(BUILD_TESTS)
