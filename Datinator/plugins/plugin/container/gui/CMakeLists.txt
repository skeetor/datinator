# CMake Datinator sources
# (C) by Gerhard W. Gruber in Germany 2015
#

set(PLUGIN_SOURCES ${PLUGIN_SOURCES}
	${CMAKE_CURRENT_SOURCE_DIR}/file_panel.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/file_panel_config_panel.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/file_panel_config_panel.h
	${CMAKE_CURRENT_SOURCE_DIR}/file_panel_gui.moc
	${CMAKE_CURRENT_SOURCE_DIR}/file_panel_gui.ui

	PARENT_SCOPE
)
