	# CMake source files
# (C) by Gerhard W. Gruber in Germany 2015
#
# We list the include files here for the purpose
# of them being added to the workspace when using Visual Studio

set (SUPPORT_LIBRARY_BASE_PATH "${CMAKE_SOURCE_DIR}/../support_lib" CACHE PATH "Path where the support libraries is located (default is assumed on the same directory as Datinator is).")
set (LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}" CACHE PATH "Path where the libraries should be put")

if(NOT ARCH)
    set(ARCH i386)
endif()

string(TOLOWER ${ARCH} ARCH)

if (CMAKE_BUILD_TYPE STREQUAL Release)
	set (BUILD_TYPE_FLAG "")		# release build
else (NOT CMAKE_BUILD_TYPE STREQUAL Release)
	set (BUILD_TYPE_FLAG "d")		# debug build
endif()

if (UNICODE_BUILD)
	set (UNICODE_FLAG "u")		# UNICODE build
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUNICODE")
else (NOT UNICODE_BUILD)
	set (UNICODE_FLAG "a")		# ANSI build
endif()

if (STATIC_BUILD)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_PLUGINS_AS_STATIC")
	set (STATIC_FLAG "s")		# static build
	set(BUILD_TYPE "STATIC")
else (NOT STATIC_BUILD)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_PLUGINS_AS_DLL")
	set (STATIC_FLAG "")		# shared build
	set(BUILD_TYPE "SHARED")
endif()

include(${SUPPORT_LIBRARY_BASE_PATH}/cmake/generate_qt_sources.cmake)

set(PLUGIN_SOURCES 
	${CMAKE_CURRENT_SOURCE_DIR}/datinator_types.h
	${CMAKE_CURRENT_SOURCE_DIR}/idata_container.h
	${CMAKE_CURRENT_SOURCE_DIR}/idata_container_reader.h
	${CMAKE_CURRENT_SOURCE_DIR}/idata_container_writer.h
)

add_subdirectory (plugin)

set(SOURCE_FILES ${SOURCE_FILES} ${PLUGIN_SOURCES}
	${CMAKE_CURRENT_SOURCE_DIR}/plugin_info.h
	${CMAKE_CURRENT_SOURCE_DIR}/plugin_info.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/plugin_manager.h
	${CMAKE_CURRENT_SOURCE_DIR}/plugin_manager.cpp

	PARENT_SCOPE
)
