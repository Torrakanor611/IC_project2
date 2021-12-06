cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set the project name
project(IC_project2 VERSION 1.0)

# add the librarys
add_library(bitstream src/bitstream/BitStream.cpp)
add_library(golomb src/golomb/Golomb.cpp)
add_library(codecimg src/codecimg/Codecimg.cpp)

# add the executable
add_executable(main ${CMAKE_SOURCE_DIR}/src/main.cpp)

target_link_libraries(main PUBLIC bitstream)

target_link_libraries(golomb bitstream)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)