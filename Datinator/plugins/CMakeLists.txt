# CMake source files
# (C) by Gerhard W. Gruber in Germany 2015
#
# PLUGINS common
#

set (SUPPORT_LIBRARY_BASE_PATH "${CMAKE_SOURCE_DIR}/../support_lib" CACHE PATH "Path where the support libraries is located (default is assumed on the same directory as Datinator is).")
set (LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}" CACHE PATH "Path where the libraries should be put")

if (STATIC_BUILD)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_PLUGINS_AS_STATIC")
else (NOT STATIC_BUILD)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_PLUGINS_AS_DLL")
endif()

include(${SUPPORT_LIBRARY_BASE_PATH}/cmake/generate_qt_sources.cmake)

set(COMMON_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/plugins_dll_api.h
	${CMAKE_CURRENT_SOURCE_DIR}/datinator_types.h
	${CMAKE_CURRENT_SOURCE_DIR}/idata_container.h
	${CMAKE_CURRENT_SOURCE_DIR}/idata_container_reader.h
	${CMAKE_CURRENT_SOURCE_DIR}/idata_container_writer.h
)

set(PLUGIN_SOURCES ${COMMON_SOURCES})
add_subdirectory (plugin)
add_subdirectory (csv)

set(SOURCE_FILES ${SOURCE_FILES} ${COMMON_SOURCES}
	${CMAKE_CURRENT_SOURCE_DIR}/plugin_info.h
	${CMAKE_CURRENT_SOURCE_DIR}/plugin_info.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/plugin_manager.h
	${CMAKE_CURRENT_SOURCE_DIR}/plugin_manager.cpp

	PARENT_SCOPE
)

set(PLUGIN_LIBRARY "${PLUGIN_LIBRARY}" PARENT_SCOPE)
set(STATIC_BUILD_LIBRARIES "${STATIC_BUILD_LIBRARIES}" PARENT_SCOPE)
