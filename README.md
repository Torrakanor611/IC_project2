# IC_project2

******
******

## Prerequisites

On Ubuntu you need the following packages installed: 
_cmake_, _doxygen_, _git_.

```
sudo apt install cmake doxygen git
```

In other Linux distributions you need equivalent packages installed.

******

## Compiling the code

In a terminal, enter the **build** directory of your project

```
mkdir build && cd build
```

Then compile the code

```
cmake ..
make
```

## Compiling documentation

In terminal, on root
```
doxygen doxyconfig
```

## Requirements (Ubuntu 20.04)
### matplotlib-cpp
we use matplotlib-cpp that is a wrapper for matplot python library
>sudo apt install python3 python3-dev

>pip3 install matplotlib

### libsndfile
>sudo apt install autoconf autogen automake build-essential libasound2-dev libflac-dev libogg-dev libtool libvorbis-dev libopus-dev libmp3lame-dev libmpg123-dev pkg-config python
>sudo apt-get install libsndfile1-dev

### opencv4
>sudo apt install libopencv-dev python3-opencv


## Directories

>files contains resources to be used in the tests

>test contains tests to the several modules

>src contains classes that implement the project modules