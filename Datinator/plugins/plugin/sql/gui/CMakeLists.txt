# CMake Datinator sources
# (C) by Gerhard W. Gruber in Germany 2015
#

set(PLUGIN_SOURCES ${PLUGIN_SOURCES}
	${CMAKE_CURRENT_SOURCE_DIR}/commit_config.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/commit_config_gui.moc
	${CMAKE_CURRENT_SOURCE_DIR}/commit_config_gui.ui
	${CMAKE_CURRENT_SOURCE_DIR}/create_option_config.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/create_option_config_gui.moc
	${CMAKE_CURRENT_SOURCE_DIR}/create_option_config_gui.ui
	${CMAKE_CURRENT_SOURCE_DIR}/db_export_gui.moc
	${CMAKE_CURRENT_SOURCE_DIR}/db_export_gui.ui
	${CMAKE_CURRENT_SOURCE_DIR}/db_export_panel.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/db_panel.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/db_panel_gui.moc
	${CMAKE_CURRENT_SOURCE_DIR}/db_panel_gui.ui
	${CMAKE_CURRENT_SOURCE_DIR}/events.h
	${CMAKE_CURRENT_SOURCE_DIR}/sql_preview_gui.moc
	${CMAKE_CURRENT_SOURCE_DIR}/sql_preview_gui.ui
	${CMAKE_CURRENT_SOURCE_DIR}/sql_preview_panel.cpp

	PARENT_SCOPE
)
