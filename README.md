
# Chromatic Sort

This is a desktop application for manipulating images using a graphical technique called "PixelSorting". 

Written in C++14 using Qt and cmake, Chromatic Sort contains a high-performance multithreaded core with minimal memory requirements. 

Available soon on Windows, macOS, and Linux operating systems.   

## Download

Standalone downloads should be available soon. Currently, the project has to be built from source to be used. See the 'Building' section below.

## Features

* Provides an interface for two common pixelsort idioms: Asendorf sort and block sort

* Fine grained control of colors and shapes being sorted

* Built-in tools for repeating pixelsort operations through an image

* The app contains both a GUI and a command-line interface for easy scripting

* Image processing back-end is configurable between `QImage`, `Magick::Image`, and `stb_image`

* Written in a cross-platform subset of standard C++14 and compiled using `-Wall -Werror` / `-W4`

* Cross-platform `cmake` project which uses the `Qt` GUI toolkit

## Building

### Installing dependencies

This requires a C++14 compatible compiler. Visual C++ 2017 for Windows, Clang for macOS and g++ for Linux are recommended. 

There are three additional dependencies required: `Qt`, `CMake`, and `boost`:

1. [Download](https://info.qt.io/download-qt-for-application-development) and install the `Qt5` libraries for your system. 

2. [Download](https://cmake.org/download/) and install `cmake` for your system. 

3. There is currently an additional dependency on `boost` which we're trying to phase-out. [Download](http://www.boost.org/users/download/) and install `boost` for your system. Follow the [getting started](http://www.boost.org/doc/libs/1_64_0/more/getting_started/) steps to compile the `Program Options` library.

### Get Chromatic Sort source code

Download or clone this project's source code using 
```
git clone https://github.com/rdmello/PixelSort.git --recursive
```

The `--recursive` flag is required since this project includes `google test` as a Git submodule which is downloaded and compiled to run tests. 

### Build using CMake

An out-of-tree build is recommended with CMake. Here are the steps for this: 

1. Create a new subdirectory in the top-level Chromatic Sort directory called `build` and change working directory to this new subdirectory: 

```
mkdir build
cd build
```

2. Create Makefiles using `cmake`. On Unix (macOS or Linux) platforms, this should be sufficient:

```
cmake ..
```

On Windows, you may need to specify the version of Visual Studio and the project bitness: 

```
cmake .. -G "Visual Studio 15 2017 Win64"
```

This `cmake` command is essential for correct operation of the executable. If `cmake` shows any errors, you should investigate it and try to fix it. In particular, you may need to set the `CMAKE_PREFIX_PATH` on Windows to help `cmake` find your `Qt` installation. 

3. Building the project using a C++ compiler. On Unix systems, `automake` should just work:

```
make
make test
./pixelsort
```

On Windows, `cmake` should create a Visual Studio solution file (`.sln`). Open this `.sln` file in the version of Visual Studio used earlier in the `cmake` command. Then build the project and execute it from the GUI. For optimal performance, build in `Release x64` mode. 

## Todos

1. Add another statusbar entry for image name, dimensions

2. Modify sort rotation selector to allow for more angles

3. Add Zoom and pan controls to the image display

4. Maybe move the GUI toolkit to something non-Qt so that we can release this under a more permissive license?

## Command-line interface

The PixelSort application should be accessible from a command-line interface. Type `pixelsort --help` for more information on the options for this. 

