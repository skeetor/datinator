# CMake Datinator sources
# (C) by Gerhard W. Gruber in Germany 2015
#
# PLUGIN library
#

if (STATIC_BUILD)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_PLUGIN_STATIC -DBUILD_PLUGINS_AS_STATIC")
else (NOT STATIC_BUILD)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_PLUGIN_DLL -DBUILD_PLUGINS_AS_DLL")
endif()

if( "${UNICODE_FLAG}${STATIC_FLAG}${BUILD_TYPE_FLAG}" STREQUAL "" )
	set(PLUGIN_NAME_TAG "")
else()
	set( PLUGIN_NAME_TAG "_${UNICODE_FLAG}${STATIC_FLAG}${BUILD_TYPE_FLAG}" )
endif()

set(PLUGIN_SOURCE_DIR "${CMAKE_SOURCE_DIR}/plugins/csv")
set(PLUGIN_BINARY_DIR "${CMAKE_BINARY_DIR}/plugins/csv")
set(PLUGIN_LIBRARY "plugin${PLUGIN_NAME_TAG}")
set(PLUGIN_LIBRARY "${PLUGIN_LIBRARY}" PARENT_SCOPE)
set(PLUGIN_GEN_INCLUDE "${PLUGIN_BINARY_DIR}")

include_directories(${SUPPORT_INCLUDE_PATH}/include)
include_directories(${SOCI_LIBRARY_PATH}/core)
include_directories(${PLUGIN_GEN_INCLUDE}/..)

set(GENERATED_PLUGIN_SOURCES "")
set(PLUGIN_SOURCES ${PLUGIN_SOURCES}
	${CMAKE_CURRENT_SOURCE_DIR}/dll_main.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/plugin.h
	${CMAKE_CURRENT_SOURCE_DIR}/plugin_dll_api.h
)

add_subdirectory (container)
add_subdirectory (gui)
add_subdirectory (sql)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)

include_directories(${Qt5Widgets_INCLUDES})
include_directories(${CMAKE_BINARY_DIR})			# required for generated UI headers
include_directories(${CMAKE_SOURCE_DIR})

generate_qt_sources(${PLUGIN_SOURCE_DIR} ${PLUGIN_BINARY_DIR} ${PLUGIN_GEN_INCLUDE} GENERATED_PLUGIN_SOURCES PLUGIN_SOURCES)

# The plugin library acts as the interface between the main program and the plugins
# and must always be linked to the main program.
set(STATIC_BUILD_LIBRARIES ${STATIC_BUILD_LIBRARIES} ${PLUGIN_LIBRARY} PARENT_SCOPE)

add_library(${PLUGIN_LIBRARY} ${BUILD_TYPE} ${PLUGIN_SOURCES} ${GENERATED_PLUGIN_SOURCES})
target_link_libraries(${PLUGIN_LIBRARY} ${SUPPORT_LIBRARY_QT} ${SUPPORT_LIBRARY} ${SUPPORT_LIBRARY_QT} ${SOCI_LIBRARY} Qt5::Core Qt5::Widgets Qt5::Gui )

set_target_properties(${PLUGIN_LIBRARY} PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
                      ARCHIVE_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
                      LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
)
