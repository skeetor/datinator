# CMake main file for support library
# (C) by Gerhard W. Gruber in Germany 2015
#
# This library contains QT helper functions.
#

include(${CMAKE_SOURCE_DIR}/cmake/generate_qt_sources.cmake)

set(TARGET_LIBRARY support_qt_${UNICODE_FLAG}${STATIC_FLAG}${BUILD_TYPE_FLAG})
set(SUPPORT_LIBRARY support_${UNICODE_FLAG}${STATIC_FLAG}${BUILD_TYPE_FLAG})
set(UI_INCLUDE_DIR "${SUPPORT_LIB_BINARY_DIR}/include")
#message("Create directory ${UI_INCLUDE_DIR}")
make_directory("${UI_INCLUDE_DIR}/support_qt")

# Enable the build directory as an include path as well, required for
# qt moc generated files.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)


include_directories(${Qt5Widgets_INCLUDES})
include_directories(${CMAKE_BINARY_DIR})			# required for generated UI headers
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${UI_INCLUDE_DIR})

set(SUPPORT_SOURCE
	${CMAKE_CURRENT_SOURCE_DIR}/support_qt_dll_api.h
	${CMAKE_CURRENT_SOURCE_DIR}/supportlib_qt_namespace.h
	${CMAKE_CURRENT_SOURCE_DIR}/dll_main.cpp
)

if (STATIC_BUILD)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_SUPPORT_STATIC -DBUILD_SUPPORT_QT_STATIC")
else (NOT STATIC_BUILD)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_SUPPORT_QT_DLL")
endif()

if (UNICODE_BUILD)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUNICODE")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pedantic -Wextra")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

add_subdirectory (db)
add_subdirectory (file)
add_subdirectory (helper)
add_subdirectory (logging)
add_subdirectory (models)
add_subdirectory (window)

generate_qt_sources(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${UI_INCLUDE_DIR} GENERATED_SOURCES SUPPORT_SOURCE)

add_library(${TARGET_LIBRARY} ${BUILD_TYPE} ${SUPPORT_SOURCE} ${GENERATED_SOURCES})
target_link_libraries(${TARGET_LIBRARY} ${SUPPORT_LIBRARY} Qt5::Widgets)


# The shared library versions need the option to export symbols
#target_compile_definitions(support_${UNICODE_FLAG}${STATIC_FLAG} PRIVATE "-DBUILD_SUPPORT_DLL" )
#target_compile_definitions(support_${UNICODE_FLAG}s PRIVATE "-DBUILD_SUPPORT_STATIC" )

# For specific target directories per debug/release build
# this can be used.
#
# <type>_OUTPUT_LIBRARY_DEBUG
# <type>_OUTPUT_LIBRARY_RELEASE

set_target_properties(${TARGET_LIBRARY} PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
                      ARCHIVE_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
                      LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
)
