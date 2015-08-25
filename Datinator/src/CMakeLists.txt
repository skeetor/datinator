# CMake Datinator sources
# (C) by Gerhard W. Gruber in Germany 2015
#

add_subdirectory (gui)
add_subdirectory (manipulator)

set(SOURCE_FILES ${SOURCE_FILES}
	${CMAKE_CURRENT_SOURCE_DIR}/application.h
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/resource.rc

	PARENT_SCOPE
)
