# CMake Datinator sources
# (C) by Gerhard W. Gruber in Germany 2015
#

set(PLUGIN_SOURCES ${PLUGIN_SOURCES}
	${CMAKE_CURRENT_SOURCE_DIR}/soci_container.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/soci_container.h
	${CMAKE_CURRENT_SOURCE_DIR}/sql_container.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/sql_container.h

	PARENT_SCOPE
)
