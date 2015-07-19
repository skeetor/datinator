# CMake main file for support library
# (C) by Gerhard W. Gruber in Germany 2015
#
# We list the include files here for the purpose
# of the being added to the workspace when using Visual Studio

set(SUPPORT_SOURCE ${SUPPORT_SOURCE}
	${CMAKE_CURRENT_SOURCE_DIR}/logging.h
	${CMAKE_CURRENT_SOURCE_DIR}/logging.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/logging_item_model.h
	${CMAKE_CURRENT_SOURCE_DIR}/logging_item_model.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/logging_dialog_box.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/logging_dialog_box_gui.ui
	${CMAKE_CURRENT_SOURCE_DIR}/logging_dialog_box_gui.moc

	PARENT_SCOPE
)
