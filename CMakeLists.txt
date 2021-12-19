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
add_library(preditorimg src/codecimg/preditor.cpp)

# add the executable
add_executable(main ${CMAKE_SOURCE_DIR}/src/main.cpp)

find_package(OpenCV REQUIRED)

# find python libraries
find_package(Python3 COMPONENTS Interpreter Development NumPy REQUIRED)
find_package(PythonLibs 3.0 REQUIRED)
message(${PYTHON_LIBRARIES})

# populate matplotlib repository
include(FetchContent)
FetchContent_Declare(
    matplotlib
    GIT_REPOSITORY https://github.com/lava/matplotlib-cpp.git
    GIT_TAG        f23347fca25219d1c42cbb91608b5556814bf572
)
FetchContent_GetProperties(matplotlib)
if(NOT matplotlib_POPULATED)
    FetchContent_Populate(matplotlib)
endif()

target_link_libraries(main PUBLIC bitstream)

target_link_libraries(golomb bitstream)

target_link_libraries(codecimg preditorimg)

target_link_libraries(codecimg ${OpenCV_LIBS} ${PYTHON_LIBRARIES} Python3::NumPy)


set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)