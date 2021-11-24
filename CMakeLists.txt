cmake_minimum_required(VERSION 3.10)

# set the project name
project(IC_project2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# add the bitstream library
add_subdirectory(src/bitstream)

# add the executable
add_executable(main ${CMAKE_SOURCE_DIR}/src/main.cpp)

target_link_libraries(main PUBLIC bitstream)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
target_include_directories(main PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/bitstream"
                          )