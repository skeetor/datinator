# CMake source files
# (C) by Gerhard W. Gruber in Germany 2015
#
# We list the include files here for the purpose
# of the being added to the workspace when using Visual Studio

add_subdirectory(main_frame)
add_subdirectory(column_editor)
add_subdirectory(mapping)

set(SOURCE_FILES ${SOURCE_FILES}
	${CMAKE_CURRENT_SOURCE_DIR}/dbcolumn_observer.h
	${CMAKE_CURRENT_SOURCE_DIR}/dbcolumn_observer.cpp

	PARENT_SCOPE
)
