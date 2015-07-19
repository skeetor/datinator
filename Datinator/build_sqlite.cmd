@echo off

set SQLITE_VERSION=3.8.2

cd d:\src\ext\c\sqlite-%SQLITE_VERSION%-aml

echo Building release version
gcc -O3 -c sqlite3.c -o sqlite_%SQLITE_VERSION%s.o
ar rcs libsqlite-%SQLITE_VERSION%s.a sqlite_%SQLITE_VERSION%s.o

echo Building debug version
gcc -g -c sqlite3.c -o sqlite_%SQLITE_VERSION%sd.o
ar rcs libsqlite-%SQLITE_VERSION%sd.a sqlite_%SQLITE_VERSION%sd.o

set TARGET_DIR=d:\opt\sqlite3

echo Creating target directories
mkdir "%TARGET_DIR%\include" 2>NUL:
mkdir "%TARGET_DIR%\include\sqlite" 2>NUL:
mkdir "%TARGET_DIR%\lib" 2>NUL:

echo Installing...
copy libsqlite-%SQLITE_VERSION%s.a "%TARGET_DIR%\lib"
copy libsqlite-%SQLITE_VERSION%sd.a "%TARGET_DIR%\lib"

copy sqlite3.h "%TARGET_DIR%\include\sqlite"
copy sqlite3ext.h "%TARGET_DIR%\include\sqlite"

del libsqlite-3.8.2sd.a sqlite_%SQLITE_VERSION%sd.o
del libsqlite-3.8.2s.a sqlite_%SQLITE_VERSION%s.o

echo Done!
