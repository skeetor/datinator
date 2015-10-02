# CMake main file for support libraries
# (C) by Gerhard W. Gruber in Germany 2015
#
# This main file will create both libraries.
# The first one is the basic support library
# without any external dependencies. It contains
# generic helper modules for various things that
# are not neccessarily related to each other, but
# also can't be put into some better named library
# for lack of a sufficient bigger context.
#
# The second library is similar, only it contains
# code that interfaces with Qt. So these helper have
# an external to dependency to it.
#

# Options:
# UNICODE_BUILD=[0|1]						default is 0 (ansi)
# STATIC_BUILD=[0|1]						default is 0 (shared)
# LIBRARY_OUTPUT_PATH=<library path>		This is the path where the built library will be put into.
#											default is <source>/lib

# To build the version use:
#
# cmake -DCMAKE_BUILD_TYPE=Release -G "<build type>" <Path to source>
#
# Example:
# cmake -DCMAKE_BUILD_TYPE=Release -G "MinGW Makefiles" ..
# cmake -DCMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles" ..
# cmake -DCMAKE_BUILD_TYPE=Release -DUNICODE_BUILD=1 -G "MinGW Makefiles" ..
# cmake -DCMAKE_BUILD_TYPE=Debug -DUNICODE_BUILD=1 -G "MinGW Makefiles" ..
# ...
#

cmake_minimum_required(VERSION 3.0.2)

project(support_lib)

set(SUPPORT_BIN_DIR "lib")        # default

IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_WINDOWS")
    set(SUPPORT_BIN_DIR "bin")
ENDIF()
IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_LINUX")
ENDIF()

set(SUPPORT_LIB_BINARY_DIR "${CMAKE_BINARY_DIR}/.." )
set (LIBRARY_OUTPUT_PATH "${SUPPORT_LIB_BINARY_DIR}/${SUPPORT_BIN_DIR}" CACHE PATH "Path where the libraries should be put")

file(MAKE_DIRECTORY "${SUPPORT_LIB_BINARY_DIR}/include")
file(MAKE_DIRECTORY "${SUPPORT_LIB_BINARY_DIR}/${SUPPORT_BIN_DIR}")

option (STATIC_BUILD "Build as a static version (default is shared)" OFF)
option (UNICODE_BUILD "Build for UNICODE (default is off)" OFF)

if(NOT LIBRARY_OUTPUT_PATH)
	set (LIBRARY_OUTPUT_PATH "${SUPPORT_LIB_BINARY_DIR}/${SUPPORT_BIN_DIR}" CACHE PATH)
endif()

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")

if (CMAKE_BUILD_TYPE STREQUAL Release)
	set (BUILD_TYPE_FLAG "")		# release build
else (NOT CMAKE_BUILD_TYPE STREQUAL Release)
	set (BUILD_TYPE_FLAG "d")		# debug build
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_DEBUG")
endif()

if (STATIC_BUILD)
	set (STATIC_FLAG "s")		# static build
	set(BUILD_TYPE "STATIC")
else (NOT STATIC_BUILD)
	set (STATIC_FLAG "")		# shared build
	set(BUILD_TYPE "SHARED")
endif()

if (UNICODE_BUILD)
	set (UNICODE_FLAG "u")		# UNICODE build
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUNICODE")
else (NOT UNICODE_BUILD)
	set (UNICODE_FLAG "a")		# ANSI build
endif()

if(NOT ARCH)
    set(ARCH i386)
endif()
# Now the ARCH variable will be in lowercase.
# It is needed because STREQUAL comparison
# is case-sensitive.
# See http://cmake.3232098.n2.nabble.com/Case-insensitive-string-compare-td7580269.html
# for more information.
string(TOLOWER ${ARCH} ARCH)

add_subdirectory (support)
add_subdirectory (support_qt)
