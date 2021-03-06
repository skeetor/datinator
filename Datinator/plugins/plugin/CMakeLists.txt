# CMake Datinator sources
# (C) by Gerhard W. Gruber in Germany 2015
#
# PLUGIN library
#

if (STATIC_BUILD)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_PLUGIN_STATIC -DBUILD_PLUGINS_STATIC")
else (NOT STATIC_BUILD)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_PLUGIN_DLL -DBUILD_PLUGINS_DLL")
endif()

set(PLUGIN_SOURCE_DIR "${CMAKE_SOURCE_DIR}/plugins/plugin")
set(PLUGIN_BINARY_DIR "${CMAKE_BINARY_DIR}/plugins/plugin")
set(PLUGIN_LIBRARY "plugin${PLUGIN_NAME_TAG}")
set(PLUGIN_LIBRARY "${PLUGIN_LIBRARY}" PARENT_SCOPE)
set(PLUGIN_GEN_INCLUDE "${PLUGIN_BINARY_DIR}")

include_directories(${SUPPORT_INCLUDE_PATH}/include)
include_directories(${SOCI_LIBRARY_BASE_PATH}/core)
include_directories(${PLUGIN_GEN_INCLUDE}/..)

set(GENERATED_PLUGIN_SOURCES "")
set(PLUGIN_SOURCES ${PLUGIN_SOURCES}
	${CMAKE_CURRENT_SOURCE_DIR}/dll_main.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/plugin.h
	${CMAKE_CURRENT_SOURCE_DIR}/plugin_dll_api.h

	${CMAKE_CURRENT_SOURCE_DIR}/plugin_info.h
	${CMAKE_CURRENT_SOURCE_DIR}/plugin_info.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/plugin_manager.h
	${CMAKE_CURRENT_SOURCE_DIR}/plugin_manager.cpp
)

add_subdirectory (container)
add_subdirectory (gui)
add_subdirectory (sql)

#find_package(Qt5Widgets REQUIRED)
#find_package(Qt5Core REQUIRED)
#find_package(Qt5Gui REQUIRED)

include_directories(${Qt5Widgets_INCLUDES})
include_directories(${CMAKE_BINARY_DIR})			# required for generated UI headers
include_directories(${CMAKE_SOURCE_DIR})

generate_qt_sources(${PLUGIN_SOURCE_DIR} ${PLUGIN_BINARY_DIR} ${PLUGIN_GEN_INCLUDE} GENERATED_PLUGIN_SOURCES PLUGIN_SOURCES)

list(INSERT STATIC_LIBRARIES 0 "${PLUGIN_LIBRARY}")
set(STATIC_LIBRARIES "${STATIC_LIBRARIES}" PARENT_SCOPE)

add_library(${PLUGIN_LIBRARY} ${BUILD_TYPE} ${PLUGIN_SOURCES} ${GENERATED_PLUGIN_SOURCES})
target_link_libraries(${PLUGIN_LIBRARY} ${SUPPORT_LIBRARY_QT} ${SUPPORT_LIBRARY} ${SOCI_CORE_LIBRARY} Qt5::Core Qt5::Widgets Qt5::Gui )

set_target_properties(${PLUGIN_LIBRARY} PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
                      ARCHIVE_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
                      LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
)
