# CMake Datinator sources
# (C) by Gerhard W. Gruber in Germany 2015
#

if (STATIC_BUILD)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_PLUGIN_STATIC -DBUILD_PLUGINS_AS_STATIC")
else (NOT STATIC_BUILD)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_PLUGIN_DLL -DBUILD_PLUGINS_AS_DLL")
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG" PARENT_SCOPE)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3" PARENT_SCOPE)

set(PLUGIN_GEN_INCLUDE "${CMAKE_SOURCE_DIR}/../include/datinator")
set(PLUGIN_LIBRARY plugin_${UNICODE_FLAG}${STATIC_FLAG}${BUILD_TYPE_FLAG} PARENT_SCOPE)
set(GENERATED_PLUGIN_SOURCES "")

include_directories(${SUPPORT_INCLUDE_PATH}/include)
include_directories(${PLUGIN_GEN_INCLUDE}/plugins)
include_directories(${SOCI_LIBRARY_PATH}/core)

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

generate_qt_sources(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${PLUGIN_GEN_INCLUDE} GENERATED_PLUGIN_SOURCES PLUGIN_SOURCES)

add_library(${PLUGIN_LIBRARY} ${BUILD_TYPE} ${PLUGIN_SOURCES} ${GENERATED_PLUGIN_SOURCES})
target_link_libraries(${PLUGIN_LIBRARY} ${SUPPORT_LIBRARY_PATH} ${SUPPORT_LIBRARY_QT_PATH} ${SOCI_LIBRARY} Qt5::Core Qt5::Widgets Qt5::Gui )

set_target_properties(${PLUGIN_LIBRARY} PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
                      ARCHIVE_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
                      LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
)
