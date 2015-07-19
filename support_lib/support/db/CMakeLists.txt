# CMake main file for support library
# (C) by Gerhard W. Gruber in Germany 2015
#
# We list the include files here for the purpose
# of the being added to the workspace when using Visual Studio

add_subdirectory(csv)

set(SUPPORT_SOURCE ${SUPPORT_SOURCE}
	${CMAKE_CURRENT_SOURCE_DIR}/column_types.h
	${CMAKE_CURRENT_SOURCE_DIR}/dbcolumn.h
	${CMAKE_CURRENT_SOURCE_DIR}/database_login.h
	${CMAKE_CURRENT_SOURCE_DIR}/database_login.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/type_helper.h
	${CMAKE_CURRENT_SOURCE_DIR}/type_helper.cpp

	PARENT_SCOPE
)
