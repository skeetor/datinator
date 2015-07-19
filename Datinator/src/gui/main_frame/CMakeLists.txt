# CMake source files
# (C) by Gerhard W. Gruber in Germany 2015
#
# We list the include files here for the purpose
# of the being added to the workspace when using Visual Studio

set(SOURCE_FILES ${SOURCE_FILES}
	${CMAKE_CURRENT_SOURCE_DIR}/config_listener.h
	${CMAKE_CURRENT_SOURCE_DIR}/config_listener.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/reader_listener.h
	${CMAKE_CURRENT_SOURCE_DIR}/reader_listener.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/writer_listener.h
	${CMAKE_CURRENT_SOURCE_DIR}/writer_listener.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/main_frame.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/main_frame_gui.ui
	${CMAKE_CURRENT_SOURCE_DIR}/main_frame_gui.moc
	${CMAKE_CURRENT_SOURCE_DIR}/main_window.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/main_window_gui.ui
	${CMAKE_CURRENT_SOURCE_DIR}/main_window_gui.moc
	${CMAKE_CURRENT_SOURCE_DIR}/most_recent.h
	${CMAKE_CURRENT_SOURCE_DIR}/most_recent.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/most_recent_qt.moc

	PARENT_SCOPE
)
