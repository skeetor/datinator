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

# compare dereferenced strings with STREQUAL only if unquoted warning (QT package needs an update).
cmake_policy(SET CMP0054 OLD)

project(Datinator)

set(SUPPORT_BIN_DIR "lib")
IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_WINDOWS")
	set(SUPPORT_BIN_DIR "bin")
ENDIF()
IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_LINUX")
ENDIF()

set (EXECUTABLE_BINARY_PATH "${CMAKE_BINARY_DIR}/../${SUPPORT_BIN_DIR}" CACHE PATH "Path where the executable is built to.")
set (SUPPORT_LIBRARY_BIN_PATH "${CMAKE_BINARY_DIR}/../${SUPPORT_BIN_DIR}" CACHE PATH "Path where the support library binaries are located (default is assumed on the same directory as Datinator is).")
set (SUPPORT_LIBRARY_BASE_PATH "${CMAKE_SOURCE_DIR}/../support_lib" CACHE PATH "Path where the support library sources are located (default is assumed on the same directory as Datinator is).")
set (SUPPORT_INCLUDE_PATH "${SUPPORT_LIBRARY_BASE_PATH}" CACHE PATH "Path where the support library includes are located (default is assumed on the same directory as Datinator is).")

option (STATIC_BUILD "Build as a static version (default is shared)" OFF)
option (UNICODE_BUILD "Build for UNICODE (default is off)" OFF)

option (BUILD_ORACLE_PLUGIN "Build Oracle plugin" OFF)
option (BUILD_SQLITE3_PLUGIN "Build SQLite3 plugin" ON)
option (BUILD_CSV_PLUGIN "Build CSV plugin" ON)

include(${SUPPORT_LIBRARY_BASE_PATH}/cmake/generate_qt_sources.cmake)

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
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_SUPPORT_STATIC -DBUILD_SUPPORT_QT_STATIC -DBUILD_PLUGINS_STATIC -DBUILD_PLUGIN_AS_STATIC")
else (NOT STATIC_BUILD)
	set (STATIC_FLAG "")		# shared build
	set(BUILD_TYPE "SHARED")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_SUPPORT_DLL -DBUILD_SUPPORT_QT_DLL -DBUILD_PLUGINS_DLL -DBUILD_PLUGIN_DLL")
endif()

if (UNICODE_BUILD)
	set (UNICODE_FLAG "u")		# UNICODE build
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUNICODE")
else (NOT UNICODE_BUILD)
	set (UNICODE_FLAG "")		# ANSI build
endif()

set(BUILD_NAME_TAG "${BUILD_TYPE_FLAG}${STATIC_FLAG}${UNICODE_FLAG}")
string(LENGTH "${BUILD_NAME_TAG}" build_len)
if(build_len GREATER 0)
	set(BUILD_NAME_TAG "_${BUILD_NAME_TAG}")
endif()

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${SUPPORT_LIBRARY_BASE_PATH})
set(SUPPORT_LIB_NAME support${BUILD_NAME_TAG})
set(SUPPORT_QT_LIB_NAME support_qt${BUILD_NAME_TAG})

if(NOT ARCH)
    set(ARCH i386)
endif()

# Now the ARCH variable will be in lowercase.
# It is needed because STREQUAL comparison
# is case-sensitive.
# See http://cmake.3232098.n2.nabble.com/Case-insensitive-string-compare-td7580269.html
# for more information.
string(TOLOWER ${ARCH} ARCH)

find_library(SUPPORT_LIBRARY
	NAMES "${SUPPORT_LIB_NAME}"
	HINTS "${SUPPORT_LIBRARY_BIN_PATH}"
)
find_library(SUPPORT_LIBRARY_QT
	NAMES "${SUPPORT_QT_LIB_NAME}"
	HINTS "${SUPPORT_LIBRARY_BIN_PATH}"
)

# External packages
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)

if (UNICODE_BUILD)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUNICODE")
endif()

# Include paths

# Enable the build directory as an include path as well, required for
# qt moc generated files.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_definitions(${Qt5Widgets_DEFINITIONS})

include_directories(${Qt5Widgets_INCLUDES})
include_directories(${CMAKE_BINARY_DIR}/src)			# required for generated UI headers
include_directories(${CMAKE_BINARY_DIR}/../include)		# required for generated UI headers
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/plugins)

include_directories(${SUPPORT_INCLUDE_PATH})
include_directories(${SUPPORT_INCLUDE_PATH}/include)

# will be filled by subsequent CMakeLists
list(APPEND STATIC_LIBRARIES "")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pedantic -Wextra")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

add_subdirectory (src)
add_subdirectory (plugins)

generate_qt_sources(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${CMAKE_BINARY_DIR} GENERATED_SOURCES SOURCE_FILES)

add_executable(datinator ${SOURCE_FILES} ${GENERATED_SOURCES})
target_link_libraries(datinator ${STATIC_LIBRARIES} ${SUPPORT_LIBRARY_QT} ${SUPPORT_LIBRARY} Qt5::Widgets Qt5::Gui Qt5::Core )

set_target_properties(datinator PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_BINARY_PATH}
                      ARCHIVE_OUTPUT_DIRECTORY ${EXECUTABLE_BINARY_PATH}
                      LIBRARY_OUTPUT_DIRECTORY ${EXECUTABLE_BINARY_PATH}
)
