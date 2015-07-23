# CMake main file for support library
# (C) by Gerhard W. Gruber in Germany 2015
#
# We list the include files here for the purpose
# of the being added to the workspace when using Visual Studio

set(CSV_SOURCES ${CSV_SOURCES}
	${CMAKE_CURRENT_SOURCE_DIR}/csv_writer_config_panel.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/csv_writer_config_panel_gui.ui
	${CMAKE_CURRENT_SOURCE_DIR}/csv_writer_config_panel_gui.moc

	PARENT_SCOPE
)
