# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Lab6 VERSION 1.0)

# Ask CMake to locate MPI on the system
find_package(MPI REQUIRED)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB INCLUDES ${PROJECT_SOURCE_DIR}/include/*.hpp)
add_executable(monte-carlo ${SOURCES} ${INCLUDES})

# Link against MPI
target_link_libraries(monte-carlo MPI::MPI_CXX)