# CMake Datinator sources
# (C) by Gerhard W. Gruber in Germany 2015
#
# CSV plugin

if(BUILD_CSV_PLUGIN)
    message("-- CSV plugin enabled")
else()
    message("-- CSV plugin disabled")
    return()
endif()

set(CSV_SOURCE_DIR "${CMAKE_SOURCE_DIR}/plugins/csv")
set(CSV_BINARY_DIR "${CMAKE_BINARY_DIR}/plugins/csv")
set(CSV_PLUGIN csv_${UNICODE_FLAG}${STATIC_FLAG}${BUILD_TYPE_FLAG})
set(CSV_GEN_INCLUDE "${CSV_BINARY_DIR}")

if (STATIC_BUILD)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_CSV_STATIC")
	list(APPEND STATIC_LIBRARIES -Wl,--whole-archive ${CSV_PLUGIN} -Wl,--no-whole-archive )
else (NOT STATIC_BUILD)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_CSV_DLL")
endif()

include_directories(${CSV_GEN_INCLUDE}/..)		# generated headers for QT

set(CSV_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/dll_main.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/csv_dll_api.h
	${CMAKE_CURRENT_SOURCE_DIR}/csv_global.h
	${CMAKE_CURRENT_SOURCE_DIR}/csv_container.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/csv_container.h
	${CMAKE_CURRENT_SOURCE_DIR}/csv_reader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/csv_reader.h
	${CMAKE_CURRENT_SOURCE_DIR}/csv_writer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/csv_writer.h
)

add_subdirectory (gui)

set(GENERATED_CSV_SOURCES "")
generate_qt_sources(${CSV_SOURCE_DIR} ${CSV_BINARY_DIR} ${CSV_GEN_INCLUDE} GENERATED_CSV_SOURCES CSV_SOURCES)

set(STATIC_LIBRARIES "${STATIC_LIBRARIES}" PARENT_SCOPE)
add_library(${CSV_PLUGIN} ${BUILD_TYPE} ${CSV_SOURCES} ${GENERATED_CSV_SOURCES})
target_link_libraries(${CSV_PLUGIN} ${PLUGIN_LIBRARY} ${SUPPORT_LIBRARY_QT} ${SUPPORT_LIBRARY} Qt5::Core Qt5::Widgets Qt5::Gui )

set_target_properties(${CSV_PLUGIN} PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
                      ARCHIVE_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
                      LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
)
