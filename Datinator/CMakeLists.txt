# CMake main file for Datinator
# (C) by Gerhard W. Gruber in Germany 2015
#

# Options:
# UNICODE_BUILD=[0|1]						default is 0 (ansi)
# STATIC_BUILD=[0|1]						default is 0 (shared)

# To build the version use:
#
# cmake -DCMAKE_BUILD_TYPE=Release -G "<build type>" <Path to source>
#
# Example:
# cmake -DCMAKE_BUILD_TYPE=Release -G "MinGW Makefiles" ...
# cmake -DCMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles" ...
# cmake -DCMAKE_BUILD_TYPE=Release -DUNICODE_BUILD=1 -G "MinGW Makefiles" ...
# cmake -DCMAKE_BUILD_TYPE=Debug -DUNICODE_BUILD=1 -G "MinGW Makefiles" ...
# ...
#

cmake_minimum_required(VERSION 3.0.2)

project(Datinator)

set (SUPPORT_LIBRARY_BASE_PATH "${CMAKE_SOURCE_DIR}/../support_lib" CACHE PATH "Path where the support libraries is located (default is assumed on the same directory as Datinator is).")
set (SUPPORT_INCLUDE_PATH "${SUPPORT_LIBRARY_BASE_PATH}" CACHE PATH "Path where the support library includes are located (default is assumed on the same directory as Datinator is).")
set (BINARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin" CACHE PATH "Path where the executableand libraries should go")
#set (SUPPORT_LIBRARY_PATH "${SUPPORT_LIBRARY_BASE_PATH}/lib" CACHE PATH "Path where the support libraries is located (default is assumed on the same directory as Datinator is).")

option (STATIC_BUILD "Build as a static version (default is shared)" OFF)
option (UNICODE_BUILD "Build for UNICODE (default is off)" OFF)

include(${SUPPORT_LIBRARY_BASE_PATH}/cmake/generate_qt_sources.cmake)

if (CMAKE_BUILD_TYPE STREQUAL Release)
	set (BUILD_TYPE_FLAG "")		# release build
else (NOT CMAKE_BUILD_TYPE STREQUAL Release)
	set (BUILD_TYPE_FLAG "d")		# debug build
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
	set (UNICODE_FLAG "")		# ANSI build
	set (UNICODE_LIB_FLAG "a")
endif()

set(BUILD_FLAGS "${BUILD_TYPE_FLAG}${STATIC_FLAG}${UNICODE_FLAG}")
string(LENGTH "${BUILD_FLAGS}" build_len)
if(build_len GREATER 0)
	set(BUILD_FLAGS "_${BUILD_FLAGS}")
endif()

set(SUPPORT_LIB_NAME support_${UNICODE_LIB_FLAG}${STATIC_FLAG}${BUILD_TYPE_FLAG})
set(SUPPORT_QT_LIB_NAME support_qt_${UNICODE_LIB_FLAG}${STATIC_FLAG}${BUILD_TYPE_FLAG})

if(NOT ARCH)
    set(ARCH i386)
endif()

# Now the ARCH variable will be in lowercase.
# It is needed because STREQUAL comparison
# is case-sensitive.
# See http://cmake.3232098.n2.nabble.com/Case-insensitive-string-compare-td7580269.html
# for more information.
string(TOLOWER ${ARCH} ARCH)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)

# Enable the build directory as an include path as well, required for
# qt moc generated files.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_definitions(${Qt5Widgets_DEFINITIONS})

include_directories(${Qt5Widgets_INCLUDES})
include_directories(${CMAKE_BINARY_DIR}/src)			# required for generated UI headers
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/plugins)

include_directories(${SUPPORT_INCLUDE_PATH})
include_directories(${SUPPORT_INCLUDE_PATH}/include)

if (STATIC_BUILD)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_PLUGINS_AS_STATIC -DBUILD_PLUGIN_STATIC -DBUILD_SUPPORT_STATIC -DBUILD_SUPPORT_QT_STATIC")
endif()

if (UNICODE_BUILD)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUNICODE")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pedantic -Wextra")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

set(PLUGIN_LIBRARY plugin_${UNICODE_FLAG}${STATIC_FLAG}${BUILD_TYPE_FLAG})

add_subdirectory (src)
add_subdirectory (plugins)

generate_qt_sources(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${CMAKE_BINARY_DIR} GENERATED_SOURCES SOURCE_FILES)

find_library(SUPPORT_LIBRARY_PATH "${SUPPORT_LIB_NAME}" PATHS ${SUPPORT_LIBRARY_BASE_PATH}/lib)
find_library(SUPPORT_LIBRARY_QT_PATH ${SUPPORT_QT_LIB_NAME} PATHS ${SUPPORT_LIBRARY_BASE_PATH}/lib)

add_executable(datinator ${SOURCE_FILES} ${GENERATED_SOURCES})
target_link_libraries(datinator ${SUPPORT_LIBRARY_QT_PATH} ${PLUGIN_LIBRARY} ${SUPPORT_LIBRARY_PATH} Qt5::Widgets Qt5::Gui Qt5::Core )
#target_link_libraries(datinator ${SUPPORT_LIBRARY_PATH} ${SUPPORT_LIBRARY_QT_PATH} Qt5::Widgets Qt5::Gui Qt5::Core )
