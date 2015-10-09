# CMake Datinator sources
# (C) by Gerhard W. Gruber in Germany 2015
#
# ORACLE plugin

if(BUILD_ORACLE_PLUGIN)
    message("-- Oracle plugin enabled")
else()
    message("-- Oracle plugin disabled")
    return()
endif()

set (ORACLE_LIBRARY_BASE_PATH "D:/opt/oracle" CACHE PATH "Path where the SQLite3 library is located")

set(ORACLE_SOURCE_DIR "${CMAKE_SOURCE_DIR}/plugins/oracle")
set(ORACLE_BINARY_DIR "${CMAKE_BINARY_DIR}/plugins/oracle")
set(ORACLE_PLUGIN oracle${PLUGIN_NAME_TAG})
set(ORACLE_GEN_INCLUDE "${ORACLE_BINARY_DIR}")

include_directories(${ORACLE_LIBRARY_BASE_PATH}/include)
include_directories(${ORACLE_GEN_INCLUDE}/..)		# generated headers for QT

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)

find_library(SOCI_ORACLE_LIBRARY
    NAMES "soci-3.2.2_oracle_s${BUILD_TYPE_FLAG}"
    HINTS "${SOCI_LIBRARY_BASE_PATH}"
)

find_library(SOCI_CORE_LIBRARY
    NAMES "soci-3.2.2_core_s${BUILD_TYPE_FLAG}"
    HINTS "${SOCI_LIBRARY_BASE_PATH}"
)

find_library(ORACLE_LIBRARY
    NAMES "oci"
    HINTS "${ORACLE_LIBRARY_BASE_PATH}/lib/msvc"
)

if (STATIC_BUILD)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_ORACLE_STATIC")
	list(APPEND STATIC_LIBRARIES -Wl,--whole-archive ${ORACLE_PLUGIN} -Wl,--no-whole-archive ${SOCI_ORACLE_LIBRARY} ${SOCI_CORE_LIBRARY} ${ORACLE_LIBRARY})
else (NOT STATIC_BUILD)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_ORACLE_DLL")
endif()


set(ORACLE_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/dll_main.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/oracle_dll_api.h
	${CMAKE_CURRENT_SOURCE_DIR}/oracle_global.h
	${CMAKE_CURRENT_SOURCE_DIR}/oracle_container.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/oracle_container.h
	${CMAKE_CURRENT_SOURCE_DIR}/oracle_reader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/oracle_reader.h
	${CMAKE_CURRENT_SOURCE_DIR}/oracle_writer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/oracle_writer.h
)

add_subdirectory (gui)

set(GENERATED_ORACLE_SOURCES "")
generate_qt_sources(${ORACLE_SOURCE_DIR} ${ORACLE_BINARY_DIR} ${ORACLE_GEN_INCLUDE} GENERATED_ORACLE_SOURCES ORACLE_SOURCES)

set(STATIC_LIBRARIES "${STATIC_LIBRARIES}" PARENT_SCOPE)

add_library(${ORACLE_PLUGIN} ${BUILD_TYPE} ${ORACLE_SOURCES} ${GENERATED_ORACLE_SOURCES})
target_link_libraries(${ORACLE_PLUGIN} ${PLUGIN_LIBRARY} ${SUPPORT_LIBRARY_QT} ${SUPPORT_LIBRARY} ${SOCI_ORACLE_LIBRARY} ${SOCI_CORE_LIBRARY} ${ORACLE_LIBRARY} Qt5::Core Qt5::Widgets Qt5::Gui )

set_target_properties(${ORACLE_PLUGIN} PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
                      ARCHIVE_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
                      LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
)
