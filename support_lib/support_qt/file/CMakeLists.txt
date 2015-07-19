# CMake main file for support library
# (C) by Gerhard W. Gruber in Germany 2015
#
# We list the include files here for the purpose
# of the being added to the workspace when using Visual Studio

add_subdirectory (path_panel)

set(SUPPORT_SOURCE ${SUPPORT_SOURCE}
	PARENT_SCOPE
)

