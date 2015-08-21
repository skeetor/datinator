# CMake main file for support library
# (C) by Gerhard W. Gruber in Germany 2015
#
# We list the include files here for the purpose
# of the being added to the workspace when using Visual Studio

set(SQLITE_SOURCES ${SQLITE_SOURCES}
	${CMAKE_CURRENT_SOURCE_DIR}/sqlite_writer_gui.moc
	${CMAKE_CURRENT_SOURCE_DIR}/sqlite_writer_gui.ui
	${CMAKE_CURRENT_SOURCE_DIR}/sqlite_writer_panel.cpp

	PARENT_SCOPE
)
