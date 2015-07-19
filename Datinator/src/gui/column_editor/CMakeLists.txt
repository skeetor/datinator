# CMake source files
# (C) by Gerhard W. Gruber in Germany 2015
#
# We list the include files here for the purpose
# of the being added to the workspace when using Visual Studio

set(SOURCE_FILES ${SOURCE_FILES}
	${CMAKE_CURRENT_SOURCE_DIR}/column_editor_dlg.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/column_editor_dlg_gui.ui
	${CMAKE_CURRENT_SOURCE_DIR}/column_editor_dlg_gui.moc
	${CMAKE_CURRENT_SOURCE_DIR}/column_editor_model.h
	${CMAKE_CURRENT_SOURCE_DIR}/column_editor_model.cpp

	PARENT_SCOPE
)
