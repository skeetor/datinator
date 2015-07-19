# CMake source files
# (C) by Gerhard W. Gruber in Germany 2015
#
# We list the include files here for the purpose
# of the being added to the workspace when using Visual Studio

add_subdirectory(autonumber)
add_subdirectory(column)
add_subdirectory(date)
add_subdirectory(editor)
add_subdirectory(null)
add_subdirectory(text)

set(SOURCE_FILES ${SOURCE_FILES}
	${CMAKE_CURRENT_SOURCE_DIR}/imanipulator.h
	${CMAKE_CURRENT_SOURCE_DIR}/manipulator.h
	${CMAKE_CURRENT_SOURCE_DIR}/manipulator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/manipulator_panel_qt.moc
	${CMAKE_CURRENT_SOURCE_DIR}/manipulator_panel.cpp

	PARENT_SCOPE
)
