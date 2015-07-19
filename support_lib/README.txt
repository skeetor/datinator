support_lib contains generic helper functions which are not specific to a particular project.

The output are two libraries which comes in different flavours, depending on how the build is configured.
You can either use the CMake GUI, or provide the options on the CMake commandline. Please refer to the
CMake manual for more details. For your convenience I provided a "generate.cmd" batchfile where the
various options are show as an example.

The result will be put into the lib directory of the project. The different options will cause different
filenames, so the libaries can be all compiled withoutoverwriting each other.


Options:
========

By default the library is compiled in ANSI mode as a DLL.

The following options can be used:

STATIC_BUILD:   0 = default and a DLL is built
                1 = static build -> name contains 's'
UNICODE_BUILD:  0 = default for ANSI mode -> name contains 'a'
                1 = enable UNICODE  -> name contains 'u'

Debug:		-> name contains 'd'
Release:	-

Example: Creating a UNICODE, STATIC debug build -> support_usd.lib
The same holds true for the QT library, only it is named support_qt_usd.lib


Building support_lib
====================

You need CMake + gcc installed. For the base library you shouldn't need anything else. For building
the support_qt library you also need QT installed and possibly provide the search path where yourQT
is located.

For your convenience a 'generate.cmd'  file is provided in the root directory of the project, where
all options are shown as samples, so you can uncomment the one you need.

