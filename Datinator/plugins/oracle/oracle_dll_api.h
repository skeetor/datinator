/*******************************************************************************
 *
 * Datinator Oracle Plugin (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef __ORACLE_DLL_API_H__
#define __ORACLE_DLL_API_H__

#include <windows.h>

// When building the DLL library, BUILD_ORACLE_DLL must be defined
// in order to export the symbols.


#ifdef BUILD_ORACLE_STATIC
	#define ORACLE_DLL_EXPORT
	#define BUILD_CONTAINER_STATIC

	#ifdef DEBUG_EXPORT_SYMBOLS
		#warning ORACLE_STATIC
	#endif
#else
    #define BUILD_CONTAINER_DLL

	#ifdef BUILD_ORACLE_DLL
        #ifdef _WIN32
            #define ORACLE_DLL_EXPORT __declspec(dllexport)
		#ifdef DEBUG_EXPORT_SYMBOLS
			#warning ORACLE_SHARED EXPORT WIN32
		#endif
		#endif // _WIN32

        #ifdef _LINUX
            #define ORACLE_DLL_EXPORT __attribute__ ((visibility ("default")))
		#ifdef DEBUG_EXPORT_SYMBOLS
			#warning ORACLE_SHARED EXPORT LINUX
		#endif
        #endif // _LINUX
	#else
        #ifdef _WIN32
            #define ORACLE_DLL_EXPORT __declspec(dllimport)
		#ifdef DEBUG_EXPORT_SYMBOLS
			#warning ORACLE_SHARED IMPORT WIN32
		#endif
        #endif // _WIN32

        #ifdef _LINUX
            #define ORACLE_DLL_EXPORT
		#ifdef DEBUG_EXPORT_SYMBOLS
			#warning ORACLE_SHARED IMPORT LINUX
		#endif
        #endif // _LINUX
	#endif // BUILD_ORACLE_DLL
#endif

#endif // __ORACLE_DLL_API_H__
