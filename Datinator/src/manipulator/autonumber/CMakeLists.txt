# CMake source files
# (C) by Gerhard W. Gruber in Germany 2015
#
# We list the include files here for the purpose
# of the being added to the workspace when using Visual Studio

set(SOURCE_FILES ${SOURCE_FILES}
	${CMAKE_CURRENT_SOURCE_DIR}/autonumber.h
	${CMAKE_CURRENT_SOURCE_DIR}/autonumber.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/autonumber_panel.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/autonumber_panel_gui.ui
	${CMAKE_CURRENT_SOURCE_DIR}/autonumber_panel_gui.moc

	PARENT_SCOPE
)
