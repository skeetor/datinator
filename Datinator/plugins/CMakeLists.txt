# CMake source files
# (C) by Gerhard W. Gruber in Germany 2015
#
# PLUGINS common
#

set (SOCI_LIBRARY_BASE_PATH "d:/opt/soci" CACHE PATH "Path where the soci library is located.")
set (SUPPORT_LIBRARY_BASE_PATH "${CMAKE_SOURCE_DIR}/../support_lib" CACHE PATH "Path where the support libraries is located (default is assumed on the same directory as Datinator is).")
set (LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}" CACHE PATH "Path where the libraries should be put")

if (STATIC_BUILD)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_PLUGINS_AS_STATIC")
else (NOT STATIC_BUILD)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_PLUGINS_AS_DLL")
endif()

include(${SUPPORT_LIBRARY_BASE_PATH}/cmake/generate_qt_sources.cmake)

include_directories(${SOCI_LIBRARY_BASE_PATH}/core)
include_directories(${SOCI_LIBRARY_BASE_PATH}/backends)

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${SOCI_LIBRARY_BASE_PATH})
find_library(SOCI_CORE_LIBRARY
    NAMES "soci-3.2.2_core_s${BUILD_TYPE_FLAG}"
    HINTS "${SOCI_LIBRARY_BASE_PATH}"
)

if (STATIC_BUILD)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_PLUGINS_AS_STATIC -DBUILD_PLUGIN_STATIC -DBUILD_SUPPORT_STATIC -DBUILD_SUPPORT_QT_STATIC")
endif()

set(COMMON_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/plugins_dll_api.h
	${CMAKE_CURRENT_SOURCE_DIR}/datinator_types.h
	${CMAKE_CURRENT_SOURCE_DIR}/idata_container.h
	${CMAKE_CURRENT_SOURCE_DIR}/idata_container_reader.h
	${CMAKE_CURRENT_SOURCE_DIR}/idata_container_writer.h
)

#set(PLUGIN_SOURCES ${COMMON_SOURCES})
add_subdirectory (plugin)
add_subdirectory (csv)
add_subdirectory (sqlite)

set(SOURCE_FILES ${SOURCE_FILES} ${COMMON_SOURCES}

	PARENT_SCOPE
)

set(PLUGIN_LIBRARY "${PLUGIN_LIBRARY}" PARENT_SCOPE)
set(STATIC_LIBRARIES "${STATIC_LIBRARIES}" PARENT_SCOPE)
