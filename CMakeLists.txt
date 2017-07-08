
cmake_minimum_required(VERSION 2.8.11)
project (pixelsort)

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

SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

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

#########################################
# Find and set up ImageMagick

# Default ImageMagick/Magick++ setup
find_package(ImageMagick 7
    COMPONENTS Magick++
    REQUIRED
)

# Custom ImageMagick/Magick++ setup
add_definitions( -DMAGICKCORE_QUANTUM_DEPTH=16 )
add_definitions( -DMAGICKCORE_HDRI_ENABLE=1 )

message(STATUS "Include: ${ImageMagick_INCLUDE_DIRS}")
message(STATUS "Libs: ${ImageMagick_LIBRARIES}")

include_directories(${ImageMagick_INCLUDE_DIRS})

####################################
# Boost setup
find_package(Boost REQUIRED COMPONENTS program_options)
include_directories(${Boost_INCLUDE_DIRS})

###################################
# Google Test setup
add_subdirectory(googletest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

###################################
# PixelSort library setup
add_library(
    PixelSortLib
    lib/Coordinate.cpp
    lib/Comparator.cpp
    lib/Matcher.cpp
    lib/PixelSort.cpp
)
include_directories("${PROJECT_SOURCE_DIR}/lib")
add_subdirectory(lib)

###################################
# runner setup
include_directories("${PROJECT_SOURCE_DIR}/runner")
add_subdirectory(runner)

###################################
# Final executable generation
add_executable(
    pixelsort 
    runner/PixelSortApp.cpp
    runner/interactive.cpp
)

target_link_libraries(
    pixelsort 
    PixelSortLib
    Qt5::Widgets 
    ${ImageMagick_LIBRARIES}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
)

###################################
# unit tests
add_executable(runUnitTests test/testSetup.cpp)
target_link_libraries(
    runUnitTests 
    PixelSortLib 
    ${ImageMagick_LIBRARIES}
    gtest 
    gtest_main
)
add_test(runUnitTests runUnitTests)

####################################
# Install the executable
install(TARGETS pixelsort DESTINATION bin)

####################################
# CPack stuff
include(CPack)



