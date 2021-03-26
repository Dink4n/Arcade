cmake_minimum_required(VERSION 3.10)

# set the project name and languages
project(Pong LANGUAGES C)

# specify the C standard
set (CMAKE_C_STANDARD 99)
set (CMAKE_C_STANDARD_REQUIRED ON)
set (CMAKE_C_EXTENSIONS OFF)

# set the build type
if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE Debug)
endif()

# find RayLib
find_package(raylib 2.5.0 REQUIRED)

# find all the source files
FILE(GLOB SOURCES "src/*.c")

# create the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# link with raylib
target_link_libraries(${PROJECT_NAME} raylib m)

# Resources
# Copy all of the resource files to the destination
file(COPY "assets/"  DESTINATION "assets/")
