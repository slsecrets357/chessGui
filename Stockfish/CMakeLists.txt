cmake_minimum_required(VERSION 3.10)
project(Stockfish)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the compilation flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

# Detect the platform
if (WIN32)
    add_definitions(-DNO_PREFETCH)
endif()

# Add the source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Include directories
include_directories(src)

# Add the executable
add_executable(stockfish ${SOURCES})

# Set the output directory
set_target_properties(stockfish PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Link libraries
if (WIN32)
    target_link_libraries(stockfish ws2_32)
endif()
