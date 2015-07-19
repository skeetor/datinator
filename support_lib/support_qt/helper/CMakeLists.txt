# CMake main file for support library
# (C) by Gerhard W. Gruber in Germany 2015
#
# We list the include files here for the purpose
# of the being added to the workspace when using Visual Studio

set(SUPPORT_SOURCE ${SUPPORT_SOURCE}
	${CMAKE_CURRENT_SOURCE_DIR}/gui_helper.h
	${CMAKE_CURRENT_SOURCE_DIR}/gui_helper.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/signal_blocker.h

	PARENT_SCOPE
)
