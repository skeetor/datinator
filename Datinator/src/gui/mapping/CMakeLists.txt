# CMake source files
# (C) by Gerhard W. Gruber in Germany 2015
#
# We list the include files here for the purpose
# of the being added to the workspace when using Visual Studio

set(SOURCE_FILES ${SOURCE_FILES}
	${CMAKE_CURRENT_SOURCE_DIR}/column_mapping_button.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/column_mapping_button_qt.moc
	${CMAKE_CURRENT_SOURCE_DIR}/column_mapping_combobox.h
	${CMAKE_CURRENT_SOURCE_DIR}/column_mapping_combobox.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/column_mapping_delegate.h
	${CMAKE_CURRENT_SOURCE_DIR}/column_mapping_delegate.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/column_mapping_item.h
	${CMAKE_CURRENT_SOURCE_DIR}/column_mapping_item.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/column_mapping_model.h
	${CMAKE_CURRENT_SOURCE_DIR}/column_mapping_model.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/column_mapping_view_qt.moc
	${CMAKE_CURRENT_SOURCE_DIR}/column_mapping_view.cpp

	PARENT_SCOPE
)
