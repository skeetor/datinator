/*******************************************************************************
 *
 * Datinator SQLite Plugin (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef __SQLITE_DLL_API_H__
#define __SQLITE_DLL_API_H__

#include <windows.h>

// When building the DLL library, BUILD_SQLITE_DLL must be defined
// in order to export the symbols.


#ifdef BUILD_SQLITE_STATIC
	#define SQLITE_DLL_EXPORT
	#define BUILD_CONTAINER_STATIC

	#ifdef DEBUG_EXPORT_SYMBOLS
		#warning SQLITE_STATIC
	#endif
#else
    #define BUILD_CONTAINER_DLL

	#ifdef BUILD_SQLITE_DLL
        #ifdef _WIN32
            #define SQLITE_DLL_EXPORT __declspec(dllexport)
		#ifdef DEBUG_EXPORT_SYMBOLS
			#warning SQLITE_SHARED EXPORT WIN32
		#endif
		#endif // _WIN32

        #ifdef _LINUX
            #define SQLITE_DLL_EXPORT __attribute__ ((visibility ("default")))
		#ifdef DEBUG_EXPORT_SYMBOLS
			#warning SQLITE_SHARED EXPORT LINUX
		#endif
        #endif // _LINUX
	#else
        #ifdef _WIN32
            #define SQLITE_DLL_EXPORT __declspec(dllimport)
		#ifdef DEBUG_EXPORT_SYMBOLS
			#warning SQLITE_SHARED IMPORT WIN32
		#endif
        #endif // _WIN32

        #ifdef _LINUX
            #define SQLITE_DLL_EXPORT
		#ifdef DEBUG_EXPORT_SYMBOLS
			#warning SQLITE_SHARED IMPORT LINUX
		#endif
        #endif // _LINUX
	#endif // BUILD_SQLITE_DLL
#endif

#ifdef __cplusplus
extern "C"
{
#endif



#ifdef __cplusplus
}
#endif

#endif // __SQLITE_DLL_API_H__
