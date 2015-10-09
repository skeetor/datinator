# CMake main file for support library
# (C) by Gerhard W. Gruber in Germany 2015
#
# This library contains the helper functions which don't
# have any dependencies on other library and thus can
# stand on it's own.
#

set(TARGET_LIBRARY support${PLUGIN_NAME_TAG})
set(DEBUG_EXPORT_SYMBOLS OFF CACHE BOOL "Set debug mode for exported symbols")
set(UI_INCLUDE_DIR "${SUPPORT_LIB_BINARY_DIR}/include")
#message("Create directory ${UI_INCLUDE_DIR}")
file(MAKE_DIRECTORY "${UI_INCLUDE_DIR}/support")

configure_file(config.h.in ${UI_INCLUDE_DIR}/support/config.h)

include_directories("..")
include_directories(${CMAKE_BINARY_DIR})
include_directories(${UI_INCLUDE_DIR})

set(SUPPORT_SOURCE
	${CMAKE_CURRENT_SOURCE_DIR}/config.h
	${CMAKE_CURRENT_SOURCE_DIR}/support_defs.h
	${CMAKE_CURRENT_SOURCE_DIR}/support_dll_api.h
	${CMAKE_CURRENT_SOURCE_DIR}/supportlib_namespace.h
	${CMAKE_CURRENT_SOURCE_DIR}/dll_main.cpp
)

set (UNICODE_POSTFIX "")

if (STATIC_BUILD)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_SUPPORT_STATIC")
else (NOT STATIC_BUILD)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_SUPPORT_DLL")
endif()

if (UNICODE_BUILD)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUNICODE")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pedantic -Wextra")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

add_subdirectory (helper)
add_subdirectory (db)
add_subdirectory (unicode)
add_subdirectory (patterns)
add_subdirectory (images)

add_library(${TARGET_LIBRARY} ${BUILD_TYPE} ${SUPPORT_SOURCE} )

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
