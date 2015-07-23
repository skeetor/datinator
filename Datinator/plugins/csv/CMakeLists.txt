# CMake Datinator sources
# (C) by Gerhard W. Gruber in Germany 2015
#
# CSV plugin

set(CSV_SOURCE_DIR "${CMAKE_SOURCE_DIR}/plugins/csv")
set(CSV_BINARY_DIR "${CMAKE_BINARY_DIR}/plugins/csv")
set(CSV_LIBRARY csv_${UNICODE_FLAG}${STATIC_FLAG}${BUILD_TYPE_FLAG})
set(CSV_GEN_INCLUDE "${CSV_BINARY_DIR}")

if (STATIC_BUILD)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_CSV_STATIC")
	set(STATIC_BUILD_LIBRARIES ${STATIC_BUILD_LIBRARIES} ${CSV_LIBRARY} PARENT_SCOPE)
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

add_library(${CSV_LIBRARY} ${BUILD_TYPE} ${CSV_SOURCES} ${GENERATED_CSV_SOURCES})
target_link_libraries(${CSV_LIBRARY} ${PLUGIN_LIBRARY} ${SUPPORT_LIBRARY_QT} ${SUPPORT_LIBRARY} ${SUPPORT_LIBRARY_QT} Qt5::Core Qt5::Widgets Qt5::Gui )

set_target_properties(${CSV_LIBRARY} PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
                      ARCHIVE_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
                      LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
)
