
cmake_minimum_required(VERSION 2.8.11)
project (pixelsort)

# setting some basic properties
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

####################################
# Set Wall and Werror
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror --std=c++14")
endif()

##########################################
# Setting the -rpath correctly for macOS
# from the cmake.org wiki

# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 

SET(CMAKE_INSTALL_RPATH "\$ORIGIN/lib;${CMAKE_INSTALL_PREFIX}/lib")

SET(CMAKE_BUILD_RPATH "\$ORIGIN/lib")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# the RPATH to be used when installing, but only if it's not a system directory
LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
IF("${isSystemDir}" STREQUAL "-1")
    SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
ENDIF("${isSystemDir}" STREQUAL "-1")

#####################################
# Set up Qt5
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets)

# release mode for Qt5 on macOS
# if(APPLE)
#     SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L /Users/Rylan/Desktop/Projects/cpp/Qt/qt-target-5.9/lib -l qtlibpng -lqtpcre2 -lqtfreetype -lqtharfbuzz -L /Users/Rylan/Desktop/Projects/cpp/Qt/qt-target-5.9/plugins/platforms -l Qt5PrintSupport -l Qt5FontDatabaseSupport -l Qt5GraphicsSupport -l Qt5CglSupport -l Qt5AccessibilitySupport -l Qt5ClipboardSupport -l Qt5ThemeSupport -lqcocoa -framework Cocoa -framework CoreText -framework CoreFoundation -framework CoreGraphics -framework CoreServices -lz -lcups -framework OpenGL -framework IOKit -framework Carbon -lQt5Core -lQt5Gui -lQt5Widgets")
# endif(APPLE)

####################################
# Boost setup
SET(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS program_options)
include_directories(${Boost_INCLUDE_DIRS})

###################################
# Google Test setup
enable_testing()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

###################################
# PixelSort library setup
add_library(
    PixelSortLib
    src/core/Coordinate.cpp
    src/core/Comparator.cpp
    src/core/Matcher.cpp
    src/core/PixelSort.cpp
    src/core/driver/InMemDriver.cpp
    src/core/driver/QImageDriver.cpp
)
include_directories("${PROJECT_SOURCE_DIR}/src/core")
include_directories("${PROJECT_SOURCE_DIR}/src/core/driver")
add_subdirectory(src/core)
add_subdirectory(src/core/driver)
target_link_libraries(
    PixelSortLib
    Qt5::Widgets
)

###################################
# GUI setup
include_directories("${PROJECT_SOURCE_DIR}/src/gui-qt")
add_subdirectory(src/gui-qt)

###################################
# Final executable generation
add_executable(
    pixelsort 
    MACOSX_BUNDLE
    src/gui-qt/PixelSortApp.cpp
    src/gui-qt/PixelSortOptions.cpp
    src/gui-qt/interactive.cpp
)

target_link_libraries(
    pixelsort 
    PixelSortLib
    Qt5::Widgets 
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
)

###################################
# unit tests
add_executable(
    allTests 
    test/testSetup.cpp
    test/core/driver/testInMemDriver.cpp
)

target_link_libraries(
    allTests 
    PixelSortLib 
    gtest 
    gtest_main
)

add_test(testSetup allTests)
add_test(testInMemDriver allTests)

####################################
# Install the executable
install(TARGETS pixelsort DESTINATION bin)

####################################
# CPack stuff
set(CPACK_PACKAGE_NAME "Chromatic Sort")
include(CPack)



