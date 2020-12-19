# This CMakeList.txt currently only supports compilation of
# a program on windows or linux, but only with the dependancies
# for oclPixelGameEngine.h

# NOTE: THIS CMAKELIST WILL NOT INSTALL DEPENDANCIES, IT WILL JUST FIND THEM AND
# COMPILE / LINK THEM RESPECTIVELY, YOU NEED TO INSTALL THEM YOURSELF

# Any problems? submit an issue, or contact the author, "Ben (plane000)#8618" on Discord

# Currently linked / compiled by default is:
# Threads (pthread), OpenGL, GLX, libPNG, X11


cmake_minimum_required(VERSION 3.7)
project(Retronew)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} CMakeFiles/)

set(CMAKE_CXX_STANDARD 17)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

set(IncludeDir common/include/) # Change include/ to your include directory
include_directories(${IncludeDir})

set(THREADS_PREFER_PTHREAD_FLAD ON)
find_package(Threads REQUIRED)

find_package(OpenGL REQUIRED)

if (UNIX)
    find_package(X11 REQUIRED)
    find_package(PNG REQUIRED)
    include_directories(${PNG_INCLUDE_DIR})
endif (UNIX)

if (WIN32)
    include_directories(${WinSDK})
endif (WIN32)


add_subdirectory(1pal)
add_subdirectory(2maze)
add_subdirectory(3move)
