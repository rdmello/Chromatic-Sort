
cmake_minimum_required(VERSION 2.8.11)
project (pixelsort)

####################################
# Set Wall and Werror
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror --std=c++14")
endif()

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
# src and runners setup
include_directories("${PROJECT_SOURCE_DIR}/src")
add_subdirectory(src)

include_directories("${PROJECT_SOURCE_DIR}/runners")
add_subdirectory(runners)

###################################
# Final executable generation
add_executable(
    pixelsort 
    src/Coordinate.cpp
    src/Comparator.cpp
    src/Matcher.cpp
    src/PixelSort.cpp
    runners/interactive.cpp
)

target_link_libraries(
    pixelsort 
    Qt5::Widgets 
    ${ImageMagick_LIBRARIES}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
)

####################################
# Install the executable
install(TARGETS pixelsort DESTINATION bin)

