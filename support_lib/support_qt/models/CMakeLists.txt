# CMake main file for support library
# (C) by Gerhard W. Gruber in Germany 2015
#
# We list the include files here for the purpose
# of the being added to the workspace when using Visual Studio

add_subdirectory(delegates)

set(SUPPORT_SOURCE ${SUPPORT_SOURCE}
	${CMAKE_CURRENT_SOURCE_DIR}/table_item_model.h
#	${CMAKE_CURRENT_SOURCE_DIR}/default_item_model.h

	PARENT_SCOPE
)
