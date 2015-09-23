set CMAKE_PREFIX_PATH=D:\opt\qt

rem Default is a DLL build, so for static link you need to explicitly set it.

rem cmake -DCMAKE_BUILD_TYPE=Debug -DSTATIC_BUILD=1 -G "CodeBlocks - MinGW Makefiles" ..
rem cmake -DCMAKE_BUILD_TYPE=Release -DSTATIC_BUILD=1 -G "MinGW Makefiles" ..

rem cmake -DCMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles" ..
rem cmake -DCMAKE_BUILD_TYPE=Release -G "MinGW Makefiles" ..

rem cmake -DCMAKE_BUILD_TYPE=Debug -DUNICODE_BUILD=1 -G "MinGW Makefiles" ..
rem cmake -DCMAKE_BUILD_TYPE=Release -DUNICODE_BUILD=1 -G "MinGW Makefiles" ..

rem cmake -DCMAKE_BUILD_TYPE=Debug -DUNICODE_BUILD=1 -DSTATIC_BUILD=1 -G "MinGW Makefiles" ..
rem cmake -DCMAKE_BUILD_TYPE=Release -DUNICODE_BUILD=1 -DSTATIC_BUILD=1 -G "MinGW Makefiles" ..


rem cmake -DCMAKE_BUILD_TYPE=Debug -DSTATIC_BUILD=1 -DCMAKE_ECLIPSE_GENERATE_SOURCE_PROJECT=TRUE -G "Eclipse CDT4 - MinGW Makefiles" ..\..\Datinator

cmake -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - MinGW Makefiles" ..
