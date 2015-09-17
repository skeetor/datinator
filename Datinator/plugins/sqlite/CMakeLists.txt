# CMake Datinator sources
# (C) by Gerhard W. Gruber in Germany 2015
#
# SQLITE plugin

set (SQLITE3_LIBRARY_BASE_PATH "D:/opt/sqlite3" CACHE PATH "Path where the SQLite3 library is located")

set(SQLITE_SOURCE_DIR "${CMAKE_SOURCE_DIR}/plugins/sqlite")
set(SQLITE_BINARY_DIR "${CMAKE_BINARY_DIR}/plugins/sqlite")
set(SQLITE_PLUGIN sqlite_${UNICODE_FLAG}${STATIC_FLAG}${BUILD_TYPE_FLAG})
set(SQLITE_GEN_INCLUDE "${SQLITE_BINARY_DIR}")

include_directories(${SQLITE3_LIBRARY_BASE_PATH})
include_directories(${SQLITE_GEN_INCLUDE}/..)		# generated headers for QT

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)

find_library(SOCI_SQLITE_LIBRARY
    NAMES "soci-3.2.2_sqlite3_s${BUILD_TYPE_FLAG}"
    HINTS "${SOCI_LIBRARY_BASE_PATH}"
)

find_library(SOCI_CORE_LIBRARY
    NAMES "soci-3.2.2_core_s${BUILD_TYPE_FLAG}"
    HINTS "${SOCI_LIBRARY_BASE_PATH}"
)

if (STATIC_BUILD)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_SQLITE_STATIC")
	list(APPEND STATIC_LIBRARIES -Wl,--whole-archive ${SQLITE_PLUGIN} -Wl,--no-whole-archive ${SOCI_SQLITE_LIBRARY} ${SOCI_SQLITE3_LIBRARY} ${SOCI_CORE_LIBRARY} ${SQLITE3_LIBRARY})
else (NOT STATIC_BUILD)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_SQLITE_DLL")
endif()

set(SQLITE_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/dll_main.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/sqlite_dll_api.h
	${CMAKE_CURRENT_SOURCE_DIR}/sqlite_global.h
	${CMAKE_CURRENT_SOURCE_DIR}/sqlite_container.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/sqlite_container.h
	${CMAKE_CURRENT_SOURCE_DIR}/sqlite_reader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/sqlite_reader.h
	${CMAKE_CURRENT_SOURCE_DIR}/sqlite_writer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/sqlite_writer.h
)

add_subdirectory (gui)

set(GENERATED_SQLITE_SOURCES "")
generate_qt_sources(${SQLITE_SOURCE_DIR} ${SQLITE_BINARY_DIR} ${SQLITE_GEN_INCLUDE} GENERATED_SQLITE_SOURCES SQLITE_SOURCES)

set(STATIC_LIBRARIES "${STATIC_LIBRARIES}" PARENT_SCOPE)

add_library(${SQLITE_PLUGIN} ${BUILD_TYPE} ${SQLITE_SOURCES} ${GENERATED_SQLITE_SOURCES})
target_link_libraries(${SQLITE_PLUGIN} ${PLUGIN_LIBRARY} ${SUPPORT_LIBRARY_QT} ${SUPPORT_LIBRARY} ${SOCI_SQLITE3_LIBRARY} ${SOCI_CORE_LIBRARY} ${SQLITE3_LIBRARY} Qt5::Core Qt5::Widgets Qt5::Gui )

set_target_properties(${SQLITE_PLUGIN} PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
                      ARCHIVE_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
                      LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
)
