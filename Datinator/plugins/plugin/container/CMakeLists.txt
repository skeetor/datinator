# CMake Datinator sources
# (C) by Gerhard W. Gruber in Germany 2015
#

add_subdirectory (file)
add_subdirectory (gui)
add_subdirectory (sql)

set(PLUGIN_SOURCES ${PLUGIN_SOURCES}
	${CMAKE_CURRENT_SOURCE_DIR}/container_base.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/container_base.h
	${CMAKE_CURRENT_SOURCE_DIR}/reader_base.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/reader_base.h
	${CMAKE_CURRENT_SOURCE_DIR}/writer_base.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/writer_base.h

	PARENT_SCOPE
)
