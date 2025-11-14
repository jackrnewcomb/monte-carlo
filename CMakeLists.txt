# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Lab6 VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB INCLUDES ${PROJECT_SOURCE_DIR}/include/*.hpp)
add_executable(client ${SOURCES} ${INCLUDES})


file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB INCLUDES ${PROJECT_SOURCE_DIR}/include/*.hpp)
add_executable(test ${SOURCES} ${INCLUDES})

