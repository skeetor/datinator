# CMake Datinator sources
# (C) by Gerhard W. Gruber in Germany 2015
#

add_subdirectory (sample_table)

set(PLUGIN_SOURCES ${PLUGIN_SOURCES}
	${CMAKE_CURRENT_SOURCE_DIR}/datatype_string.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/datatype_string.h
	${CMAKE_CURRENT_SOURCE_DIR}/progress.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/progress.h

	PARENT_SCOPE
)
